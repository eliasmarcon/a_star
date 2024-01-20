#include "Map.h"
#include <stdlib.h>

#define FLT_MAX 3.402823466e+38F /* max value */

void initializeFloatMap(FloatMap *map, int capacity) {
    map->items = (FloatMapItem *)malloc(sizeof(FloatMapItem) * capacity);
    map->capacity = capacity;
    map->size = 0;
}

void setFloatMapValue(FloatMap *map, struct Node *key, float value) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i].key == key) {
            map->items[i].value = value;
            return;
        }
    }

    // Add new item if key not found
    if (map->size < map->capacity) {
        map->items[map->size++] = (FloatMapItem){key, value};
    }
}

float getFloatMapValue(FloatMap *map, struct Node *key) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i].key == key) {
            return map->items[i].value;
        }
    }
    return FLT_MAX;
}

void initializeNodeMap(NodeMap *map, int capacity) {
    map->items = (NodeMapItem *)malloc(sizeof(NodeMapItem) * capacity);
    map->capacity = capacity;
    map->size = 0;
}

void setNodeMapValue(NodeMap *map, struct Node *key, struct Node *value) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i].key == key) {
            map->items[i].value = value;
            return;
        }
    }

    if (map->size < map->capacity) {
        map->items[map->size++] = (NodeMapItem){key, value};
    }
}

struct Node *getNodeMapValue(NodeMap *map, struct Node *key) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i].key == key) {
            return map->items[i].value;
        }
    }
    
    return NULL;
}
