#include <stack/stack.h>
#include <stack/private.h>
#include<iostream>
#include<fstream>
#include<memory>
#include<string.h>

#include<catch.hpp>

SCENARIO("stack_init test"){
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  REQUIRE(stack.size        == 0   );
  REQUIRE(stack.size        == 0   );
  REQUIRE(stack.values      == NULL);
  REQUIRE(stack.elementSize == sizeof(uint32_t));
}

SCENARIO("stack_empty test"){
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  REQUIRE((bool)stack_empty(&stack) == true);
}

SCENARIO("stack_firstAlloc and stack_free test"){
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  REQUIRE(stack_firstAlloc(&stack) == ERR_OK);
  REQUIRE(stack.size     == 0   );
  REQUIRE(stack.reserved == 1   );
  REQUIRE(stack.values   != NULL);
  stack_free(&stack);
  REQUIRE(stack.size     == 0   );
  REQUIRE(stack.reserved == 0   );
  REQUIRE(stack.values   == NULL);
}

SCENARIO("stack_realloc test"){
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  stack_firstAlloc(&stack);
  REQUIRE(stack_realloc(&stack) == ERR_OK);
  REQUIRE(stack.size     == 0   );
  REQUIRE(stack.reserved == 2   );
  REQUIRE(stack.values != NULL);
  REQUIRE(stack_realloc(&stack) == ERR_OK);
  REQUIRE(stack.size     == 0   );
  REQUIRE(stack.reserved == 4   );
  REQUIRE(stack.values != NULL);
  REQUIRE(stack_realloc(&stack) == ERR_OK);
  REQUIRE(stack.size     == 0   );
  REQUIRE(stack.reserved == 8   );
  REQUIRE(stack.values != NULL);
  stack_free(&stack);
}

SCENARIO("stack_resize test"){
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  REQUIRE(stack_resize(&stack) == ERR_OK);
  REQUIRE(stack.size   == 0   );
  REQUIRE(stack.reserved   == 1   );
  REQUIRE(stack.values != NULL);
  REQUIRE(stack_resize(&stack) == ERR_OK);
  REQUIRE(stack.size   == 0   );
  REQUIRE(stack.reserved   == 2   );
  REQUIRE(stack.values != NULL);
  REQUIRE(stack_resize(&stack) == ERR_OK);
  REQUIRE(stack.size   == 0   );
  REQUIRE(stack.reserved   == 4   );
  REQUIRE(stack.values != NULL);
  REQUIRE(stack_resize(&stack) == ERR_OK);
  REQUIRE(stack.size   == 0   );
  REQUIRE(stack.reserved   == 8   );
  REQUIRE(stack.values != NULL);
  stack_free(&stack);
}

SCENARIO("stack_push stack_pop test"){
  uint32_t v;
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  v=0;
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE(stack.size == 1);
  REQUIRE(stack.reserved == 1);
  REQUIRE(((uint32_t*)stack.values)[0] == 0);
  v=1;
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE(stack.size == 2);
  REQUIRE(stack.reserved == 2);
  REQUIRE(((uint32_t*)stack.values)[0] == 0);
  REQUIRE(((uint32_t*)stack.values)[1] == 1);
  v=2;
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE(stack.size == 3);
  REQUIRE(stack.reserved == 4);
  REQUIRE(((uint32_t*)stack.values)[0] == 0);
  REQUIRE(((uint32_t*)stack.values)[1] == 1);
  REQUIRE(((uint32_t*)stack.values)[2] == 2);
  REQUIRE(stack_pop(&stack,&v) == ERR_OK);
  REQUIRE(v == 2);
  REQUIRE(stack.size == 2);
  REQUIRE(stack.reserved == 4);
  REQUIRE(((uint32_t*)stack.values)[0] == 0);
  REQUIRE(((uint32_t*)stack.values)[1] == 1);
  REQUIRE(stack_pop(&stack,&v) == ERR_OK);
  REQUIRE(v == 1);
  REQUIRE(stack.size == 1);
  REQUIRE(stack.reserved == 4);
  REQUIRE(((uint32_t*)stack.values)[0] == 0);
  REQUIRE(stack_pop(&stack,&v) == ERR_OK);
  REQUIRE(v == 0);
  REQUIRE(stack.size == 0);
  REQUIRE(stack.reserved == 4);
  REQUIRE(stack_pop(&stack,&v) != ERR_OK);

  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE(stack_pop(&stack,NULL) == ERR_OK);

  stack_free(&stack);
}

