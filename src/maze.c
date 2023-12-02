#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "Maze.h"

const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right

int checkWall(int x, int y,int size);

int rightHole = 0;

// int isValidCoordinate(int x, int y, int n) {
//     return x >= 0 && x < n && y >= 0 && y < n;
// }


/**
 * Checks if there is a wall at the specified coordinates in the maze.
 * 
 * @param x The x-coordinate of the cell in the maze.
 * @param y The y-coordinate of the cell in the maze.
 * @param size The size of the maze.
 * @return 1 if there is a wall at the specified coordinates, 0 otherwise.
 */
int checkWall(int y, int x,int size){
    if(y==0 || x==0 || x==size-1 || y==size-1){
        return 1;
    }
    else if(x==size-1){
        // check if right wall has hole
        if(rightHole==0){
            rightHole = 1;
            return 0;
        }
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * Prints the maze.
 *
 * @param maze The maze to be printed.
 * @param size The size of the maze.
 */
void printMaze(int **maze, int size) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if(maze[y][x]==2){
                printf("X");
            }
            else{
                printf("%c", maze[y][x] ? '[' : ' '); // '#' for wall, ' ' for path
                printf("%c", maze[y][x] ? ']' : ' '); // '#' for wall, ' ' for path
            }
        }
        printf("\n");
    }
}

/**
 * Generates a maze using the depth-first search algorithm.
 *
 * @param maze The maze grid represented as a 2D array.
 * @param visited The visited grid represented as a 2D array.
 * @param y The current y-coordinate in the maze.
 * @param x The current x-coordinate in the maze.
 * @param size The size of the maze grid.
 */
void generateMaze(int **maze, int **visited, int y, int x, int size) {
    visited[y][x] = 1;
    maze[y][x] = 0; // 0 represents a path
        
    int dirs[4] = {0, 1, 2, 3};
    // Shuffle directions
    for (int i = 0; i < 4; i++) {
        int j = (int)(rand()) % 4;
        // Swap
        int temp;
        temp = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int dy = directions[dirs[i]][0];
        int dx = directions[dirs[i]][1];
        int newY = y + dy * 2;
        int newX = x + dx * 2;
        if (newY >= 0 && newY < size && newX >= 0 && newX < size && !visited[newY][newX] && checkWall(newY, newX, size)==0) {
            maze[y + dy][x + dx] = 0; // Break wall
            generateMaze(maze, visited, newY, newX, size);
        }
    }
}

/**
 * Ensures that the maze has an exit.
 * 
 * This function checks if the given maze has at least one exit. If the maze does not have an exit,
 * it modifies the maze to create an exit.
 * 
 * @param maze The maze represented as a 2D array of integers.
 * @param size The size of the maze (number of rows/columns).
 */
void ensureExit(int **maze, int size) {
    for (int x = size-1; x >= 0; x--) {
        for (int y = 0; y < size; y++) {
            if (!maze[y][x]) {
                for(int i = x+1; i < size; i++){
                    maze[y][i] = 0;
                    if(i==size-1){
                        return;
                    }
                }
            }
        }
    }
}

/**
 * Creates a maze of the specified size.
 *
 * @param size The size of the maze.
 * @return A pointer to a 2D array representing the maze.
 */
int** createMaze(int size) {
    srand((unsigned) time(NULL));
    int **maze = (int **)malloc(size * sizeof(int *));
    int **visited = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        maze[i] = (int *)malloc(size * sizeof(int));
        visited[i] = (int *)malloc(size * sizeof(int));
    }
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            maze[y][x] = 1; // 1 represents a wall
            visited[y][x] = 0;
        }
    }

    // Set entry and exit
    int entryY = (rand() % (size-2))+1;
    int entryX = 0; // Entry at left side

    generateMaze(maze, visited, entryY, entryX, size);

    // Ensure exit
    ensureExit(maze, size);

    for (int i = 0; i < size; i++) {
        free(visited[i]);
    }
    free(visited);

    printMaze(maze, size);
    return maze;
}
