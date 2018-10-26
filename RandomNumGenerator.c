#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>


int main(int argc, char *argv[]){
	int rank,p;
	int n = 128;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	assert((n/p)%2 == 0);
	int *A = malloc(sizeof(int)*(n/p));	
	int *B = malloc(sizeof(int)*(n/p));	
	int i;
	for (i=1; i <= (n/p); i++){
		A[i-1] = i;
	}
	for(i=0; i < (log((n/p))/log(2)); i++){
		int j;
		#pragma omp parallel for
		for(j = 0; j < n/p;j++){
			int pw = pow(2,i);
			if (j<pw){
				B[j] = A[j];
			}else{
				B[j] = A[j] + A[j-pw];
			}
		}
		memcpy(A,B,sizeof(int)*(n/p));	
	}
	if(rank == 4){
		printf("My rank is %d\n", rank);
		for (i=0; i< (n/p); i++){
			printf("A[%d] = %d\n", i, A[i]);
		}
	}








	MPI_Finalize();
}
