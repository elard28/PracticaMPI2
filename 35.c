#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{
	int comm_sz; 
	int	my_rank;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	return 0;
}