#include "PriorityQueue.h"
#include <stdlib.h>

void heapify(struct PriorityQueue *pq, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq->size && pq->elements[left].fScore < pq->elements[smallest].fScore)
        smallest = left;

    if (right < pq->size && pq->elements[right].fScore < pq->elements[smallest].fScore)
        smallest = right;

    if (smallest != i) {
        struct PQNode temp = pq->elements[i];
        pq->elements[i] = pq->elements[smallest];
        pq->elements[smallest] = temp;
        heapify(pq, smallest);
    }
}

void initializePriorityQueue(struct PriorityQueue *pq, int capacity) {
    pq->elements = (struct PQNode *)malloc(sizeof(struct PQNode) * capacity);
    pq->capacity = capacity;
    pq->size = 0;
}

void insertWithPriority(struct PriorityQueue *pq, struct Node *node, float fScore) {
    if (pq->size == pq->capacity) {
        // Resize the heap array if needed
        pq->capacity *= 2;
        pq->elements = realloc(pq->elements, sizeof(struct PQNode) * pq->capacity);
    }

    int i = pq->size++;
    pq->elements[i].node = node;
    pq->elements[i].fScore = fScore;

    while (i != 0 && pq->elements[(i - 1) / 2].fScore > pq->elements[i].fScore) {
        struct PQNode temp = pq->elements[i];
        pq->elements[i] = pq->elements[(i - 1) / 2];
        pq->elements[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

struct Node* popLowest(struct PriorityQueue *pq) {
    if (pq->size <= 0) return NULL;

    struct Node* root = pq->elements[0].node;
    pq->elements[0] = pq->elements[--pq->size];
    heapify(pq, 0);

    return root;
}

bool isEmpty(struct PriorityQueue *pq) {
    return pq->size == 0;
}

bool containsInPriorityQueue(struct PriorityQueue *pq, struct Node *node) {
    for (int i = 0; i < pq->size; i++) {
        if (pq->elements[i].node == node) {
            return true;
        }
    }
    return false;
}