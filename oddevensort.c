#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define TAM 16

int A[TAM];

void llenar()
{
	srand(time(NULL));
	for (int i = 0; i < TAM; ++i)
		A[i]=rand()%100;
}

void print()
{
	for (int i = 0; i < TAM; ++i)
		printf("%d ", A[i]);
	printf("\n");
}

void qs(int A[],int izq, int der )
{ 
	int i, j, pivot , tmp; 
	i = izq; 
	j = der; 
	pivot = A[(izq+der)/2]; 
    do{ 
        while( (A[i] < pivot) && (j <= der) )
            i++;
        while( (pivot < A[j]) && (j > izq) )
            j--;
        if( i <= j )
        { 
            tmp = A[i]; 
            A[i] = A[j]; 
            A[j] = tmp; 
            i++;  
            j--; 
        }
    }while( i <= j ); 

   
    if( izq < j ) 
        qs( A, izq, j );
        
    if( i < der ) 
        qs( A, i, der ); 
}

void quicksort(int list[],int n)
{
    qs(list,0,n-1);
}



/*void Odd_even_sort( int a[],int n) 
{
	int phase, i, temp;
	for (phase = 0; phase < n; phase++)
		if (phase % 2 == 0) 
		{ // Even phase 
			for (i = 1; i < n; i += 2)
				if (a[i − 1] > a[i]) 
				{
					temp = a[i];
					a[i] = a[i − 1];
					a[i − 1] = temp;
				}
		} 
		else 
		{ // Odd phase 
			for (i = 1; i < n − 1; i += 2)
				if (a[i] > a[i+1]) 
				{
					temp = a[i];
					a[i] = a[i+1];
					a[i+1] = temp;
				}
		}
}*/


int main(int argc, char const *argv[])
{
	int comm_sz; 
	int	my_rank;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (my_rank==0)
	{
		llenar();
		print();
	}

	int ini = my_rank*TAM/comm_sz;
    int end = (my_rank+1)*TAM/comm_sz;
    int pre = (ini+TAM-(TAM/comm_sz))%TAM;

    printf("Proceso %d tiene pre=%d , ini=%d , end=%d\n", my_rank,pre,ini,end);

    MPI_Scatter (A, TAM/comm_sz, MPI_INT, &A[ini], TAM/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
    qs(A,ini,end-1);
    MPI_Gather(&A[ini], TAM/comm_sz, MPI_INT, A, TAM/comm_sz, MPI_INT,0, MPI_COMM_WORLD);

    /*for (int phase = 0; phase < comm_sz; ++phase)
    {
    	MPI_Scatter (A, TAM/comm_sz, MPI_INT, &A[ini], TAM/comm_sz, MPI_INT, 0, MPI_COMM_WORLD);
    	if(phase%2==0)
    	{
    		if(my_rank%2!=0)
    		{
    			MPI_Send(&A[ini],TAM/comm_sz,MPI_INT,(my_rank+1)%comm_sz,0,MPI_COMM_WORLD);

    			MPI_Recv(&A[ini],TAM/comm_sz,MPI_INT,(my_rank+1)%comm_sz,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    		}
    		else
    		{
    			MPI_Recv(&A[pre],TAM/comm_sz,MPI_INT,(my_rank+comm_sz-1)%comm_sz,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    			qs(A,pre,end-1);
    			MPI_Send(&A[pre],TAM/comm_sz,MPI_INT,(my_rank+comm_sz-1)%comm_sz,0,MPI_COMM_WORLD);
    		}
    	}
    	else
    	{
    		if(my_rank%2!=0)
    		{
    			MPI_Recv(&A[pre],TAM/comm_sz,MPI_INT,(my_rank+comm_sz-1)%comm_sz,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				qs(A,pre,end-1);
				MPI_Send(&A[pre],TAM/comm_sz,MPI_INT,(my_rank+comm_sz-1)%comm_sz,0,MPI_COMM_WORLD);
    		}
			else
			{
				MPI_Send(&A[ini],TAM/comm_sz,MPI_INT,(my_rank+1)%comm_sz,0,MPI_COMM_WORLD);

				MPI_Recv(&A[ini],TAM/comm_sz,MPI_INT,(my_rank+1)%comm_sz,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			}
    	}
    	MPI_Gather(&A[ini], TAM/comm_sz, MPI_INT, A, TAM/comm_sz, MPI_INT,0, MPI_COMM_WORLD);
    }*/

    if (my_rank==0)
    	print();

    MPI_Finalize();

	return 0;
}