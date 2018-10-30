#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if (argc < 5){
		printf("Usage a.out A B P X_0\n");
		exit(0);
	}
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int n =	32; 
	int p = atoi(argv[3]);
	int* x = (int*)malloc(sizeof(int)*n);
	int i;
	x[0] = atoi(argv[4]);
	printf("%d -> %d\n",0 , x[0]);
	for(i = 1 ; i < n; i++ ){
		x[i] = (a*x[i-1]+b)%p; 
		printf("%d -> %d\n",i , x[i]);
	}
}
