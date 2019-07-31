#include <queue/queue.h>
#include <queue/private/element.h>
#include<iostream>
#include<fstream>
#include<memory>

#include<catch.hpp>

SCENARIO("queue_init test"){
  Queue q;
  queue_init(&q,sizeof(uint32_t));
  REQUIRE(q.elements == 0   );
  REQUIRE(q.elementSize == sizeof(uint32_t));
  REQUIRE(q.ends[QUEUE_FRONT] == NULL);
  REQUIRE(q.ends[QUEUE_BACK] == NULL);
}

SCENARIO("queue_empty test"){
  Queue q;
  queue_init(&q,sizeof(uint32_t));
  REQUIRE((bool)queue_empty(&q) == true);
}

SCENARIO("queue_push test"){
  uint32_t v;
  Queue q;
  queue_init(&q,sizeof(uint32_t));

  REQUIRE(q.ends[0] == NULL);
  REQUIRE(q.ends[1] == NULL);

  v=10;
  REQUIRE(queue_push(&q,&v) == ERR_OK);
  REQUIRE(q.ends[0] != NULL);
  REQUIRE(q.ends[0]->neighbors[1] == NULL);
  REQUIRE(q.ends[1] != NULL);
  REQUIRE(q.ends[1]->neighbors[0] == NULL);
  
  REQUIRE(q.ends[0] == q.ends[1]);

  REQUIRE(q.ends[0]->data != NULL);

  REQUIRE(*(uint32_t*)(q.ends[0]->data) == 10);

  v=11;
  REQUIRE(queue_push(&q,&v) == ERR_OK);
  REQUIRE(q.ends[0] != NULL);
  REQUIRE(q.ends[0]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1] == NULL);
  REQUIRE(q.ends[1] != NULL);
  REQUIRE(q.ends[1]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0] == NULL);

  REQUIRE(q.ends[0]->neighbors[1] == q.ends[1]);
  REQUIRE(q.ends[1]->neighbors[0] == q.ends[0]);
  
  REQUIRE(q.ends[0]->data != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->data != NULL);

  REQUIRE(*(uint32_t*)q.ends[0]->data == 10);
  REQUIRE(*(uint32_t*)q.ends[0]->neighbors[1]->data == 11);

  v=12;
  REQUIRE(queue_push(&q,&v) == ERR_OK);
  REQUIRE(q.ends[0] != NULL);
  REQUIRE(q.ends[0]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->neighbors[1] == NULL);
  REQUIRE(q.ends[1] != NULL);
  REQUIRE(q.ends[1]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0]->neighbors[0] == NULL);

  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1] == q.ends[1]);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0] == q.ends[0]);
  
  REQUIRE(q.ends[0]->data != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->data != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->data != NULL);

  REQUIRE(*(uint32_t*)q.ends[0]->data == 10);
  REQUIRE(*(uint32_t*)q.ends[0]->neighbors[1]->data == 11);
  REQUIRE(*(uint32_t*)q.ends[0]->neighbors[1]->neighbors[1]->data == 12);

  v=13;
  REQUIRE(queue_push(&q,&v) == ERR_OK);
  REQUIRE(q.ends[0] != NULL);
  REQUIRE(q.ends[0]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->neighbors[1]->neighbors[1] == NULL);
  REQUIRE(q.ends[1] != NULL);
  REQUIRE(q.ends[1]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0]->neighbors[0]->neighbors[0] == NULL);

  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->neighbors[1] == q.ends[1]);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0]->neighbors[0] == q.ends[0]);
  
  REQUIRE(q.ends[0]->data != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->data != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->data != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->neighbors[1]->data != NULL);
  
  REQUIRE(*(uint32_t*)q.ends[0]->data == 10);
  REQUIRE(*(uint32_t*)q.ends[0]->neighbors[1]->data == 11);
  REQUIRE(*(uint32_t*)q.ends[0]->neighbors[1]->neighbors[1]->data == 12);
  REQUIRE(*(uint32_t*)q.ends[0]->neighbors[1]->neighbors[1]->neighbors[1]->data == 13);

  queue_free(&q);
}

