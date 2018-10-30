#include "matrixoperations.h"

long int ** init_2b2_matrix(void){
	long int ** Mmatrix = (long int**)malloc(sizeof(long int*)*2);
	Mmatrix[0] = (long int*)malloc(sizeof(long int)*2);
	Mmatrix[1] = (long int*)malloc(sizeof(long int)*2);
	return Mmatrix;
}

long int ** assign_2b2_matrix(long int** Mmatrix, long int topLeft, long int bottomLeft, long int topRight, long int bottomRight){
	Mmatrix[0][0] = topLeft;
	Mmatrix[0][1] = topRight;
	Mmatrix[1][0] = bottomLeft;
	Mmatrix[1][1] = bottomRight;
	return Mmatrix;
}
long int oneby2times2by2(long int * obt, long int ** tbt, int p){
	long int *ret = (long int *)malloc(sizeof(long int)*2);
	ret[0] = (obt[0] * tbt[0][0]) + (obt[1] * tbt[1][0]);
	ret[1] = (obt[0] * tbt[0][1]) + (obt[1] * tbt[1][1]); 
	return modmatrixbyval(ret, p);
	
}

long int** twoby2times2by2(long int **tbt, long int **tbt2){
	long int **ret = (long int **)malloc(sizeof(long int*)*2);
	ret[0] = (long int *)malloc(sizeof(long int)*2);
	ret[1] = (long int *)malloc(sizeof(long int)*2);

	ret[0][0] = (tbt[0][0] * tbt2[0][0])+(tbt[0][1] * tbt2[1][0]);
	ret[0][1] = (tbt[0][0] * tbt2[0][1])+(tbt[0][1] * tbt2[1][1]);
	ret[1][0] = (tbt[1][0] * tbt2[0][0])+(tbt[1][1] * tbt2[1][0]);
	ret[1][1] = (tbt[1][0] * tbt2[0][1])+(tbt[1][1] * tbt2[1][1]);
	return ret;
} 

long int modmatrixbyval(long int * obt, int p){
	long int *ret = (long int *)malloc(sizeof(long int)*2);
	ret[0] = obt[0] % p;
	ret[1] = obt[1] % p;
	return ret[0];
}
/*

int main(long int argc, char *argv){
	long int **twobytwo = (long int **)malloc(sizeof(long int*)*2);
	long int* onebytwo = (long int*)malloc(sizeof(long int)*2);
	long int i;
	for (i = 0; i < 2 ; i++){
		twobytwo[i] = (long int*)malloc(sizeof(long int)*2);
	}
	onebytwo[0] = 5;
	onebytwo[1] = 3;
	twobytwo[0][0] = 1;
	twobytwo[1][0] = 2;
	twobytwo[0][1] = 3;
	twobytwo[1][1] = 4;
	printf("| %d | %d |\n", twobytwo[0][0], twobytwo[0][1]);
	printf("| %d | %d |\n", twobytwo[1][0], twobytwo[1][1]);
	
	printf("\n");
		
	printf("| %d | %d |\n", onebytwo[0], onebytwo[1]);
	long int* answer = (long int*)malloc(sizeof(long int)*2);
	answer = oneby2times2by2(onebytwo,twobytwo);
	printf("\n| %d | %d |\n", answer[0], answer[1]);
	answer = modmatrixbyval(answer,10);
	printf("\n| %d | %d |\n", answer[0], answer[1]);

	long int** square = twoby2times2by2(twobytwo,twobytwo);
	printf("\n| %d | %d |\n", square[0][0], square[0][1]);
	printf("| %d | %d |\n", square[1][0], square[1][1]);

	answer = oneby2times2by2(onebytwo,square);
	printf("\n| %d | %d |\n", answer[0], answer[1]);
	answer = modmatrixbyval(answer,10);
	printf("\n| %d | %d |\n", answer[0], answer[1]);


}

*/
