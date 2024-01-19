#ifndef MAP_H
#define MAP_H

#include "Node.h"

typedef struct {
    struct Node *key;
    float value;
} FloatMapItem;

typedef struct {
    struct Node *key;
    struct Node *value;
} NodeMapItem;

typedef struct {
    FloatMapItem *items;
    int size;
    int capacity;
} FloatMap;

typedef struct {
    NodeMapItem *items;
    int size;
    int capacity;
} NodeMap;

void initializeFloatMap(FloatMap *map, int capacity);
void setFloatMapValue(FloatMap *map, struct Node *key, float value);
float getFloatMapValue(FloatMap *map, struct Node *key);

void initializeNodeMap(NodeMap *map, int capacity);
void setNodeMapValue(NodeMap *map, struct Node *key, struct Node *value);
struct Node *getNodeMapValue(NodeMap *map, struct Node *key);

#endif // MAP_H
