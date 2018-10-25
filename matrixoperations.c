#include "matrixoperations.h"

int* oneby2times2by2(int * obt, int ** tbt){
	int *ret = (int *)malloc(sizeof(int)*2);
	ret[0] = (obt[0] * tbt[0][0]) + (obt[1] * tbt[1][0]);
	ret[1] = (obt[0] * tbt[0][1]) + (obt[1] * tbt[1][1]); 
	return ret;
}

int** twoby2times2by2(int **tbt, int **tbt2){
	int **ret = (int **)malloc(sizeof(int*)*2);
	ret[0] = (int *)malloc(sizeof(int)*2);
	ret[1] = (int *)malloc(sizeof(int)*2);

	ret[0][0] = (tbt[0][0] * tbt2[0][0])+(tbt[0][1] * tbt2[1][0]);
	ret[0][1] = (tbt[0][0] * tbt2[0][1])+(tbt[0][1] * tbt2[1][1]);
	ret[1][0] = (tbt[1][0] * tbt2[0][0])+(tbt[1][1] * tbt2[1][0]);
	ret[1][1] = (tbt[1][0] * tbt2[0][1])+(tbt[1][1] * tbt2[1][1]);
	return ret;
} 

int* modmatrixbyval(int * obt, int p){
	int *ret = (int *)malloc(sizeof(int)*2);
	ret[0] = obt[0] % p;
	ret[1] = obt[1] % p;
	return ret;
}
/*

int main(int argc, char *argv){
	int **twobytwo = (int **)malloc(sizeof(int*)*2);
	int* onebytwo = (int*)malloc(sizeof(int)*2);
	int i;
	for (i = 0; i < 2 ; i++){
		twobytwo[i] = (int*)malloc(sizeof(int)*2);
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
	int* answer = (int*)malloc(sizeof(int)*2);
	answer = oneby2times2by2(onebytwo,twobytwo);
	printf("\n| %d | %d |\n", answer[0], answer[1]);
	answer = modmatrixbyval(answer,10);
	printf("\n| %d | %d |\n", answer[0], answer[1]);

	int** square = twoby2times2by2(twobytwo,twobytwo);
	printf("\n| %d | %d |\n", square[0][0], square[0][1]);
	printf("| %d | %d |\n", square[1][0], square[1][1]);

	answer = oneby2times2by2(onebytwo,square);
	printf("\n| %d | %d |\n", answer[0], answer[1]);
	answer = modmatrixbyval(answer,10);
	printf("\n| %d | %d |\n", answer[0], answer[1]);


}

*/
