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
    int metric;
    int edgeCount;
    struct Edge **edges;
};

char *NodetoString(const struct Node *node);
char *convertToJson(const struct Node *node);
struct Node *convertFromJson(const char *jsonString);
void printChild(struct Node *node, int i);
void printChildren(struct Node *node, int rank);
void printFullGraph(struct Node *);
#endif /* node_h */