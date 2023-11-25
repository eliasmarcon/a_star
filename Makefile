EXECS=mpi_distributed_bfs
MPICC?=mpicc.mpich

# Define the output directory
OUTDIR=out

# Include directory for header files
INCLUDEDIR=include

# Source files
SRC=$(wildcard src/*.c)

# Create the output directory if it doesn't exist
$(OUTDIR):
	mkdir -p $(OUTDIR)

all: ${EXECS}

a_star: $(OUTDIR) $(SRC)
	${MPICC} -I$(INCLUDEDIR) -o $(OUTDIR)/$@ $(SRC)

clean:
	rm -f $(OUTDIR)/*
