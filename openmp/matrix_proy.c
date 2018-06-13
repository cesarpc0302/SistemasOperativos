#include <stdio.h>
#include <pthread.h>  
#include <time.h>
#include <stdlib.h>
#include <math.h>
//#include <omp.h>

#define m 500 /**/
#define n 1000 /**/
#define q 1000 /**/


int c, d, k = 0;
float first[m][n], second[n][q], multiply[m][q];

void generate_matrix()
{

/*  	#pragma omp parallel
  	{
	#pragma omp for*/
  	for (c = 0; c < m; c++)
    	for (d = 0; d < n; d++)
      		first[c][d]=rand();
	//}

    
    /*#pragma omp parallel 
    {
	#pragma omp for*/
    for (c = 0; c < n; c++)
    	for (d = 0; d < q; d++)
        	second[c][d]=rand();
    //}
    
}

void matrix_mult ()
{
	
	/*#pragma omp parallel 
	{ 
	#pragma omp for
*/	for (c = 0; c < m; c++) {
		for (d = 0; d < q; d++) {
			for (k = 0; k < n; k++) {
				multiply[c][d] += first[c][k]*second[k][d];
        	}
		}
    }
    //}
}

void matrix_sum ()
{
	
	/*#pragma omp parallel
	{ 
	#pragma omp for
*/	for (c = 0; c < m; c++) {
		for (d = 0; d < q; d++) {
			for (k = 0; k < n; k++) {
				multiply[c][d] += first[c][k]+second[k][d];
        	}
		}
    }
  //  }
}

void matrix_sub ()
{
	
	/*#pragma omp parallel
	{ 
	#pragma omp for
*/	for (c = 0; c < m; c++) {
		for (d = 0; d < q; d++) {
			for (k = 0; k < n; k++) {
				multiply[c][d] += first[c][k]-second[k][d];
        	}
		}
    }
  //  }
}

void matrix_first_exp ()
{
	/*#pragma omp parallel
	{ 
	#pragma omp for
*/	for (c = 0; c < m; c++) {
		for (k = 0; k < n; k++) {
			first[c][k] += first[c][k]*first[c][k];
		}
    }
  //  }
}


void matrix_second_exp ()
{
	/*#pragma omp parallel
	{ 
*/	#pragma omp for
	for (c = 0; c < n; c++) {
		for (k = 0; k < q; k++) {
			second[c][k] += second[c][k]*second[c][k];
		}
    }
  //  }
}

void matrix_first_sqrt ()
{
	
	/*#pragma omp parallel
	{ 
	#pragma omp for
*/	for (c = 0; c < m; c++) {
		for (k = 0; k < n; k++) {
			first[c][k] += sqrt(first[c][k]);
		}
    }
  //  }
}


void matrix_second_sqrt ()
{
	/*#pragma omp parallel 
	{ 
	#pragma omp for
*/	for (c = 0; c < n; c++) {
		for (k = 0; k < q; k++) {
			second[c][k] += sqrt(second[c][k]);
		}
    }
  //  }
}

void print_matrix ()
{
   printf("Product of the matrices:\n");
 	int sum = 0;
    for (c = 0; c < m; c++) {
      	for (d = 0; d < q; d++)
        	//printf("%d\t", multiply[c][d]);
        sum++;
      	//printf("\n");
    }
    
 
}

int main(int argc, char *argv[])
{
/*	char * num = argv[1];
	int threads = atoi(num);
	printf("threads: %d\n\n",threads);
*/	
	clock_t en,st;
//	omp_set_num_threads(threads);
	st = clock();	
    generate_matrix();
    matrix_mult ();
    print_matrix ();
    matrix_sum ();
    print_matrix ();    
    matrix_sub ();    
    print_matrix ();
    matrix_first_exp ();
    print_matrix (); 
    matrix_second_exp ();
    print_matrix ();
    matrix_first_sqrt ();
    print_matrix ();
    matrix_second_sqrt ();
    print_matrix ();
       
	en = clock();
	double t = (double)(en-st)/CLOCKS_PER_SEC;
	printf("Duracion total: %f\n", t);
  	printf("task finished\n");
 
	return 0;
}
