#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h"
#include <math.h>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

const int neighboursHelper[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right


struct Node* getNeigbours(int ** maze, int x,int y, int * exit, int n, int parentDirection);

float calculateEucledianDistance(int x, int y, int * exit) {
    int dx = exit[1] - x;
    int dy = exit[0] - y;
    return sqrt((dx * dx) + (dy * dy));
}

struct Node* mazeToGraph(int **maze, int n) {
    // define entry and exit points
    int entryY = -1;
    int exitY = -1;
    for(int i=0;i<n;i++){
        if(maze[i][0] == 0){
            entryY = i;
        }
    }
    for(int i=0;i<n;i++){
        if(maze[i][n-1] == 0){
            exitY = i;
        }
    }
    int exit[2] = { exitY,n - 1 };
    printf("Exit: %d, %d\n", exit[1], exit[0]);
    struct Node* entry = getNeigbours(maze, 0, entryY, (int *)&exit, n, 2);
    return entry;
}

int isValidCoordinate(int x, int y, int n) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

struct Node* getNeigbours(int ** maze, int x,int y, int * exit, int n, int parentDirection) {
    struct Node * thisNode = (struct Node*)malloc(sizeof(struct Node));
    thisNode->metric = calculateEucledianDistance(x, y, exit);
    thisNode->subgraph = -1;
    thisNode->x = x;
    thisNode->y = y;
    if(exit[0] == y && exit[1] == x){
        thisNode->edgeCount = 0;
        return thisNode;
    }
    int neighboursCount = 0;
    bool neighDirection[4] = {false, false, false, false};
    for (int i = 0; i < 4; i++) {
        if(i == parentDirection){
            continue;
        }
        int newX = x + neighboursHelper[i][0];
        int newY = y + neighboursHelper[i][1];
        if (newX >= 0 && newX < n && newY >= 0 && newY < n && maze[newY][newX] == 0) {
            neighDirection[i] = true;
            neighboursCount++;
        }
    }
    thisNode->edgeCount = neighboursCount;
    thisNode->edges = (struct Edge**)malloc(sizeof(struct Edge*) * thisNode->edgeCount);
    int edgeIndex = 0;
    for(int direction = 0; direction < 4; direction++){
        if(!neighDirection[direction]){
            continue;
        }
        int weight = 0;
        int newX = x;
        int newY = y;
        
        //follow the path to the next crossing
        int newXTemp, newYTemp;
        do{
            newXTemp = newX + neighboursHelper[direction][0];
            newYTemp = newY + neighboursHelper[direction][1];
            if (isValidCoordinate(newXTemp, newYTemp, n) && maze[newYTemp][newXTemp] == 0) {
                weight++;
                newX = newXTemp;
                newY = newYTemp;
            }
        }while(isValidCoordinate(newXTemp, newYTemp, n) && maze[newYTemp][newXTemp] == 0);
        // invert the direction of the edge to get the new parentDirection
        int newParentDirection;
        if(direction == UP){
            newParentDirection = DOWN;
        }else if(direction == DOWN){
            newParentDirection = UP;
        }else if(direction == LEFT){
            newParentDirection = RIGHT;
        }else if(direction == RIGHT){
            newParentDirection = LEFT;
        }
        // add neighbour to the node / add an edge with the weight of the distance to the crossing

        // recursively call getNeigbours with the new coordinates and the new parentDirection
        struct Node* neighbour = getNeigbours(maze, newX, newY, exit, n, newParentDirection);
        thisNode->edges[edgeIndex] = (struct Edge*)malloc(sizeof(struct Edge));
        thisNode->edges[edgeIndex]->weight = weight;
        thisNode->edges[edgeIndex]->node = neighbour;
        edgeIndex++;
    }

    return thisNode;
}


void printNode(struct Node *node)
{
    printf("Node (%d,%d) - Metric: %.2f\n",node->x,node->y, node->metric);
    for(int i = 0; i < node->edgeCount; i++){
        printf("\tEdge %d - Weight: %d, Node: (%d,%d)\n", i, node->edges[i]->weight, node->edges[i]->node->x, node->edges[i]->node->y);
    }
    for(int i = 0; i < node->edgeCount; i++){
        printNode(node->edges[i]->node);
    }
}

void printSubgraph(struct Node *node,int id)
{
    if(node->subgraph != id) return;
    printf("Node (%d,%d) - Metric: %.2f\n",node->x,node->y, node->metric);
    for(int i = 0; i < node->edgeCount; i++){
        printf("\tEdge %d - Weight: %d, Node: (%d,%d)\n", i, node->edges[i]->weight, node->edges[i]->node->x, node->edges[i]->node->y);
    }
    
    for(int i = 0; i < node->edgeCount; i++){
        printSubgraph(node->edges[i]->node, id);
    }
}


int totalNodes(struct Node *root) {
    int count = 1; // Count this node
    for (int i = 0; i < root->edgeCount; ++i) {
        count += totalNodes(root->edges[i]->node);
    }
    return count;
}

int totalSubgraphNodes(struct Node *root,int id) {
    if(root->subgraph != id) return 0;
    int count = 1; // Count this node
    for (int i = 0; i < root->edgeCount; ++i) {
        count += totalSubgraphNodes(root->edges[i]->node, id);
    }
    return count;
}

void dfs(struct Node *node, int *targetSize, int *currentSize, int *subgraphId, struct Node **subgraphRoots) {
    // We are a root node either if we are the first node or have more children than space in the subgraph
    if ((*currentSize == 0 )|| (*currentSize + node->edgeCount > *targetSize)){
        *currentSize = 0;
        node->isSubgraphRoot = 1;
        subgraphRoots[--(*subgraphId)] = node;
    } else {
        node->isSubgraphRoot = 0;
    }
    
    // add node to subgraph
    node->subgraph = *subgraphId;
    *currentSize += 1;
    
    if(*currentSize >= *targetSize){
        // Check if childern are leaves and append them to the subgraph
        // for (int i = 0; i < node->edgeCount; ++i) {
        //     if(node->edges[i]->node->edgeCount == 0){
        //         node->edges[i]->node->isSubgraphRoot = 0;
        //         node->edges[i]->node->subgraph = *subgraphId;
        //         (*currentSize)++;
        //     }
        // }
        if(subgraphId > 0){
            *currentSize = 0;
        }
    }
    // Continue DFS recursively
    for (int i = 0; i < node->edgeCount; ++i) {
        // Skip if already in a subgraph
        if(node->edges[i]->node->subgraph != -1){
            continue;
        }
        dfs(node->edges[i]->node, targetSize, currentSize, subgraphId, subgraphRoots);
    }
}


struct Node** splitTree(struct Node *root, int n) {
    int total = totalNodes(root);
    printf("Total: %d\n", total);
    int targetSize = (total + n - 1) / n;
    printf("Target: %d\n", targetSize);
    int currentSize = 0;
    // first rootNode decrements subgraphId to n-1
    int subgraphId = n;

    struct Node **subgraphRoots = (struct Node**)malloc(n * sizeof(struct Node*));
    if (subgraphRoots == NULL) {
        // Handle memory allocation error
        return NULL;
    }

    dfs(root, &targetSize, &currentSize, &subgraphId, subgraphRoots);

    // Handle case where the last subgraph is incomplete
    // if (currentSize > 0 && subgraphId < n - 1) {
    //     subgraphId++;
    // }

    return subgraphRoots;
}




// Add your function to reset 'visited' flag and other helper functions as needed


// struct Node** roots = splitTree(rootNode, numberOfSubgraphs);
// // Use the roots array as needed
// // ...

// // Remember to free the roots array when done
// free(roots);


// // Check if starting a new subgraph
//     if(*currentSize >= (targetSize - node->edgeCount)){
//         (*subgraphId)++;
//         node->isSubgraphRoot = 1;
//         subgraphRoots[*subgraphId] = node;
//         *currentSize = 1;
//     }
//     else{
//         if (*currentSize == 0) {
//             node->isSubgraphRoot = 1;
//             subgraphRoots[*subgraphId] = node;
//         } else {
//             node->isSubgraphRoot = 0;
//         }
        
//         node->subgraph = *subgraphId;
//         *currentSize += 1;

//         // If current subgraph has reached target size, prepare for next subgraph
//         if (*currentSize == targetSize) {
//             printf("Subgraph %d: %d\n", *subgraphId, *currentSize);
//             *currentSize = 0;
//             (*subgraphId)++;
//         }
//     }



//     // Continue DFS
//     for (int i = 0; i < node->edgeCount; ++i) {
//         dfs(node->edges[i]->node, targetSize, currentSize, subgraphId, subgraphRoots);
//     }