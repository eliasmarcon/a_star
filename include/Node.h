#ifndef node_h
#define node_h
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


 struct Node
 {
     float metric;
     int edgeCount;
     struct Edge **edges;
     int x;
     int y;
     int id;
 };

struct Edge
{
    int weight;
    struct Node *node;
};

/*struct Node {
    float metric;
    int edgeCount;
    struct Edge **edges;
    int x, y;
    int visited;
    int subgraph;
    int isSubgraphRoot; // New field to indicate if the node is a subgraph root
};*/

struct Node* mazeToGraph(int **maze, int n, struct Node* exitNode);
void printNode(struct Node *node);
int totalNodes(struct Node *root); 
void buildAdjacencyMatrix(struct Node * node, int ** matrix);
struct Node** a_star(struct Node *start, struct Node *goal, int size, int *pathLength);
void printPath(struct Node** path, int pathLength);
/*
void printSubgraph(struct Node *node,int id);
struct Node** splitTree(struct Node *root, int n);
int totalSubgraphNodes(struct Node *root,int id);
*/
#endif /* node_h */