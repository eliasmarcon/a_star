#ifndef node_h
#define node_h
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Edge
{
    int weight;
    struct Node *node;
};

struct Node
{
    float metric;
    int edgeCount;
    struct Edge **edges;
    int x;
    int y;
};

struct Node* mazeToGraph(int **maze, int n);
void printNode(struct Node *node);
#endif /* node_h */