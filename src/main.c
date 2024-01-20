#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "../include/FloodMax.h"
#include "../include/Maze.h"
#include "../include/Node.h"
#define STANDARD_N 10


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
    
    if(rank == leader){
        printf("======================= Leader Election ===============================\n");
        printf("I am rank %d and I am the leader\n", rank);
        // B) Server - Randomly Generate a graph (maze with one exit- point and one entrance at the borders of the maze) of size n by n ( n should be large )

        printf("\n======================= Maze Generation ===============================\n");
        maze = createMaze(n);

        printMaze(maze, n);

        struct Node *exitNode = malloc(sizeof(struct Node));
        struct Node *mazeGraph = mazeToGraph(maze, n, exitNode);

        printf("\n==================== Graph Representation ============================\n");

        printNode(mazeGraph);

        // C) Server - Split the graph into (about) equal size subgraphs and each subgraph contains entry & exit points (try to be efficient here, use the MST algorithm)
        
        // our graph already is a MST as we generated our maze without cycles

        totalNodesCount = totalNodes(mazeGraph);
        matrix = (int**) malloc(totalNodesCount * sizeof(int *));
        for(int i = 0; i < totalNodesCount; i++){
            matrix[i] = (int*) calloc(totalNodesCount, sizeof(int));
        }

        buildAdjacencyMatrix(mazeGraph, matrix);

        // D) Distribute subgraphs to different processors

        // This didn't work out for us, so we run the algorithm on the entire maze on the leader

        int pathLength;
        struct Node** path = a_star(mazeGraph, exitNode, totalNodesCount, &pathLength); 

        printf("\n======================= A Star Path ===============================\n");

        printf("Path found by a star: \n");

        printPath(path, pathLength);


        printf("\n======================= Maze with A*  =============================\n");

        // mark the path in the maze
        for (int i = 0; i < pathLength; i++)
        {
            // from the node to the next node mark the other fileds as 2, 3, 4, 5 in order to find if its upside down or not
            if (i < pathLength - 1)
            {
                if (path[i]->x < path[i + 1]->x)
                {
                    for (int j = path[i]->x; j < path[i + 1]->x; j++)
                    {
                        maze[path[i]->y][j] = 2;
                    }
                }
                else if (path[i]->x > path[i + 1]->x)
                {
                    for (int j = path[i]->x; j > path[i + 1]->x; j--)
                    {
                        maze[path[i]->y][j] = 3;
                    }
                }
                else if (path[i]->y < path[i + 1]->y)
                {
                    for (int j = path[i]->y; j < path[i + 1]->y; j++)
                    {
                        maze[j][path[i]->x] = 4;
                    }
                }
                else if (path[i]->y > path[i + 1]->y)
                {
                    for (int j = path[i]->y; j > path[i + 1]->y; j--)
                    {
                        maze[j][path[i]->x] = 5;
                    }
                }
            }

            // also mark the last node
            maze[path[i]->y][path[i]->x] = 2;
        }

        printMaze(maze, n);
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
