#include "Map.h"
#include <stdlib.h>

void initializeFloatMap(FloatMap *map, int capacity) {
    map->items = (FloatMapItem *)malloc(sizeof(FloatMapItem) * capacity);
    map->capacity = capacity;
    map->size = 0;
}

void setFloatMapValue(FloatMap *map, struct Node *key, float value) {
    // Simple linear search. Inefficient for large maps
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
    // Handle capacity limit (e.g., resize array)
}

float getFloatMapValue(FloatMap *map, struct Node *key) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i].key == key) {
            return map->items[i].value;
        }
    }
    return -1; // Return a default value if not found
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
    // Handle capacity limit as needed
}

struct Node *getNodeMapValue(NodeMap *map, struct Node *key) {
    for (int i = 0; i < map->size; i++) {
        if (map->items[i].key == key) {
            return map->items[i].value;
        }
    }
    return NULL; // Return NULL if not found
}
