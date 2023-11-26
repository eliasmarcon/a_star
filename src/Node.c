#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <math.h>

#define UP 

/**
 * @brief Prints the children of a given node.
 *
 * @param node Pointer to the node whose children are to be printed.
 */
void printChildren(struct Node *node, int rank)
{
    if (node->childrenCount > 0)
    {
        struct Node **children = (node->children);
        for (int i = 0; i < node->childrenCount; i++)
        {
            struct Node *child = children[i];
            printChild(child, i);
        }
        //   printf("Rank: %d, %d\n", children->rank);
    }
}

/**
 * Prints the ith child of the given node.
 *
 * @param node The node whose child will be printed.
 * @param i The index of the child to be printed.
 */
void printChild(struct Node *node, int i)
{
    printf("\t %d. Child: %d\n", i, node->rank);
}

/**
 * Converts a Node struct to a string representation.
 *
 * @param node The Node struct to be converted.
 * @return A string representation of the Node struct.
 */
char *NodetoString(const struct Node *node)
{
    return convertToJson(node);
}

cJSON *nodeToJson(const struct Node *node);

/**
 * @brief Converts a Node struct to a JSON string.
 *
 * @param node The Node struct to be converted.
 * @return char* The JSON string representation of the Node struct.
 */
char *convertToJson(const struct Node *node)
{
    cJSON *jsonNode = nodeToJson(node);
    char *jsonString = cJSON_Print(jsonNode);
    cJSON_Delete(jsonNode);
    return jsonString;
}

/**
 * Converts a Node struct to a JSON object.
 *
 * @param node The Node struct to be converted.
 * @return A cJSON object representing the Node struct.
 */
cJSON *nodeToJson(const struct Node *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    cJSON *jsonNode = cJSON_CreateObject();
    cJSON_AddNumberToObject(jsonNode, "rank", node->rank);
    cJSON_AddNumberToObject(jsonNode, "depth", node->depth);
    cJSON_AddNumberToObject(jsonNode, "parent", node->parent);
    cJSON_AddNumberToObject(jsonNode, "childrenCount", node->childrenCount);

    if (node->childrenCount > 0 && node->children != NULL)
    {
        cJSON *childrenArray = cJSON_CreateArray();
        for (int i = 0; i < node->childrenCount; ++i)
        {
            cJSON_AddItemToArray(childrenArray, nodeToJson(node->children[i]));
        }
        cJSON_AddItemToObject(jsonNode, "children", childrenArray);
    }

    return jsonNode;
}

struct Node *jsonToNode(const cJSON *jsonNode);

/**
 * Converts a JSON string to a Node struct.
 *
 * @param jsonString The JSON string to convert.
 * @return A pointer to the converted Node struct.
 */
struct Node *convertFromJson(const char *jsonString)
{
    cJSON *jsonNode = cJSON_Parse(jsonString);
    struct Node *node = jsonToNode(jsonNode);
    cJSON_Delete(jsonNode);
    return node;
}

/**
 * @briyef Converts a JSON object to a Node struct.
 *
 * @param jsonNode The JSON object to convert.
 * @return struct Node* The Node struct created from the JSON object.
 */
struct Node *jsonToNode(const cJSON *jsonNode)
{
    if (jsonNode == NULL)
        return NULL;

    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL)
        return NULL;

    node->rank = cJSON_GetObjectItemCaseSensitive(jsonNode, "rank")->valueint;
    node->depth = cJSON_GetObjectItemCaseSensitive(jsonNode, "depth")->valueint;
    node->parent = cJSON_GetObjectItemCaseSensitive(jsonNode, "parent")->valueint;
    node->childrenCount = cJSON_GetObjectItemCaseSensitive(jsonNode, "childrenCount")->valueint;

    if (node->childrenCount > 0)
    {
        node->children = malloc(sizeof(struct Node *) * node->childrenCount);
        if (node->children == NULL)
        {
            free(node);
            return NULL;
        }

        cJSON *childrenArray = cJSON_GetObjectItemCaseSensitive(jsonNode, "children");
        cJSON *child;
        int i = 0;
        cJSON_ArrayForEach(child, childrenArray)
        {
            node->children[i++] = jsonToNode(child);
        }
    }
    else
    {
        node->children = NULL;
    }

    return node;
}

