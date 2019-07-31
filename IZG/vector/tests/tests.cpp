#include <vector/vector.h>
#include <vector/private.h>
#include<iostream>
#include<fstream>
#include<memory>

#include<catch.hpp>

SCENARIO("vector_init test"){
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  REQUIRE(vector.size == 0   );
  REQUIRE(vector.reserved     == 0   );
  REQUIRE(vector.data   == NULL);
  REQUIRE(vector.elementSize == sizeof(uint32_t));
}

SCENARIO("vector_empty test"){
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  REQUIRE((bool)vector_empty(&vector) == true);
}

SCENARIO("vector_firstAlloc and vector_free test"){
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  REQUIRE(vector_firstAlloc(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 1   );
  REQUIRE(vector.data != NULL);
  vector_free(&vector);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 0   );
  REQUIRE(vector.data == NULL);
}

SCENARIO("vector_realloc test"){
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  vector_firstAlloc(&vector);
  REQUIRE(vector_realloc(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 2   );
  REQUIRE(vector.data != NULL);
  REQUIRE(vector_realloc(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 4   );
  REQUIRE(vector.data != NULL);
  REQUIRE(vector_realloc(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 8   );
  REQUIRE(vector.data != NULL);
  vector_free(&vector);
}

SCENARIO("vector_rereserved test"){
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  REQUIRE(vector_resize(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 1   );
  REQUIRE(vector.data != NULL);
  REQUIRE(vector_resize(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 2   );
  REQUIRE(vector.data != NULL);
  REQUIRE(vector_resize(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 4   );
  REQUIRE(vector.data != NULL);
  REQUIRE(vector_resize(&vector) == ERR_OK);
  REQUIRE(vector.size   == 0   );
  REQUIRE(vector.reserved   == 8   );
  REQUIRE(vector.data != NULL);
  vector_free(&vector);
}

SCENARIO("vector_pushback test"){
  uint32_t v;
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  v=0;
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE(vector.size == 1);
  REQUIRE(vector.reserved == 1);
  REQUIRE(((uint32_t*)vector.data)[0] == 0);
  v=1;
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE(vector.size == 2);
  REQUIRE(vector.reserved == 2);
  REQUIRE(((uint32_t*)vector.data)[0] == 0);
  REQUIRE(((uint32_t*)vector.data)[1] == 1);
  v=2;
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE(vector.size == 3);
  REQUIRE(vector.reserved == 4);
  REQUIRE(((uint32_t*)vector.data)[0] == 0);
  REQUIRE(((uint32_t*)vector.data)[1] == 1);
  REQUIRE(((uint32_t*)vector.data)[2] == 2);

  vector_free(&vector);
}

SCENARIO("vector_empty vector_size test"){
  uint32_t v=0;
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  REQUIRE((bool)vector_empty(&vector) == true);
  REQUIRE(vector_size(&vector) == 0);
  vector_pushback(&vector,&v);
  REQUIRE((bool)vector_empty(&vector) == false);
  REQUIRE(vector_size(&vector) == 1);
  vector_pushback(&vector,&v);
  REQUIRE((bool)vector_empty(&vector) == false);
  REQUIRE(vector_size(&vector) == 2);
  vector_pushback(&vector,&v);
  REQUIRE((bool)vector_empty(&vector) == false);
  REQUIRE(vector_size(&vector) == 3);
  vector_free(&vector);
}

SCENARIO("vector_get vector_set test"){
  uint32_t v=0;
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));

  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);

  v = 100;
  REQUIRE(vector_set(&vector,0,&v) == ERR_OK);
  REQUIRE(((uint32_t*)vector.data)[0] == 100);
  REQUIRE(((uint32_t*)vector.data)[1] == 0  );
  v = 101;
  REQUIRE(vector_set(&vector,1,&v) == ERR_OK);
  REQUIRE(((uint32_t*)vector.data)[0] == 100);
  REQUIRE(((uint32_t*)vector.data)[1] == 101);

  REQUIRE(vector_get(&vector,0,&v) == ERR_OK);
  REQUIRE(v == 100);
  REQUIRE(vector_get(&vector,1,&v) == ERR_OK);
  REQUIRE(v == 101);

  REQUIRE(vector_get(&vector,2,&v) != ERR_OK);
  REQUIRE(vector_set(&vector,2,&v) != ERR_OK);

  vector_free(&vector);
}


SCENARIO("vector_at test"){
  uint32_t v=0;
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  REQUIRE(vector_at(&vector,0) == NULL);

  v=13;
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE(vector_at(&vector,0) != NULL);
  REQUIRE(*(uint32_t*)vector_at(&vector,0) == 13);

  v=123;
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE(vector_at(&vector,0) != NULL);
  REQUIRE(vector_at(&vector,1) != NULL);
  REQUIRE(*(uint32_t*)vector_at(&vector,0) == 13 );
  REQUIRE(*(uint32_t*)vector_at(&vector,1) == 123);

  vector_free(&vector);
}

SCENARIO("vector large element"){
  struct Elem{
    uint32_t a,b,c;
    Elem(uint32_t A=0,uint32_t B=0,uint32_t C=0):a(A),b(B),c(C){}
    bool operator==(Elem const&e)const{return a==e.a&&b==e.b&&c==e.c;}
  };
  Elem v;
  Vector vector;
  vector_init(&vector,sizeof(Elem));
  REQUIRE((bool)vector_empty(&vector) == true);
  REQUIRE(vector_size(&vector) == 0);
  REQUIRE(vector_elementSize(&vector) == sizeof(Elem));

  v = Elem(1,2,3);
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE((bool)vector_empty(&vector) == false);
  REQUIRE(vector_size(&vector) == 1);
  REQUIRE(*(Elem*)vector_at(&vector,0) == Elem(1,2,3));

  v = Elem(4,5,6);
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE((bool)vector_empty(&vector) == false);
  REQUIRE(vector_size(&vector) == 2);
  REQUIRE(*(Elem*)vector_at(&vector,0) == Elem(1,2,3));
  REQUIRE(*(Elem*)vector_at(&vector,1) == Elem(4,5,6));


  v = Elem(7,8,9);
  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE((bool)vector_empty(&vector) == false);
  REQUIRE(vector_size(&vector) == 3);
  REQUIRE(*(Elem*)vector_at(&vector,0) == Elem(1,2,3));
  REQUIRE(*(Elem*)vector_at(&vector,1) == Elem(4,5,6));
  REQUIRE(*(Elem*)vector_at(&vector,2) == Elem(7,8,9));

  vector_free(&vector);
}

SCENARIO("vector_data test"){
  uint32_t v=0;
  Vector vector;
  vector_init(&vector,sizeof(uint32_t));
  REQUIRE(vector_data(&vector) == NULL);

  REQUIRE(vector_pushback(&vector,&v) == ERR_OK);
  REQUIRE(vector.data == vector_data(&vector));

  vector_free(&vector);
}

