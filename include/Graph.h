#ifndef graph_h
#define graph_h
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Edge
{
    int neighbor;
    int weight;
};


int generateRandomConnectedNetwork(int numNodes, int insertConnections, int **index, int **edges);
void printGraph(int numNodes, int *index, int *edges);

#endif // graph_h