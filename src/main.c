#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "../include/FloodMax.h"
#include "../include/Maze.h"
#include "../include/Node.h"
#define STANDARD_N 10

void printMatrix(int** matrix, int rows, int cols){
    
    printf("Matrix:\n");
    printf("         | ");
    int dashes = 11;
    for(int i = 0; i < cols; i++){
        printf("%3d ", i);
        dashes += 4;	
    }
    printf("\n");
    for(int i = 0; i < dashes; i++) {
        printf("-");
    }
    printf("\n");
    for(int i = 0; i < rows; i++){
        printf("Row %3d: | ", i);
        for(int j = 0; j < cols; j++){
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank and size of the MPI communicator
    int rank, size, n;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (argc > 1)
    {
        n = atoi(argv[1]);
        if (rank == 0)
            printf("Rank: %d, Setting n to %d\n", rank, n);
    }
    else
    {
        n = STANDARD_N;
    }

    
    // A) Leader Election
    int diameter = 3; // full mesh, but retry
    // Server == leader
    volatile int leader = rank;
    int count = electLeader(rank, size, diameter, &leader);

    int sumCalls = 0;
    MPI_Reduce(&count, &sumCalls, 1, MPI_INT, MPI_SUM, leader, MPI_COMM_WORLD);
    int** maze;
    int rows = 0;
    int totalNodesCount;
    int batchSize;
    int ** matrix;
    //printf("Leader is %d\n", leader);
    if(rank == leader){
        printf("I am rank %d and I am the leader\n", rank);
        // B) Server - Randomly Generate a graph (maze with one exit- point and one entrance at the borders of the maze) of size n by n ( n should be large )

        maze = createMaze(n);
        struct Node *exitNode = malloc(sizeof(struct Node));
        struct Node *mazeGraph = mazeToGraph(maze, n, exitNode);
        // printNode(mazeGraph);

        // C) Server - Split the graph into (about) equal size subgraphs and each subgraph contains entry & exit points (try to be efficient here, use the MST algorithm)
        
        // our graph already is a MST as we generated our maze without cycles

        // create adjacency matrix
        totalNodesCount = totalNodes(mazeGraph);
        // matrix = (int**) malloc(totalNodesCount * sizeof(int *));
        // for(int i = 0; i < totalNodesCount; i++){
        //     matrix[i] = (int*) calloc(totalNodesCount, sizeof(int));
        // }
        // buildAdjacencyMatrix(mazeGraph, matrix);
        
        // print matrix
        // printMatrix(matrix, totalNodesCount, totalNodesCount);



        // D) Distribute subgraphs to different processors
        // batchSize = (int)(totalNodesCount / size);
        // calculate how many rows the leader has to do
        // rows = (totalNodesCount % size) + batchSize;

        // free(subgraphs);

        int pathLength;
        struct Node** path = a_star(mazeGraph, exitNode, totalNodesCount, &pathLength); // '5' is the size of your graph
        printPath(path, pathLength);
    }


    // E) Each subgraph constructs the path from its entry points to its exit points


    // F) Each processor passes its shortest paths to the server


    // G) Server creates an updated version of the graph with the received shortest paths calculates the overall shortest path from it.



    // Finalize MPI
    MPI_Finalize();
    if(rank == leader){
        for (int i = 0; i < size; i++) {
            free(maze[i]);
        }
        // for(int i = 0; i < totalNodesCount; i++){
        //     free(matrix[i]);
        // }
        // free(matrix);
        free(maze);
    }

    return 0;
}
