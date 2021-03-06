#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>
#include "matrixoperations.h"


int main( int argc, char *argv[]){
	//User must input A B and a large Prime P
	// A and B for the initial M matrix [A 0] [B 1]
	if (argc < 5){
		printf("Useage a.out a b P X_0\n");
		return 0;
	}
	int A = atoi(argv[1]);
	int B = atoi(argv[2]);
	int P = atoi(argv[3]);
	int X_0 = atoi(argv[4]);
	int rank,p;
	int n = 32;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	assert(p>=2);
	assert((n/p)%2 == 0);
	//Creating a 3d matrix to hold the many M(2x2) matrixies
	//n x 2 x 2
	long int ***Mmatrix = (long int ***)malloc(sizeof(long int**)*n/p);

	// Use a temp 3d matrix to store for next timestep
	long int ***temp = (long int ***)malloc(sizeof(long int**)*n/p);
	int i;


	

	// Initializing 2 arrays of 2d matrices in each rank of size n/p
	// Mmatrix - for current timestep
	// temp - for next timestep	
	for (i=0; i < (n/p); i++){
		Mmatrix[i] = init_2b2_matrix();
		temp[i] = init_2b2_matrix();
	}
	//Done with matrix allocation. Empty matricies right now	
	//if you have a rank you can send it to in range
			//Send global to its rank + 2^timestep//if you have a rank you can send it to in range
			//Send global to its rank + 2^timestep//Start matrix 1 as identity matrix
	/*========= Assinging identity matrix values to the first matrix in array Mmatrix =========*/
	Mmatrix[0] = assign_2b2_matrix(Mmatrix[0], 1, 0, 0, 1);
	
	// all other matricies start as M
	// where M =
	//  [A 0]
	//  [B 1]


	// Condition to make sure the first matrix in rank 0's block is the identity matrix
	if(rank == 0){
		i = 1;
	} else {
		i = 0;
	}

	// Assign initial values to each matrix in rank's respective block
	for (; i < (n/p); i++){
		Mmatrix[i] = assign_2b2_matrix(Mmatrix[i], A, B, 0, 1);
	}
	
	//Just to print out a random rank's array of M's
	// just for debugging
	/* if (rank == 4){
		for(i = 0; i< n/p; i++){
			printf("%d \n", i);
			printf("| %li | %li |\n", Mmatrix[i][0][0], Mmatrix[i][0][1]);
			printf("| %li | %li |\n", Mmatrix[i][1][0], Mmatrix[i][1][1]);
		}
	} */


	//Start of N-value parallel prefix
	for(i=0; i < (log((n/p))/log(2)); i++){
		//for each timestep 0-log2(n) where n = n/p
		int j;
		//start of parallel region
		#pragma omp parallel for
		for(j = 0; j < n/p;j++){
			//for every value in the array
			int pw = pow(2,i);               
			

			//pw = 2 ^ the timestep
			if (j<pw){
				//if the index is lower than 2^ts then we just copy its local to its global
				temp[j] = Mmatrix[j];
			}else{
				//else the index is bigger than 2^ts
				// then we multiply its local to index - pw and make that its global
				temp[j] = twoby2times2by2(Mmatrix[j],Mmatrix[j-pw]);
			}
		}
		//Copy all globals into locals
		memcpy(Mmatrix,temp,sizeof(long int**)*(n/p));	
	}


	//At this polong int every rank holds an array 
	// A = [M^0, M^1, M^2, ... , M^n/p-1]

	//prlong inting a random array's array of M's
	/* if (rank == 1){
		for(i = 0; i< n/p; i++){
			printf("%d \n", i);
			printf("| %li | %li |\n", Mmatrix[i][0][0], Mmatrix[i][0][1]);
			printf("| %li | %li |\n", Mmatrix[i][1][0], Mmatrix[i][1][1]);
		}
	} */
	
	//Start P-value parallel prefix
	
	long int **local = init_2b2_matrix(); // To keep track of local matrix as it updates
	long int **global = init_2b2_matrix();	// To keep track of the global matrix as it updates
	long int **recvglobal = init_2b2_matrix();	// To store the received global from its mate process
	long int **offset = init_2b2_matrix();	// To store the offset that the rank under it sends to it and use it to update block
	

	// Assign calculated value from n parallel prefix to each of the ranks' initial local and global
	global = assign_2b2_matrix(global, Mmatrix[n/p-1][0][0], Mmatrix[n/p-1][1][0], Mmatrix[n/p-1][0][1], Mmatrix[n/p-1][1][1]);
	local = assign_2b2_matrix(global, Mmatrix[n/p-1][0][0], Mmatrix[n/p-1][1][0], Mmatrix[n/p-1][0][1], Mmatrix[n/p-1][1][1]);
	
    
	MPI_Status status[2];
	MPI_Request request[2];
	int mate,v;
	for(i = 0; i < log(p)/log(2); i++){
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank == 0){
			printf("-------------------------ITERATION #%d----------------------------\n", i);
		}
		
		v = pow(2,i);
		mate = rank ^ v;
		v = v << 1;
		/* if(rank == 1){
			printf("Timestep: %d\nRank: %d global[0] %li | %li\n", i, rank, global[0][0], global[0][1]);
			printf("Rank: %d global[1] %li | %li\n", rank, global[1][0], global[1][1]);
		} */
		//printf("Rank %d sending to %d at timestep %d\n", rank, mate, i);
		MPI_Isend(global[0],2, MPI_LONG, mate, 0, MPI_COMM_WORLD, &request[0]);
		MPI_Recv(recvglobal[0], 2, MPI_LONG, mate, MPI_ANY_TAG, MPI_COMM_WORLD, &status[0]);
		MPI_Wait(&request[0], &status[0]);

		MPI_Isend(global[1],2, MPI_LONG, mate, 0, MPI_COMM_WORLD, &request[1]);
		MPI_Recv(recvglobal[1], 2, MPI_LONG, mate, MPI_ANY_TAG, MPI_COMM_WORLD, &status[1]);
		MPI_Wait(&request[1], &status[1]);

		/* if(rank == 7){
			
			printf("Timestep: %d\nRank: %d recvglobal[0] %li | %li\n", i, rank, recvglobal[0][0], recvglobal[0][1]);
			printf("Rank: %d recvglobal[1] %li | %li\n", rank, recvglobal[1][0], recvglobal[1][1]);
		} */
		global = twoby2times2by2(global,recvglobal);
		
		if(mate < rank){
			local = twoby2times2by2(local,recvglobal);
		}
		MPI_Barrier(MPI_COMM_WORLD);
		int j;
		for(j = 0; j < p; j++){
			
			if(rank == j){
				printf("Rank %d local:\n| %li | %li |\n| %li | %li |\n", rank, local[0][0], local[0][1], local[1][0], local[1][1]);
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
		
	}

	

	/* if(rank == 0 || rank == 1){
		printf("Rank: %d\n global[0] %li %li\n", rank, global[0][0], global[0][1]);
		printf("Rank: %d\n global[1] %li %li\n", rank, global[1][0], global[1][1]);
	} */
	
	//
	
	
	
	/* if(rank == 1){
		printf("Rank: %d global[0] %li | %li\n", rank, global[0][0], global[0][1]);
		printf("Rank: %d global[1] %li | %li\n", rank, global[1][0], global[1][1]);
	} */
	
	if(rank != p - 1){ // if process is not the last one
		MPI_Isend(local[0], 2, MPI_LONG, rank + 1, 0, MPI_COMM_WORLD, &request[0]);
		
	} 
	if(rank != 0){ // if process is not the first one
		MPI_Recv(offset[0], 2, MPI_LONG, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status[0]);
	} 
	
	if(rank != p - 1){ // if process is not the last one
		MPI_Isend(local[1], 2, MPI_LONG, rank + 1, 0, MPI_COMM_WORLD, &request[0]);
	} 
	if(rank != 0){ // if process is not the first one
		MPI_Recv(offset[1], 2, MPI_LONG, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status[0]);
	} 

	/* if(rank == 2){
		printf("Rank: %d offset[0] %li | %li\n", rank, offset[0][0], offset[0][1]);
		printf("Rank: %d offset[1] %li | %li\n", rank, offset[1][0], offset[1][1]);
	} */

	// Updating each matrix with the offset
	if(rank != 0){
		#pragma omp parallel for
		for(i=0;i<n/p;i++){
			Mmatrix[i] = twoby2times2by2(Mmatrix[i], offset);
		}
	}
	
	long int* X_i = (long int*) malloc(sizeof(long int)*n/p);
	long int* base = (long int*)malloc(sizeof(long int)*2);
	base[0] = X_0, base[1] = 1;
	
	//#pragma omp parallel for
	for(i=0;i<n/p;i++){
		/* if(rank == 2){
			printf("Rank: %d Mmatrix[0] %li | %li\n", rank, Mmatrix[i][0][0], Mmatrix[i][0][1]);
			printf("Rank: %d Mmatrix[1] %li | %li\n", rank, Mmatrix[i][1][0], Mmatrix[i][1][1]);

		} */
		X_i[i] = oneby2times2by2(base, Mmatrix[i], P);
		if(rank == 0 || rank == 1 || rank == 2){
			printf("Rank: %d X_i[%d] = %li\n", rank, i, X_i[i]);
		}
	}
	


	MPI_Finalize();
}
