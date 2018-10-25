#include "matrixoperations.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if (argc < 4){
		printf("Useage a.out A B N\n");
		exit(0);
	}
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int n = atoi(argv[3]);
	int p = 11;
	int* xarr = (int *)malloc(sizeof(int)*n);
	int* rhs = (int *)malloc(sizeof(int)*2);
	int* lhs = (int *)malloc(sizeof(int)*2);
	int** minit = (int **)malloc(sizeof(int*)*2);
	minit[0] = (int *)malloc(sizeof(int)*2);
	minit[1] = (int *)malloc(sizeof(int)*2);
	int** m = (int **)malloc(sizeof(int*)*2);
	m[0] = (int *)malloc(sizeof(int)*2);
	m[1] = (int *)malloc(sizeof(int)*2);
	

	minit[0][0] = a;
	minit[1][0] = b;
	minit[0][1] = 0;
	minit[1][1] = 1;

	m = minit;
	xarr[0] = (a+b)%p;
	
	rhs[0] = xarr[0];
	rhs[1] = 1;	
	lhs[1] = 1;
	
	int i;
	for (i = 1; i< n; i++){
		lhs[0] = xarr[i-1];
		rhs = modmatrixbyval( oneby2times2by2( rhs, m), p);
		xarr[i] = rhs[0];
		m = twoby2times2by2(m,minit);		
	}
	for (i=0;i<n;i++){
		printf("%d --> %d\n", i, xarr[i]);
	}











}