/**
 * Prints the given node.
 *
 * @param node The node to be printed.
 * @param isLast Flag indicating whether the node is the last one in the list.
 */
void printNode(struct Node *node, int isLast)
{

    if (node == NULL)
        return;

    // Print leading space with vertical lines
    for (int i = 0; i < node->depth - 1; i++)
    {
        printf("    ");
    }

    // Print the current node
    if (node->depth > 0)
    {
        printf(isLast ? "└── " : "|── ");
    }

    printf("%d\n", node->rank);

    // Recursively print children
    if (node->childrenCount > 0)
    {
        struct Node **children = node->children;
        for (int i = 0; i < node->childrenCount; i++)
        {

            printNode(children[i], i == node->childrenCount - 1);
        }
    }
}

/**
 * Prints the full graph starting from the given node.
 *
 * @param node The starting node of the graph to be printed.
 */
void printFullGraph(struct Node *node)
{
    printNode(node, 1);
}


void addNode(Node** head, int row, int col) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->row = row;
    newNode->col = col;
    newNode->next = *head;
    *head = newNode;
}

void calculateEucledianDistance(int * point, int * exit) {
    int x = point[0] - exit[0];
    int y = point[1] - exit[1];
    return sqrt(x * x + y * y);
}

struct Node* mazeToGraph(int maze**,struct Node* graph[], int n) {
    // define entry and exit points
    int entry[2] = { 0, 0 };
    //TODO: get actual entry point
    int exit[2] = { n - 1, n - 1 };
    //TODO: get actual exit point

    struct Node* entry = getNeigbours(maze, &entry, &exit, n, 2);
    return entry;
}



struct Node* getNeigbours(int ** maze, int * point, int * exit, int n, int parentDirection) {
    int neighboursCount = 0;
    int neighboursHelper[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right
    bool neighDirection[4] = {false, false, false, false};
    for (int i = 0; i < 4; i++) {
        if(i == parentDirection){
            continue;
        }
        int newX = x + neighboursHelper[i][0];
        int newY = y + neighboursHelper[i][1];
        if (newX >= 0 && newX < n && newY >= 0 && newY < n && maze[newX][newY] == 0) {
            neighboursCount++;
            neighDirection[i] = true;
        }
    }
    struct Node * thisNode = (struct Node*)malloc(sizeof(struct Node));
    thisNode->weight = calculateEucledianDistance(point, exit);
    for(int neighbor = 0; neighbor < neighboursCount; neighbor++){
        int weight = 0;
        // get the direction of the neighbour
        // TODO: follow the path to the next crossing
        while(maze[newX][newY] == 0){
            weight++;
            newX = newX + neighboursHelper[i][0];
            newY = newY + neighboursHelper[i][1];
        }
        // TODO: add an edge with the weight of the distance to the exit and a recursive call to getNeighbours
    }

    return thisNode;

    /*int** neighbours = (int**)malloc(sizeof(int*) * (*neighboursCount));
    for (int i = 0; i < *neighboursCount; i++) {
        neighbours[i] = (int*)malloc(sizeof(int) * 2);
    }
    int index = 0;
    for (int i = 0; i < 4; i++) {
        int newX = x + neighboursHelper[i][0];
        int newY = y + neighboursHelper[i][1];
        if (newX >= 0 && newX < n && newY >= 0 && newY < n && maze[newX][newY] == 0) {
            neighbours[index][0] = newX;
            neighbours[index][1] = newY;
            index++;
        }
    }*/
    return neighbours;
}