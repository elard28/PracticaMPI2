#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

//#define TAM 1024
//#define TAM 2048
//#define TAM 4096
//#define TAM 8192
#define TAM 16384

int A[TAM][TAM], B[TAM], C[TAM];

void llenar()
{
    for (int i=0; i<TAM; i++)
    {
        B[i] = rand()%100;
        for (int j=0; j<TAM; j++)
            A[i][j] = rand()%100;
    }
}

void print()
{
    for (int i=0; i<TAM; ++i) 
    {
        for (int j=0; j<TAM; ++j)
            printf("%d\t", A[i][j]);
        printf("|\t%d\n", B[i]);
        printf("\n");
    }
    printf("\n");
}

void printRes()
{
	for (int i=0; i<TAM; ++i) 
        printf("%d\n\n", C[i]);
    printf("\n");
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

	double tInicio;
	double tFin;

    int comm_sz; 
    int	my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank==0)
    {
        llenar();
        //print();
    }

	int ini = my_rank*TAM/comm_sz;
    int end = (my_rank+1)*TAM/comm_sz;

    MPI_Bcast (B, TAM, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter (A, TAM*TAM/comm_sz, MPI_INT, A[ini], TAM*TAM/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    tInicio = MPI_Wtime();
    for (int i = ini; i < end; ++i)
    { 
        for (int j = 0; j < TAM; ++j) 
        	C[i]+=A[i][j]*B[j];
    }
    tFin = MPI_Wtime();

    MPI_Allgather(&C[ini], TAM/comm_sz, MPI_INT, C, TAM/comm_sz, MPI_INT, MPI_COMM_WORLD);

    if (my_rank==0)
    {
        //printRes();
        printf("Con %d procesos demora %6.3f milisegundos\n",comm_sz, (tFin-tInicio)*1000.0);
    }

    MPI_Finalize();

	return 0;
}