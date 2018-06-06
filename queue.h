#ifndef QUEUE_H
#define QUEUE_H

//queue data structures
typedef struct node {
  char* dir;
  struct node* prev;
  struct node* next;  
} queueNode;

typedef struct queue {
  queueNode* head;
  queueNode* tail;  
} queue;

//constructors
queueNode* makeNode();
queue* makeQueue();

int isEmpty(queue* q);

//queue operations
void enqueue(queue* q, queueNode* qnode);
queueNode* dequeue(queue* q);
void join(queue* q1, queue* q2);

void printQueue(queue* q);

#endif
