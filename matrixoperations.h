#include <stdio.h>
#include <stdlib.h>

long int ** init_2b2_matrix(void);
long int ** assign_2b2_matrix(long int**, long int, long int, long int, long int);
long int oneby2times2by2(long int *, long int**, int);
long int** twoby2times2by2(long int**, long int**);
long int modmatrixbyval(long int*, int);
