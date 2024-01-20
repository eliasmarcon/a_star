#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>
#include "Node.h"

struct PQNode {
    struct Node *node;
    float fScore;
};

struct PriorityQueue {
    struct PQNode *elements;
    int capacity;
    int size;
};

void initializePriorityQueue(struct PriorityQueue *pq, int capacity);
void insertWithPriority(struct PriorityQueue *pq, struct Node *node, float fScore);
struct Node* popLowest(struct PriorityQueue *pq);
bool isEmpty(struct PriorityQueue *pq);
bool containsInPriorityQueue(struct PriorityQueue *pq, struct Node *node);

#endif // PRIORITY_QUEUE_H