SCENARIO("queue_pop test"){
  uint32_t v;
  Queue q;
  queue_init(&q,sizeof(uint32_t));

  v=10;REQUIRE(queue_push(&q,&v) == ERR_OK);
  v=11;REQUIRE(queue_push(&q,&v) == ERR_OK);
  v=12;REQUIRE(queue_push(&q,&v) == ERR_OK);
  v=13;REQUIRE(queue_push(&q,&v) == ERR_OK);

  REQUIRE(queue_pop(&q,&v) == ERR_OK);
  REQUIRE(v == 10);
  
  REQUIRE(q.ends[0] != NULL);
  REQUIRE(q.ends[0]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1]->neighbors[1] == NULL);
  REQUIRE(q.ends[1] != NULL);
  REQUIRE(q.ends[1]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0]->neighbors[0] == NULL);

  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1] == q.ends[1]);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0] == q.ends[0]);
  
  REQUIRE(queue_pop(&q,&v) == ERR_OK);
  REQUIRE(v == 11);
  
  REQUIRE(q.ends[0] != NULL);
  REQUIRE(q.ends[0]->neighbors[1] != NULL);
  REQUIRE(q.ends[0]->neighbors[1]->neighbors[1] == NULL);
  REQUIRE(q.ends[1] != NULL);
  REQUIRE(q.ends[1]->neighbors[0] != NULL);
  REQUIRE(q.ends[1]->neighbors[0]->neighbors[0] == NULL);

  REQUIRE(q.ends[0]->neighbors[1] == q.ends[1]);
  REQUIRE(q.ends[1]->neighbors[0] == q.ends[0]);
  
  REQUIRE(queue_pop(&q,&v) == ERR_OK);
  REQUIRE(v == 12);
  
  REQUIRE(q.ends[0] != NULL);
  REQUIRE(q.ends[0]->neighbors[1] == NULL);
  REQUIRE(q.ends[1] != NULL);
  REQUIRE(q.ends[1]->neighbors[0] == NULL);
  
  REQUIRE(q.ends[0] == q.ends[1]);
  
  REQUIRE(queue_pop(&q,&v) == ERR_OK);
  REQUIRE(v == 13);
  
  REQUIRE(q.ends[0] == NULL);
  REQUIRE(q.ends[1] == NULL);

  REQUIRE(queue_pop(&q,NULL) != ERR_OK);

  queue_free(&q);
}

SCENARIO("queue_empty queue_elements test"){
  uint32_t v=0;
  Queue queue;
  queue_init(&queue,sizeof(uint32_t));
  REQUIRE((bool)queue_empty(&queue) == true);
  REQUIRE(queue_elements(&queue) == 0);
  queue_push(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 1);
  queue_push(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 2);
  queue_push(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 3);
  queue_pop(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 2);
  queue_pop(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 1);
  queue_pop(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == true);
  REQUIRE(queue_elements(&queue) == 0);
  queue_free(&queue);
}

SCENARIO("queue large element"){
  struct Elem{
    uint32_t a,b,c;
    Elem(uint32_t A=0,uint32_t B=0,uint32_t C=0):a(A),b(B),c(C){}
    bool operator==(Elem const&e)const{return a==e.a&&b==e.b&&c==e.c;}
  };
  Elem v;
  Queue queue;
  queue_init(&queue,sizeof(Elem));
  REQUIRE((bool)queue_empty(&queue) == true);
  REQUIRE(queue_elements(&queue) == 0);
  REQUIRE(queue_elementSize(&queue) == sizeof(Elem));

  v = Elem(1,2,3);
  REQUIRE(queue_push(&queue,&v) == ERR_OK);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 1);

  v = Elem(4,5,6);
  REQUIRE(queue_push(&queue,&v) == ERR_OK);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 2);

  v = Elem(7,8,9);
  REQUIRE(queue_push(&queue,&v) == ERR_OK);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 3);

  v = Elem();

  queue_pop(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 2);
  REQUIRE(v == Elem(1,2,3));

  queue_pop(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == false);
  REQUIRE(queue_elements(&queue) == 1);
  REQUIRE(v == Elem(4,5,6));

  queue_pop(&queue,&v);
  REQUIRE((bool)queue_empty(&queue) == true);
  REQUIRE(queue_elements(&queue) == 0);
  REQUIRE(v == Elem(7,8,9));

  queue_free(&queue);
}

SCENARIO("queue_front queue_back test"){
  uint32_t v=0;
  Queue queue;
  queue_init(&queue,sizeof(uint32_t));
  REQUIRE(queue_front(&queue) == NULL);
  REQUIRE(queue_back(&queue) == NULL);

  v=13;
  REQUIRE(queue_push(&queue,&v) == ERR_OK);
  REQUIRE(queue_front(&queue) != NULL);
  REQUIRE(queue_back(&queue) != NULL);
  REQUIRE(*(uint32_t*)queue_front(&queue) == 13);
  REQUIRE(*(uint32_t*)queue_back(&queue) == 13);

  v=123;
  REQUIRE(queue_push(&queue,&v) == ERR_OK);
  REQUIRE(queue_front(&queue) != NULL);
  REQUIRE(queue_back(&queue) != NULL);
  REQUIRE(*(uint32_t*)queue_front(&queue) == 13);
  REQUIRE(*(uint32_t*)queue_back(&queue) == 123);

  queue_free(&queue);
}
