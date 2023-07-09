# ARCHER2
#CC=cc

# Cirrus
#CC=mpicc -cc=icc

# Bridges2
CC=mpicc

#
# On Cirrus, you must also load these modules:
#
#   module load mpt
#   module load intel-compilers-18

#
# On Bridges2, you must also load these modules:
#
#   module load openmpi

all: pingpong

pingpong: pingpong.c location.c
	$(CC) -o $@ pingpong.c location.c
