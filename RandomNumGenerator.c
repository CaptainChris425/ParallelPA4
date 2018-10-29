#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>
#include "matrixoperations.h"


int main(int argc, char *argv[]){
	if (argc < 4){
		printf("Useage a.out a b P\n");
		return 0;
	}
	int A = atoi(argv[1]);
	int B = atoi(argv[2]);
	int P = atoi(argv[3]);
	int rank,p;
	int n = 10;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	assert((n/p)%2 == 0);

	int A[10] = {1,2,3,4,5,6,7,8,9,10};
	int B[10];
	if (rank == 4){
		for(i = 0; i< n; i++){
			printf("%d \n", i);
			printf("| %d |\n", A[i]);
		}
	}
	for(i=0; i < (log(n)/log(2)); i++){
		int j;
		#pragma omp parallel for
		for(j = 0; j < n;j++){
			int pw = pow(2,i);
			if (j<pw){
				B[j] = A[j];
			}else{
				B[j] = A[j] + A[j-pw];
			}
		}
		memcpy(A,B,sizeof(A));	
	}


	if (rank == 4){
		for(i = 0; i< n; i++){
			printf("%d \n", i);
			printf("| %d |\n", A[i]);
		}
	}
	

	int local = A[n-1];
	int global = local;	
	MPI_Status status;
	MPI_Request request;
	for(i=0; i<log(p)/log(2); i++){
		local = global;
		int recvglobal;
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank + pow(2,i) < p){
			MPI_Send(&global,1,MPI_INT,rank + pow(2,i), 0, MPI_COMM_WORLD);
		}	
		int j;
		for(j=0; j<p; j++){
			if(j+pow(2,i) == rank){
				MPI_Recv(&recvglobal, 1, MPI_INT, rank-pow(2,i),MPI_ANY_TAG,MPI_COMM_WORLD, &status);
				if (rank-pow(2,i) < rank){
					global = global+recvglobal;
				}
			}
		}

	}

	int recievedglobal;
	if(rank+1 < p)
		MPI_Send(&global, 1 , MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	if(rank-1 > 0)
		MPI_Recv(&recievedglobal, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//	printf("My rank is %d and my recvglobal is %d",rank,recievedglobal);
	MPI_Barrier(MPI_COMM_WORLD);
	global = recievedglobal;
	for(i=0;i<n/p;i++){
		A[i] = A[i] + global;
	}
/*
	for(i=0;i<p;i++){
		MPI_Barrier(MPI_COMM_WORLD);
		if(i==rank){
			for (i=0; i< (n/p); i++){
				printf("A[%d] = %d\n", i, A[i]);
			}
		}
	}
*/

//
	if(rank == 1){
		printf("My rank is %d\n", rank);
		for (i=0; i< (n/p); i++){
			printf("A[%d] = %d\n", i, A[i]);
		}
		printf("The global value we got was %d",local);
	}
	if(rank == 0){
		printf("My rank is %d\n", rank);
		for (i=0; i< (n/p); i++){
			printf("A[%d] = %d\n", i, A[i]);
		}
		printf("The global value we got was %d",local);
	}








	MPI_Finalize();
}
