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
        struct Node *mazeGraph = mazeToGraph(maze, n);
        printNode(mazeGraph);

        // C) Server - Split the graph into (about) equal size subgraphs and each subgraph contains entry & exit points (try to be efficient here, use the MST algorithm)
        
        // our graph already is a MST as we generated our maze without cycles

        // create adjacency matrix
        totalNodesCount = totalNodes(mazeGraph);
        matrix = (int**) malloc(totalNodesCount * sizeof(int *));
        for(int i = 0; i < totalNodesCount; i++){
            matrix[i] = (int*) calloc(totalNodesCount, sizeof(int));
        }
        buildAdjacencyMatrix(mazeGraph, matrix);
        
        // print matrix
        printMatrix(matrix, totalNodesCount, totalNodesCount);



        // D) Distribute subgraphs to different processors
        batchSize = (int)(totalNodesCount / size);
        //calculate how many rows the leader has to do
        rows = (totalNodesCount % size) + batchSize;

        //free(subgraphs);
    }
    MPI_Request *barrier = malloc(sizeof(MPI_Request));
    MPI_Ibarrier(MPI_COMM_WORLD, barrier);
    MPI_Wait(barrier, MPI_STATUS_IGNORE);
    MPI_Bcast(&batchSize, 1, MPI_INT, leader, MPI_COMM_WORLD);
    if(rank != leader){
        rows = batchSize;
    }
    //printf("Rank %d will process %d rows of the adjency matrix\n", rank, rows);
    MPI_Bcast(&totalNodesCount, 1, MPI_INT, leader, MPI_COMM_WORLD);
    int ** partMatrix = (int**) malloc(rows * sizeof(int *));
    for(int i = 0; i < rows; i++){
        partMatrix[i] = (int*) calloc(totalNodesCount, sizeof(int));
    }
    //printf("Rank %d has allocated matrix with %d rows and %d cols\n", rank, rows, totalNodesCount);
    MPI_Ibarrier(MPI_COMM_WORLD, barrier);
    MPI_Wait(barrier, MPI_STATUS_IGNORE);
    MPI_Datatype row_type;
    MPI_Type_contiguous(totalNodesCount, MPI_INT, &row_type);
    MPI_Type_commit(&row_type);
    if(rank != leader){
        // receive full matrix
        printf("Rank %d is waiting for %d rows from rank %d\n", rank, rows, leader);
        MPI_Recv(&(partMatrix[0][0]), (batchSize+1)*totalNodesCount, MPI_INT, leader, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d has received %d rows from rank %d\n", rank, rows, leader);
    }
    if(rank == leader){
        
        int otherNode = -1;
        sleep(1);
        for(int i = 0; i < size; ++i){
            if(i == leader){
                continue;
            }
            // Check if the index is within the matrix bounds
            MPI_Send(0, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&(matrix[rows + (batchSize* i)][0]), (batchSize+1)*totalNodesCount, MPI_INT, i, 0, MPI_COMM_WORLD);

            printf("Rank %d has sent %d rows to rank %d\n", rank, batchSize,i);
        }
        // set the part of the matrix that the leader has to process
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < totalNodesCount; j++){
                partMatrix[i][j] = matrix[i][j];
            }
        }
    }
    MPI_Ibarrier(MPI_COMM_WORLD, barrier);
    MPI_Wait(barrier, MPI_STATUS_IGNORE);
    MPI_Type_free(&row_type);
    printf("Rank %d has received the matrix:\n", rank);
    printMatrix(partMatrix, rows, totalNodesCount);

    //MPI_Send(&matrix[0][0], totalNodesCount * totalNodesCount, MPI_INT, i, 0, MPI_COMM_WORLD);
    


    // E) Each subgraph constructs the path from its entry points to its exit points


    // F) Each processor passes its shortest paths to the server


    // G) Server creates an updated version of the graph with the received shortest paths calculates the overall shortest path from it.



    // Finalize MPI
    MPI_Finalize();
    if(rank == leader){
        for (int i = 0; i < size; i++) {
            free(maze[i]);
        }
        for(int i = 0; i < totalNodesCount; i++){
            free(matrix[i]);
        }
        free(matrix);
        free(maze);
    }
    for(int i = 0; i < rows; i++){
        free(partMatrix[i]);
    }
    free(partMatrix);
    free(barrier);

    return 0;
}
