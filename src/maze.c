#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Maze.h"

// Directions: 0 = up, 1 = right, 2 = down, 3 = left
int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

void printMaze(int **maze, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%c ", maze[i][j] ? '#' : ' ');
        }
        printf("\n");
    }
}

bool isValid(int x, int y, int n) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

void generateMaze(int **maze, int x, int y, int n) {
    maze[x][y] = 1; // Mark the current cell as visited

    // Randomly shuffle the directions
    for (int i = 0; i < 4; i++) {
        int randDir = rand() % 4;
        int newX = x + directions[randDir][0] * 2;
        int newY = y + directions[randDir][1] * 2;

        if (isValid(newX, newY, n) && maze[newX][newY] == 0) {
            // Carve a passage
            maze[x + directions[randDir][0]][y + directions[randDir][1]] = 1;
            generateMaze(maze, newX, newY, n);
        }
    }
}

int** createMaze(int n) {
    int** maze = (int **)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        maze[i] = (int *)malloc(n * sizeof(int));
    }

    // Initialize maze with walls
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            maze[i][j] = 0;
        }
    }

    // Set entrance and exit points
    maze[0][0] = 1; // Entrance
    maze[n - 1][n - 1] = 1; // Exit

    // Generate maze starting from the entrance
    generateMaze(maze, 0, 0, n);

    // Print the generated maze
    printMaze(maze, n);

    return maze;
}
