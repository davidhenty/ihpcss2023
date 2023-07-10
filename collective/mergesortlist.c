#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define NUMLIST 3 // Number of distinct lists (of integers)
#define N 5       // Length of each list

void mergesortlist(void *vinvec, void *vinoutvec, int *n, MPI_Datatype *type);
void mergelist(int *merge, int *a, int *b, int n);

int main(void)
{
  int i, ilist;

  // local sorted list

  int mysortedlist[NUMLIST][N];

  // global sorted list

  int sortedlist[NUMLIST][N];

  MPI_Comm comm;

  MPI_Datatype MPI_LIST;
  MPI_Op MPI_MERGELIST;

  int size, rank;

  comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  // Define datatype appropriate for a single array of N integers

  MPI_Type_contiguous(N, MPI_INT, &MPI_LIST);
  MPI_Type_commit(&MPI_LIST);

  // Register new reduction operation to merge two sorted lists

  MPI_Op_create(mergesortlist, 1, &MPI_MERGELIST);

  // Generate sorted lists on each rank

  for (i=0; i < N; i++)
    {
      for (ilist=0; ilist < NUMLIST; ilist++)
	{
	  mysortedlist[ilist][i] = rank+size*(N-i-1) + 100*ilist;
	  sortedlist[ilist][i] = -1;
	}
    }

  for (i=0; i < N; i++)
   {
     printf("rank %d, mysortedlist[%d] =", rank, i);

     for (ilist=0; ilist < NUMLIST; ilist++)
       {
	 printf(" %3d", mysortedlist[ilist][i]);
       }
     printf("\n");
    }

  printf("\n");

  // Perform reduction to rank 0

  MPI_Reduce(mysortedlist, sortedlist, NUMLIST, MPI_LIST, MPI_MERGELIST,
	     0, comm);

  if (rank == 0)
    {
      for (i=0; i < N; i++)
	{
	  printf("rank %d, sortedlist[%d] =", rank, i);

	  for (ilist=0; ilist < NUMLIST; ilist++)
	    {
	      printf(" %3d", sortedlist[ilist][i]);
	    }
	  printf("\n");
	}

      printf("\n");
    }

  MPI_Finalize();

  return 0;
}


void mergesortlist(void *vinvec, void *vinoutvec, int *n, MPI_Datatype *type)
{
  MPI_Aint lb, listextent, intextent;

  int i, ilist;
  int nvec, nlist;

  int *invec    = (int *) vinvec;
  int *inoutvec = (int *) vinoutvec;

  // the count is the number of individual lists

  nlist = *n;

  // Infer length of each list from the extents
  // Should really check "type" is valid, i.e. a contiguous block of ints

  MPI_Type_get_extent(MPI_INT, &lb, &intextent);
  MPI_Type_get_extent(*type, &lb, &listextent);

  nvec = listextent/intextent;

  // Need a temporary as "mergelist" does not work in-place

  int *mergevec = (int *) malloc(nvec*sizeof(int));

  // Merge each of the "nlist" lists in turn

  for (ilist=0; ilist < nlist; ilist++)
    {
      mergelist(mergevec, &invec[ilist*nvec], &inoutvec[ilist*nvec], nvec);

      for (i=0; i < nvec; i++)
	{
	  inoutvec[ilist*nvec+i] = mergevec[i];
	}
    }

  free(mergevec);
}


void mergelist(int *merge, int *a, int *b, int n)
{
  int i, ia, ib;

  ia = 0;
  ib = 0;

  for (i=0; i < n; i++)
    {
      if (a[ia] > b[ib])
        {
          merge[i] = a[ia];
          ia++;
        }
      else
        {
          merge[i] = b[ib];
          ib++;
        }
    }
}
