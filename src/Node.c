#include "Node.h"
#include "Map.h"
#include "PriorityQueue.h"
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


struct Node* getNeigbours(int ** maze, int x,int y, int * exit, int n, int parentDirection, int * id, struct Node* exitNode);

float calculateEucledianDistance(int x, int y, int * exit) {
    int dx = exit[1] - x;
    int dy = exit[0] - y;
    return sqrt((dx * dx) + (dy * dy));
}

struct Node* mazeToGraph(int **maze, int n, struct Node* exitNode) {
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
    int id = 0;
    struct Node* entry = getNeigbours(maze, 0, entryY, (int *)&exit, n, 2, &id, exitNode);
    return entry;
}

int isValidCoordinate(int x, int y, int n) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

int inverseDirection(int direction){
    if(direction == UP){
        return DOWN;
    }else if(direction == DOWN){
        return UP;
    }else if(direction == LEFT){
        return RIGHT;
    }else if(direction == RIGHT){
        return LEFT;
    }
    return -1;
}

struct Node* getNeigbours(int ** maze, int x, int y, int * exit, int n, int parentDirection, int * id, struct Node* exitNode) {
    
    // Allocate memory for a new node and initialize its properties
    struct Node * thisNode = (struct Node*)malloc(sizeof(struct Node));
    thisNode->metric = calculateEucledianDistance(x, y, exit); // Calculate the Euclidean distance to the exit
    //thisNode->subgraph = -1; // Initialize subgraph identifier
    thisNode->x = x; // Set the node's x-coordinate
    thisNode->y = y; // Set the node's y-coordinate
    thisNode->id = *id; // Set the node's id
    ++(*id); // Increment the id counter

    // Check if the current position is the exit
    if(exit[0] == y && exit[1] == x){
        thisNode->edgeCount = 0; // If it's the exit, there are no outgoing edges

        exitNode = thisNode; // Set the exit node

        return thisNode;
    }

    // Count the number of valid neighbors
    int neighboursCount = 0;
    bool neighDirection[4] = {false, false, false, false}; // Direction flags for each neighbor
    for (int i = 0; i < 4; i++) {
        if(i == parentDirection){
            continue; // Skip the parent direction to prevent backtracking
        }
        int newX = x + neighboursHelper[i][0]; // Calculate new x-coordinate for the neighbor
        int newY = y + neighboursHelper[i][1]; // Calculate new y-coordinate for the neighbor
        if (isValidCoordinate(newX, newY, n) && maze[newY][newX] == 0) { // Check if the neighbor is within bounds and passable
            neighDirection[i] = true; // Mark direction as valid
            neighboursCount++; // Increment valid neighbors count
        }
    }
    // printf("Node: %d, %d: Neighbours: %d\n", x, y, neighboursCount);

    // Allocate memory for edges to each valid neighbor
    thisNode->edgeCount = neighboursCount;
    thisNode->edges = (struct Edge**)malloc(sizeof(struct Edge*) * thisNode->edgeCount);

    // Iterate through each direction to establish edges to neighbors
    int edgeIndex = 0;
    for(int direction = 0; direction < 4; direction++){
        if(!neighDirection[direction]){
            continue; // Skip if the direction is not valid
        }
        int weight = 0; // Initialize the weight of the edge
        int newX = x; // Start from the current x-coordinate
        int newY = y; // Start from the current y-coordinate
        
        // Follow the path in the current direction until a turn or intersection
        int newXTemp, newYTemp;
        int otherNeighbours = 0;
        do {
            newXTemp = newX + neighboursHelper[direction][0]; // Calculate next x-coordinate
            newYTemp = newY + neighboursHelper[direction][1]; // Calculate next y-coordinate
            if (isValidCoordinate(newXTemp, newYTemp, n) && maze[newYTemp][newXTemp] == 0) { // Check if next position is valid and passable
                //Check if new coordinate has neighbors in other directions
                weight++; // Increment the weight

                newX = newXTemp; // Update x-coordinate
                newY = newYTemp; // Update y-coordinate

                // Count the number of valid neighbors
                for (int i = 0; i < 4; i++) {
                    if(i == direction || i == inverseDirection(direction)){
                        continue; // Skip the parent direction to prevent backtracking
                    }
                    int newX = newXTemp + neighboursHelper[i][0]; // Calculate new x-coordinate for the neighbor
                    int newY = newYTemp + neighboursHelper[i][1]; // Calculate new y-coordinate for the neighbor
                    if (isValidCoordinate(newX, newY, n) && maze[newY][newX] == 0) { // Check if the neighbor is within bounds and passable
                        otherNeighbours++; // Increment valid neighbors count
                    }
                }
                // printf("Path: %d, %d: Neighbours: %d\n", newX, newY, otherNeighbours);

            }
        } while(isValidCoordinate(newXTemp, newYTemp, n) && maze[newYTemp][newXTemp] == 0 && otherNeighbours == 0); // Continue until a non-passable cell is reached

        // Determine the opposite direction to prevent backtracking in the recursive call
        int newParentDirection = inverseDirection(direction);

        // Recursively explore the neighbor node
        struct Node* neighbour = getNeigbours(maze, newX, newY, exit, n, newParentDirection, id, exitNode);

        // Create an edge to the neighbor and assign its weight and destination node
        thisNode->edges[edgeIndex] = (struct Edge*)malloc(sizeof(struct Edge));
        thisNode->edges[edgeIndex]->weight = weight;
        thisNode->edges[edgeIndex]->node = neighbour;
        edgeIndex++; // Move to the next edge index
    }