SCENARIO("stack_empty stack_size test"){
  uint32_t v=0;
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  REQUIRE((bool)stack_empty(&stack) == true);
  REQUIRE(stack_size(&stack) == 0);
  stack_push(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 1);
  stack_push(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 2);
  stack_push(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 3);
  stack_pop(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 2);
  stack_pop(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 1);
  stack_pop(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == true);
  REQUIRE(stack_size(&stack) == 0);
  stack_free(&stack);
}

SCENARIO("stack large element"){
  struct Elem{
    uint32_t a,b,c;
    Elem(uint32_t A=0,uint32_t B=0,uint32_t C=0):a(A),b(B),c(C){}
    bool operator==(Elem const&e)const{return a==e.a&&b==e.b&&c==e.c;}
  };
  Elem v;
  Stack stack;
  stack_init(&stack,sizeof(Elem));
  REQUIRE((bool)stack_empty(&stack) == true);
  REQUIRE(stack_size(&stack) == 0);
  REQUIRE(stack_elementSize(&stack) == sizeof(Elem));

  v = Elem(1,2,3);
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 1);

  v = Elem(4,5,6);
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 2);

  v = Elem(7,8,9);
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 3);

  v = Elem();

  stack_pop(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 2);
  REQUIRE(v == Elem(7,8,9));

  stack_pop(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == false);
  REQUIRE(stack_size(&stack) == 1);
  REQUIRE(v == Elem(4,5,6));

  stack_pop(&stack,&v);
  REQUIRE((bool)stack_empty(&stack) == true);
  REQUIRE(stack_size(&stack) == 0);
  REQUIRE(v == Elem(1,2,3));

  stack_free(&stack);
}

SCENARIO("stack_top test"){
  uint32_t v=0;
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  REQUIRE(stack_top(&stack) == NULL);

  v=13;
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE(stack_top(&stack) != NULL);
  REQUIRE(*(uint32_t*)stack_top(&stack) == 13);

  v=123;
  REQUIRE(stack_push(&stack,&v) == ERR_OK);
  REQUIRE(stack_top(&stack) != NULL);
  REQUIRE(*(uint32_t*)stack_top(&stack) == 123);

  stack_free(&stack);
}

SCENARIO("stack_at and stack_contain test"){
  uint32_t v=0;
  Stack stack;
  stack_init(&stack,sizeof(uint32_t));
  v=12;
  stack_push(&stack,&v);
  v=13;
  stack_push(&stack,&v);
  v=203;
  stack_push(&stack,&v);
  v=2032;
  stack_push(&stack,&v);

  v = 12;
  REQUIRE(memcmp(stack_at(&stack,0),&v,sizeof(uint32_t)) == 0);
  v = 13;
  REQUIRE(memcmp(stack_at(&stack,1),&v,sizeof(uint32_t)) == 0);
  v = 203;
  REQUIRE(memcmp(stack_at(&stack,2),&v,sizeof(uint32_t)) == 0);
  v = 2032;
  REQUIRE(memcmp(stack_at(&stack,3),&v,sizeof(uint32_t)) == 0);

  v = 111;
  REQUIRE(stack_contain(&stack,&v) == 0);
  v = 203;
  REQUIRE(stack_contain(&stack,&v) == 1);

  stack_free(&stack);
}
