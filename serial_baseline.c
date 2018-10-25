#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if (argc < 4){
		printf("Usage a.out A B N\n");
		exit(0);
	}
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int n = atoi(argv[3]);
	int p = 199;
	int* x = (int*)malloc(sizeof(int)*n);
	int i;
	x[0] = (a+b)%p;
	printf("%d -> %d\n",0 , x[0]);
	for(i = 1 ; i < n; i++ ){
		x[i] = (a*x[i-1]+b)%p; 
		printf("%d -> %d\n",i , x[i]);
	}


}

