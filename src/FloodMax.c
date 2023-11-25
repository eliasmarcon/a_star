#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define TIMEOUT_MS 10 // Timeout in milliseconds

/**
 * Checks if a timeout has occurred.
 *
 * @param start The starting time of the operation.
 * @return True if a timeout has occurred, false otherwise.
 */
bool check_timeout(clock_t start)
{
    clock_t current_time = clock();
    double elapsed_time_ms = 1000.0 * (current_time - start) / CLOCKS_PER_SEC;
    return elapsed_time_ms >= TIMEOUT_MS;
}

/**
 * @brief Elects a leader among a group of processes using the FloodMax algorithm.
 *
 * This function elects a leader among a group of processes by implementing the FloodMax algorithm.
 * The algorithm works by each process comparing its rank with its neighbors and passing the maximum
 * rank to its neighbors until the maximum rank reaches all processes in the group. The process with
 * the maximum rank is elected as the leader.
 *
 * @param rank The rank of the current process.
 * @param size The total number of processes in the group.
 * @param diameter The diameter of the communication network.
 * @param leader A pointer to the variable that will store the elected leader's rank.
 * @return The rank of the elected leader.
 */
int electLeader(int rank, int size, int diameter, volatile int *leader)
{
    int calls = 0;
    // send uid to all neighbours
    bool changed = true;
    int updatedBy = -1;
    MPI_Request requests[size];
    int *recData = calloc(size, sizeof(int));
    bool active_requests[size];
    int num_active_requests = 0;

    for (int i = 0; i < size; i++)
    {
        // For each node set active request to false
        active_requests[i] = false;
        if (i != rank)
        {
            // Send a nonblocking request to every node except current rank (self)
            requests[i] = MPI_REQUEST_NULL;
            MPI_Irecv(&recData[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
            // Set Active request
            active_requests[i] = true;
            num_active_requests++;
        }
    }
    for (int round = 0; round < diameter; round++)
    {
        if (changed)
        {
            int amount = 0;
            for (int i = 0; i < size; i++)
            {
                if ((i != rank && i != *leader && i != updatedBy))
                {
                    // i == neighbour rank
                    calls++;
                    MPI_Send((void *)leader, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    amount++;
                }
            }
        }
        changed = false;
        updatedBy = -1;

        // receive from all neighbours with timeout
        // start timer
        MPI_Status statuses[size];
        clock_t start_time = clock();

        // check_timeout(start_time) function checks if a certain amount of time has passed since start_time. The num_active_requests variable keeps track of how many requests are still active.
        while (!check_timeout(start_time))
        {
            // do nothing
        }

        for (int i = 0; i < size; i++)
        {
            if (active_requests[i] && i != rank)
            {
                int flag = 0;

                // MPI_Test is a non-blocking function that checks if a specific MPI request has completed
                MPI_Test(&requests[i], &flag, &statuses[i]);
                if (flag)
                {
                    if (recData[i] > *leader)
                    {
                        changed = true;
                        *leader = recData[i];
                        updatedBy = i;
                    }
                    MPI_Irecv(&recData[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);

                    active_requests[i] = false;
                    num_active_requests--;
                }
            }
        }
    }
    // Cancel any remaining active requests
    for (int i = 0; i < size; i++)
    {
        if (active_requests[i])
        {
            MPI_Cancel(&requests[i]);
            MPI_Request_free(&requests[i]);
        }
    }
    free(recData);
    return calls;
}

/*
int main(int argc, char **argv)
{
    int rank, size, diameter;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc > 1)
    {
        // Check if a command-line argument for diameter is provided
        diameter = atoi(argv[1]);
    }
    else
    {
        // If no argument is provided, set a default diameter
        diameter = 10; // Change to your desired value
    }

    volatile int leader = rank;
    clock_t time = clock();
    int count = electLeader(rank, size, diameter, &leader);
    time = clock() - time;

    int sumCalls = 0;
    MPI_Reduce(&count, &sumCalls, 1, MPI_INT, MPI_SUM, leader, MPI_COMM_WORLD);

    // print leader and time taken
    if (rank == leader)
    {
        printf("==================== Run Start ============================\n");
        printf("=============== Optimized Floodmax ========================\n");
        printf("Number of processes: %d\n", size);
        printf("Network Diameter: %d\n", diameter);
        printf("Leader is %d\n", leader);
        printf("Time taken: %f\n", ((double)time) / CLOCKS_PER_SEC);
        printf("Total Messages sent: %d\n", sumCalls);
        printf("====================  Run End  ============================\n\n");
    }

    MPI_Finalize();

    return 0;
}
*/