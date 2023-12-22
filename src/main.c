#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "../include/FloodMax.h"
#include "../include/Maze.h"
#include "../include/Node.h"
#define STANDARD_N 10

void printMatrix(int** matrix, int size){
    
    printf("Matrix:\n");
    printf("         | ");
    int dashes = 11;
    for(int i = 0; i < size; i++){
        printf("%3d ", i);
        dashes += 4;	
    }
    printf("\n");
    for(int i = 0; i < dashes; i++) {
        printf("-");
    }
    printf("\n");
    for(int i = 0; i < size; i++){
        printf("Row %3d: | ", i);
        for(int j = 0; j < size; j++){
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
    if(rank == leader){
        printf("I am rank %d and I am the leader\n", rank);
        printf("Leader is %d\n", leader);
        // B) Server - Randomly Generate a graph (maze with one exit- point and one entrance at the borders of the maze) of size n by n ( n should be large )

        maze = createMaze(n);
        struct Node *mazeGraph = mazeToGraph(maze, n);
        printNode(mazeGraph);

        // C) Server - Split the graph into (about) equal size subgraphs and each subgraph contains entry & exit points (try to be efficient here, use the MST algorithm)
        
        // our graph already is a MST as we generated our maze without cycles


        // Split MST into subgraphs
        /*
        struct Node** subgraphs =  splitTree(mazeGraph, size);
        int nodes = 0;
        for (int i = 0; i < size; i++) {
            printf("Subgraph %d:\n", i);
            nodes += totalSubgraphNodes(subgraphs[i],i);
            printSubgraph(subgraphs[i],i);
        }
        int totalNodesCount = totalNodes(mazeGraph);
        printf("Total nodes: %d, Subgraph nodes: %d\n", totalNodesCount, nodes);
        if(nodes != totalNodesCount){
            printf("Error: Subgraphs do not contain all nodes\n");
            return 1;
        }
        */
        int totalNodesCount = totalNodes(mazeGraph);
        int ** matrix = (int**) malloc(totalNodesCount * sizeof(int *));
        for(int i = 0; i < totalNodesCount; i++){
            matrix[i] = (int*) calloc(totalNodesCount, sizeof(int));
        }
        buildAdjacencyMatrix(mazeGraph, matrix);
        
        // print matrix
        printMatrix(matrix, totalNodesCount);


        // D) Distribute subgraphs to different processors

        //free(subgraphs);
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
        free(maze);
    }

    return 0;
}
