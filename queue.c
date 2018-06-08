#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

//constructors
queueNode* makeNode() {
  queueNode* qnode = (queueNode*) malloc(sizeof(queueNode));
  qnode->dir = NULL;
  qnode->prev = qnode->next = NULL;
}

queue* makeQueue() {
  queue* q = (queue*) malloc(sizeof(queue));
  q->head = q->tail = NULL;
}


//destructors
void freeQueueNode(queueNode* qn) {
  free(qn->dir);
  free(qn);  
}

void freeQueue(queue* q) {

  queueNode* qn = NULL;
  while(!isEmpty(q)) {
    qn = dequeue(q);
    freeQueueNode(qn);
  }

  free(q);
}


//returns if queue has any elements in it
int isEmpty(queue* q) {
  return q->head == NULL;
}

//queue operations
void enqueue(queue* q, queueNode* qnode) {

  //if either reference is null, return
  if(qnode == NULL || q == NULL)
    return;

  //if the queue is empty, then add the first element
  if(q->head == NULL) {
    qnode->next = qnode->prev = NULL;
    q->head = q->tail = qnode;
    return;
  }

  qnode->prev = NULL;
  qnode->next = q->head;
  q->head->prev = qnode;
  q->head = qnode;
}

queueNode* dequeue(queue* q) {
  
  if(q == NULL)
    return NULL;

  if(q->tail == NULL) {
    q->head = q->tail = NULL;
    return NULL;
  }   

  queueNode* ret = q->tail;
  if(q->tail->prev == NULL) {
    q->head = q->tail = NULL;
  } else {
    q->tail = q->tail->prev;
    q->tail->next = NULL;
  }

  return ret;
}



//concatenate second queue to the first
void join(queue* q1, queue* q2) {
  
  //if either reference is null, return
  if(q1 == NULL || q2 == NULL)
    return;
  
  if(q1->tail == NULL) {
    q1->tail = q1->head = q2->head;
  }
  
  if(q2->head == NULL)
    return;

  q1->tail->next = q2->head;
  q2->head->prev = q1->tail;
}

//print queue
void printQueue(queue* q) {

  if(q == NULL) {
    printf("NIL\n");
    return;
  }
    

  printf("{");

  int first = 1;
  queueNode* qnode = q->head;

  while(qnode != NULL) {

    if(first) {
      printf("%s", qnode->dir);
      first = 0; 
    } else {
      printf(", %s", qnode->dir);
    }

    qnode = qnode->next;
  }

  printf("}\n");  
}