    return thisNode; // Return the constructed node with its edges
}


void printNode(struct Node *node)
{
    printf("Node %d (%d,%d) - Metric: %.2f\n",node->id,node->x,node->y, node->metric);
    for(int i = 0; i < node->edgeCount; i++){
        printf("\tEdge %d - Weight: %d, Node %d: (%d,%d)\n", i, node->edges[i]->weight, node->edges[i]->node->id,node->edges[i]->node->x, node->edges[i]->node->y);
    }
    for(int i = 0; i < node->edgeCount; i++){
        printNode(node->edges[i]->node);
    }
}

int totalNodes(struct Node *root) {
    int count = 1; // Count this node
    for (int i = 0; i < root->edgeCount; ++i) {
        count += totalNodes(root->edges[i]->node);
    }
    return count;
}

void buildAdjacencyMatrix(struct Node * node, int ** matrix){
    for(int edgeIndex = 0; edgeIndex < node->edgeCount; edgeIndex++){
        matrix[node->id][node->edges[edgeIndex]->node->id] = node->edges[edgeIndex]->weight;
        matrix[node->edges[edgeIndex]->node->id][node->id] = node->edges[edgeIndex]->weight;
        buildAdjacencyMatrix(node->edges[edgeIndex]->node, matrix);
    }
}

// struct Node** reconstructPath(NodeMap *parentMap, struct Node *current, int pathCapacity, int *pathLength) {
//     struct Node** path = (struct Node**)malloc(sizeof(struct Node*) * pathCapacity);
//     *pathLength = 0;

//     while (current != NULL) {
//         if (*pathLength >= pathCapacity) {
//             pathCapacity *= 2;
//             path = (struct Node**)realloc(path, sizeof(struct Node*) * pathCapacity);
//         }
//         path[(*pathLength)++] = current;
//         current = getNodeMapValue(parentMap, current);
//     }

//     for (int i = 0; i < *pathLength / 2; i++) {
//         struct Node* temp = path[i];
//         path[i] = path[*pathLength - 1 - i];
//         path[*pathLength - 1 - i] = temp;
//     }

//     return path;
// }


struct Node** reconstructPath(NodeMap *parentMap, struct Node *current, int pathCapacity, int *pathLength) {
    *pathLength = 0; // Ensure pathLength is initialized to 0
    struct Node** path = (struct Node**)malloc(sizeof(struct Node*) * pathCapacity);

    // Backtrack from goal to start
    while (current != NULL) {
        if (*pathLength >= pathCapacity) {
            // Resize if necessary
            pathCapacity *= 2;
            path = (struct Node**)realloc(path, sizeof(struct Node*) * pathCapacity);
        }
        path[*pathLength] = current;
        (*pathLength)++; // Increment the path length
        // printf("Node %d: (%d, %d)\n", current->id, current->x, current->y);
        current = getNodeMapValue(parentMap, current); // Move to the parent of the current node
        // printf("Node %d: (%d, %d)\n", current->id, current->x, current->y);
    }

    // Reverse the path
    for (int i = 0; i < *pathLength / 2; i++) {
        struct Node* temp = path[i];
        path[i] = path[*pathLength - 1 - i];
        path[*pathLength - 1 - i] = temp;
    }

    return path;
}



struct Node** a_star(struct Node *start, struct Node *goal, int size, int *pathLength) {
    struct PriorityQueue openSet;
    initializePriorityQueue(&openSet, size); 
    insertWithPriority(&openSet, start, start->metric);
    printf("Start: %d, %d\n", start->x, start->y);
    printf("Metric: %.2f\n", start->metric);

    FloatMap gScore;
    initializeFloatMap(&gScore, size);
    setFloatMapValue(&gScore, start, 0);

    NodeMap parentMap;
    initializeNodeMap(&parentMap, size);

    while (!isEmpty(&openSet)) {
        struct Node *current = popLowest(&openSet);
        if (current->metric == 0) {
            struct Node** path = reconstructPath(&parentMap, current, size, pathLength);
            free(openSet.elements);
            return path;
        }

        for (int i = 0; i < current->edgeCount; i++) {
            struct Node *neighbor = current->edges[i]->node;
            float tentative_gScore = getFloatMapValue(&gScore, current) + current->edges[i]->weight;

            if (tentative_gScore < getFloatMapValue(&gScore, neighbor)) {
                setNodeMapValue(&parentMap, neighbor, current);
                setFloatMapValue(&gScore, neighbor, tentative_gScore);
                float fScore = tentative_gScore + neighbor->metric;

                if (!containsInPriorityQueue(&openSet, neighbor)) {
                    insertWithPriority(&openSet, neighbor, fScore);
                }
            }
        }
    }

    free(openSet.elements);
    return NULL; // No path found
}

void printPath(struct Node** path, int pathLength) {
    if (path == NULL || pathLength <= 0) {
        printf("No path found or path is empty.\n");
        return;
    }

    printf("Path (length = %d):\n", pathLength);
    for (int i = 0; i < pathLength; i++) {
        if (path[i] != NULL) {
            // Assuming each node has 'x', 'y', and 'id' attributes
            printf("Node %d: (%d, %d)\n", path[i]->id, path[i]->x, path[i]->y);
        } else {
            printf("NULL Node encountered in path.\n");
        }
    }
}



/*
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
*/



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