#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int main(int argc, char *argv[]){
	int A[8] = {1,2,3,4,5,6,7,8};
	int B[8];
	int n = 8;
	printf("%.2lf log2 \n", log(n)/log(2));
	for(int i=0; i<(log(n)/log(2)); i++){
		#pragma omp parallel for
		for (int j=0; j<n; j++){
			int pw = pow(2,i);
			if (j<pw){
				B[j] = A[j];
			}else{
				B[j] = A[j] + A[j-pw];
			}
		}
		memcpy(A,B,sizeof(A));	
		printf("%dth step:\n",i);
		for (int i = 0; i< n; i++){
			printf("| %d | %d |\n", i, A[i]);
	
		}
		printf("end %dth step:\n",i);
	}
		
/*
	int prefix[9] = {0};
//	omp_lock_t mylock;
//	omp_init_lock(&mylock);
	#pragma omp parallel for 
	for (int i = 1; i < n; i++){
//		omp_set_lock(&mylock);
		prefix[i-1] = A[i-1] + A[i];
//		omp_unset_lock(&mylock);
	}
	printf("First step:\n");
	for (int i = 1; i<= n; i++){
		printf("| %d | %d |\n", i, prefix[i-1]);
	}
	printf("end First step:\n");
	int k = 2;
	while ( k<n){
		#pragma omp parallel for 
		for (int j = k; j<n; j++){
//			omp_set_lock(&mylock);
			prefix[j] = prefix[j-k] + prefix[j];
//			omp_unset_lock(&mylock);
		}
		printf("%dth step:\n", k);
		for (int i = 1; i< n; i++){
			printf("| %d | %d |\n", i, prefix[i-1]);
		}
		printf("end %dth step:\n",k);
		k = k+k;
	}
	for (int i = 1; i< n; i++){
		printf("| %d | %d |\n", i, prefix[i-1]);
	}
//	omp_destroy_lock(&mylock);
*/
/*
	#pragma omp parallel num_threads(n)
	{
		int rank = omp_get_thread_num();
		int *local = A;
		int global = A;
		int v = 0;
		int i;
		for(i = 0; i < (log(n)/log(2))-1; i++){
			int mate = pow(rank,v);

			

		}
*/


}
