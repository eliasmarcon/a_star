EXECS=mpi_a_star
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

mpi_a_star: $(OUTDIR) $(SRC)
	${MPICC} -I$(INCLUDEDIR) -o $(OUTDIR)/$@ $(SRC) -lm -Ofast

clean:
	rm -f $(OUTDIR)/*
