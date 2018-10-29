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
	int n = 128;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	assert((n/p)%2 == 0);
	//Creating a 3d matirx to hold the many M(2x2) matrixies
	//n x 2 x 2
	int ***Mmatrix = (int ***)malloc(sizeof(int**)*n/p);
	//use a temp to store for next timestrp
	int ***temp = (int ***)malloc(sizeof(int**)*n/p);
	int i;
	for (i=0; i < (n/p); i++){
		Mmatrix[i] = (int **)malloc(sizeof(int *)*2);
		temp[i] = (int **)malloc(sizeof(int *)*2);
	}
	for (i=0; i < (n/p); i++){
		Mmatrix[i][0] = (int*)malloc(sizeof(int)*2);
		Mmatrix[i][1] = (int*)malloc(sizeof(int)*2);
		temp[i][0] = (int*)malloc(sizeof(int)*2);
		temp[i][1] = (int*)malloc(sizeof(int)*2);
	}
	

	Mmatrix[0][0][0] = 1; 
	Mmatrix[0][0][1] = 0;
	Mmatrix[0][1][0] = 0;
	Mmatrix[0][1][1] = 1;
	for (i=1; i < (n/p); i++){
		Mmatrix[i][0][0] = A;
		Mmatrix[i][0][1] = 0;
		Mmatrix[i][1][0] = B;
		Mmatrix[i][1][1] = 1;
	}

	if (rank == 4){
		for(i = 0; i< n/p; i++){
		printf("%d \n", i);
		printf("| %d | %d |\n", Mmatrix[i][0][0], Mmatrix[i][0][1]);
		printf("| %d | %d |\n", Mmatrix[i][1][0], Mmatrix[i][1][1]);
		}
	}
	for(i=0; i < (log((n/p))/log(2)); i++){
		int j;
		#pragma omp parallel for
		for(j = 0; j < n/p;j++){
			int pw = pow(2,i);
			if (j<pw){
				temp[j] = Mmatrix[j];
			}else{
				temp[j] = twoby2times2by2(Mmatrix[j],Mmatrix[j-pw]);
			}
		}
		memcpy(Mmatrix,temp,sizeof(int**)*(n/p));	
	}


	if (rank == 4){
		for(i = 0; i< n/p; i++){
		printf("%d \n", i);
		printf("| %d | %d |\n", Mmatrix[i][0][0], Mmatrix[i][0][1]);
		printf("| %d | %d |\n", Mmatrix[i][1][0], Mmatrix[i][1][1]);
		}
	}
	

	int** local = Mmatrix[n/p-1];
	int** global = local;	
	MPI_Status status;
	MPI_Request request;
	for(i=0; i<log(p)/log(2); i++){
		local = global;
		int **recvglobal;
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank + pow(2,i) < p){
			MPI_Send((&global[0][0]),4,MPI_INT,rank + pow(2,i), 0, MPI_COMM_WORLD);
		}	
		int j;
		for(j=0; j<p; j++){
			if(j+pow(2,i) == rank){
				MPI_Recv(&(recvglobal[0][0]), 4, MPI_INT, rank-pow(2,i),MPI_ANY_TAG,MPI_COMM_WORLD, &status);
				if (rank-pow(2,i) < rank){
					global = twoby2times2by2(global,recvglobal);
				}
			}
		}

	}
/*
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
	for(i=0;i<p;i++){
		MPI_Barrier(MPI_COMM_WORLD);
		if(i==rank){
			for (i=0; i< (n/p); i++){
				printf("A[%d] = %d\n", i, A[i]);
			}
		}
	}


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

*/






	MPI_Finalize();
}