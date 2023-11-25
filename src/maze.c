#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "Maze.h"

int checkWall(int x, int y,int size);

int rightHole = 0;
int leftHole = 0;
int topHole = 0;
int bottomHole = 0;

// // Directions: 0 = up, 1 = right, 2 = down, 3 = left
// int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// /**
//  * Prints the maze.
//  * 
//  * @param maze The maze to be printed.
//  * @param n The size of the maze.
//  */
// void printMaze(int **maze, int n) {
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             printf("%c ", maze[i][j] ? '#' : ' ');
//         }
//         printf("\n");
//     }
// }

// /**
//  * Checks if the given coordinates (x, y) are valid within the maze of size n.
//  *
//  * @param x The x-coordinate.
//  * @param y The y-coordinate.
//  * @param n The size of the maze.
//  * @return true if the coordinates are valid, false otherwise.
//  */
// bool isValid(int x, int y, int n) {
//     return x >= 0 && x < n && y >= 0 && y < n;
// }

// /**
//  * Generates a maze using the recursive backtracking algorithm.
//  *
//  * @param maze The 2D array representing the maze.
//  * @param x The starting x-coordinate of the maze.
//  * @param y The starting y-coordinate of the maze.
//  * @param n The size of the maze (n x n).
//  */
// void generateMaze(int **maze, int x, int y, int n) {
//     maze[x][y] = 1; // Mark the current cell as visited

//     // Randomly shuffle the directions
//     for (int i = 0; i < 4; i++) {
//         int randDir = rand() % 4;
//         int newX = x + directions[randDir][0] * 2;
//         int newY = y + directions[randDir][1] * 2;

//         if (isValid(newX, newY, n) && maze[newX][newY] == 0) {
//             // Carve a passage
//             maze[x + directions[randDir][0]][y + directions[randDir][1]] = 1;
//             generateMaze(maze, newX, newY, n);
//         }
//     }
// }

/**
 * Creates a maze of size n x n.
 *
 * @param n The size of the maze.
 * @return A pointer to a 2D array representing the maze.
 */
// int** createMaze(int n) {
//     int** maze = (int **)malloc(n * sizeof(int*));
//     for (int i = 0; i < n; i++) {
//         maze[i] = (int *)malloc(n * sizeof(int));
//     }

//     // Initialize maze with walls
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             maze[i][j] = 0;
//         }
//     }

//     // Set entrance and exit points
//     maze[0][0] = 1; // Entrance
//     maze[n - 1][n - 1] = 1; // Exit

//     // Generate maze starting from the entrance
//     generateMaze(maze, 0, 0, n);

//     // Print the generated maze
//     printMaze(maze, n);

//     return maze;
// }

int checkWall(int x, int y,int size){
    if(y==0){
        return 1;
    }
    else if(y==size-1){
        // check if right wall has hole
        if(rightHole==0){
            rightHole = 1;
            return 0;
        }
        return 1;
    }
    else if(x==0){
        return 1;
    }
    else if(x==size-1){
        return 1;
    }
    return 0;
}

/**
 * Prints the maze.
 *
 * @param maze The maze to be printed.
 * @param size The size of the maze.
 */
void printMaze(int **maze, int size) {
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if(maze[x][y]==2){
                printf("X");
            }
            else{
                printf("%c", maze[x][y] ? '#' : ' '); // '#' for wall, ' ' for path
                printf("%c", maze[x][y] ? '#' : ' '); // '#' for wall, ' ' for path
            }
        }
        printf("\n");
    }
}

void generateMaze(int **maze, int **visited, int y, int x, int size) {
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    visited[x][y] = 1;
    maze[x][y] = 0; // 0 represents a path
        

    // Shuffle directions
    for (int i = 0; i < 4; i++) {
        int j = (int)(rand()) % 4;
        printf("j:%d\n",j);

        int temp[2];
        temp[0] = dirs[i][0];
        temp[1] = dirs[i][1];
        dirs[i][0] = dirs[j][0];
        dirs[i][1] = dirs[j][1];
        dirs[j][0] = temp[0];
        dirs[j][1] = temp[1];
    }

    for (int i = 0; i < 4; i++) {
        int dx = dirs[i][0];
        int dy = dirs[i][1];
        int newY = y + dy * 2;
        int newX = x + dx * 2;
        if (newY >= 0 && newY < size && newX >= 0 && newX < size && !visited[newX][newY] && checkWall(newX, newY, size)==0) {
            maze[x + dx][y + dy] = 0; // Break wall
            generateMaze(maze, visited, newY, newX, size);
        }
    }
}

void ensureExit(int **maze, int size) {
    for (int x = size-1; x >= 0; x--) {
        for (int y = 0; y < size; y++) {
            if (!maze[y][x]) {
                for(int i = x+1; i < size; i++){
                    maze[y][i] = 0;
                    return;
                }
            }
        }
    }
}

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
            maze[x][y] = 1; // 1 represents a wall
            visited[x][y] = 0;
        }
    }

    // Set entry and exit
    int entryX = rand() % size;
    int entryY = 0; // Entry at left side

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

    // // Free memory
    // for (int i = 0; i < size; i++) {
    //     free(maze[i]);
    //     free(visited[i]);
    // }
    // free(maze);
    // free(visited);