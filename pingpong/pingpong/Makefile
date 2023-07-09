# ARCHER2
#CC=cc

# Cirrus
CC=mpicc -cc=icc

#
# On Cirrus, you must also load these modules:
#
#   module load mpt
#   module load intel-compilers-18

all: pingpong

pingpong: pingpong.c location.c
	$(CC) -o $@ pingpong.c location.c
