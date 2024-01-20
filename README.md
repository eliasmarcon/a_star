# A star

Implement an a star algorithm in MPI, to traverse a maze.

## Authors

- Elias Marcon MSc. | [@eliasmarcon](https://github.com/eliasmarcon)
- Ing. Fabian Steiner BSc.| [@fasteiner](https://github.com/fasteiner/)
- Jan Langela Regincos BSc. | [@janpilu](https://github.com/janpilu)

## Folder Structure

- `./include`:

  - `FloodMax.h`: defines the data structure for the `FloodMax.c` file

  - `Map.h`: defines the data structure for the `Map.c` file

  - `Maze.h`: defines the data structure for the `Maze.c` file

  - `Node.h`: defines the data structure for the `Node.c` file

  - `PriorityQueue.h`: defines the data structure for the `PriorityQueue.c` file

- `./src`:

  - `FloodMax.c`: Leader Election Algorithm

  - `Map.c`: Simple Map Data Structure

  - `Maze.c`: Generates a Maze

  - `Node.c`: Simple Node Data Structure for defining a tree/ graph

  - `PriorityQueue.c`: Simple Priority Queue Data Structure for the A* Algorithm

- `./out`: contains the executable file `mpi_a_star`

- `Makefile`: configuration file that controls the build process of the MPI program (contains rules and commands to compile the project and create the executable file `mpi_a_star`)

- `start_a_star.sh`: automates the execution of the `make all` command and MPI program (distributed breadth-first) with a user-specified number of processes and max depth (saves the results of the run in the output file `a_star_results.txt` for monitoring and analysis)

- `a_star_results.txt`: contains the results of the most recent run

## How to build

### Prerequisites

- Ubuntu 20.04
- mpich

### Build

- the `make all` command is included in the `start_distirbuted_bfs.sh` file

### Usage

To use the provided Bash script for running the a star algorithm, follow these steps:

1. Ensure that MPI is installed on your system.

2. Open a terminal and navigate to the directory containing this bash script.

3. Run the bash script by providing the run_type (cluster, local), the number of MPI tasks and the desired max depth before switching to DFS as command-line arguments.

- Default Parameters:
  - run_type = local
  - number of tasks = 5
  - maze size = 5

```sh
./start_a_star.sh <run_type> <number_of_tasks> <max_depth> (replace `<run_type>` `<number_of_tasks>` and `<max_depth>` with the actual values)

./start_a_star.sh local 5 5

./start_a_star.sh cluster 10 8

```