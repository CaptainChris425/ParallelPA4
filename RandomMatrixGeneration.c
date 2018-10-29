#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <mpi.h>
#include <assert.h>
#include "matrixoperations.h"


int main(int argc, char *argv[]){
	//User must input A B and a large Prime P
	// A and B for the initial M matrix [A 0] [B 1]
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
	for (i=0; i < (n/p); i++)
		Mmatrix[i][0] = (int*)malloc(sizeof(int)*2);
		Mmatrix[i][1] = (int*)malloc(sizeof(int)*2);
		temp[i][0] = (int*)malloc(sizeof(int)*2);
		temp[i][1] = (int*)malloc(sizeof(int)*2);
	}
	//Done with matrix allocation. Empty matricies right now	
	//Start matrix 1 as identity matrix
	// all other matricies start as M
	// where M =
	//  [A 0]
	//  [B 1]
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
	
	//Just to print out a random rank's array of M's
	// just for debugging
	if (rank == 4){
		for(i = 0; i< n/p; i++){
		printf("%d \n", i);
		printf("| %d | %d |\n", Mmatrix[i][0][0], Mmatrix[i][0][1]);
		printf("| %d | %d |\n", Mmatrix[i][1][0], Mmatrix[i][1][1]);
		}
	}


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
		memcpy(Mmatrix,temp,sizeof(int**)*(n/p));	
	}

	//At this point every rank holds an array 
	// A = [M^0, M^1, M^2, ... , M^n/p-1]

	//printing a random array's array of M's
	if (rank == 4){
		for(i = 0; i< n/p; i++){
		printf("%d \n", i);
		printf("| %d | %d |\n", Mmatrix[i][0][0], Mmatrix[i][0][1]);
		printf("| %d | %d |\n", Mmatrix[i][1][0], Mmatrix[i][1][1]);
		}
	}
	
	//Start P-value parallel prefix
	int** local = Mmatrix[n/p-1];
	int** global = local;	
	MPI_Status status;
	MPI_Request request;
	for(i=0; i<log(p)/log(2); i++){
		//For each timestep 0 - log2(n) where n = p
		//copy global into local
		local = global;
		int **recvglobal;
		MPI_Barrier(MPI_COMM_WORLD);
		if(rank + pow(2,i) < p){
			//if you have a rank you can send it to in range
			//Send global to its rank + 2^timestep
			MPI_Send((&global[0][0]),4,MPI_INT,rank + pow(2,i), 0, MPI_COMM_WORLD);
		}	
		int j;
		for(j=0; j<p; j++){
			//for every rank (this is some funky logic but it works well
			if(j+pow(2,i) == rank){
				//if there was a rank that had sent to the current rank
				// we can check this by saying for every possible rank value if
								//its rank + 2^ts is the rank we are on
								// then we know it needs to recieve a value
				//recieve global value
				MPI_Recv(&(recvglobal[0][0]), 4, MPI_INT, rank-pow(2,i),MPI_ANY_TAG,MPI_COMM_WORLD, &status);
				if (rank-pow(2,i) < rank){
					//if it had recieved from a lower rank then multiply the global by the recieved global
					global = twoby2times2by2(global,recvglobal);
				}
			}
		}

	}

//AFTER THIS
//we need to send every global value to the next rank up
	//then add it to every value in the array
			//serially - not by parallel prefix
				//just straight add the value to everyone in the array

//once we do that
//then we have all the arrays in all the ranks to have their correct M matricies
//after that then we just multiply an array [x0 1] with all the M's to come up with the random numbers
//then we have to gather the arrays into a single rank so we can output them in order
			//or just have to print them in order
 




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
