#include <stdio.h>
#include <stdlib.h>

#define NROWS 5             // rows
#define NCOLS 10            // columns

void print_2Darr( double **arr_2D, int N_rows, int N_cols );
void free_matrix_d( double** arr, long int Nrows );
void myError(char error_text[]);

int main() {

    int
        ii,jj;              // indices for arrays

    int
        N_cols;             // columns (to check that one of the alloc-functions work w/o macro, too)

    double
        *arr1[NROWS],
        **arr3,
        **arr4,
        **arr6;

    // Note that   (sizeof *arr)  = sizeof( double *)
    // and         (sizeof **arr) = sizeof( double )


    // array of pointers to columns
    // valgrind --leak-check=yes shows no error for following method
    for ( ii=0 ; ii<NROWS ; ++ii) {
         arr1[ii] = calloc(NCOLS , sizeof(double) );
        if ( arr1[ii]==NULL )
            myError( "could not (c)allocate arr1[ii]" );
    }
    for (ii = 0; ii < NROWS; ++ii)
        for (jj = 0; jj < NCOLS; ++jj )
           arr1[ii][jj] = 3.*(double)(ii+jj);
    printf( "arr1:\n" );
    print_2Darr( arr1, NROWS, NCOLS );
    for ( ii=0 ; ii<NROWS ; ++ii)
        free( arr1[ii] ) ;

    // pointer to pointers
    // valgrind --leak-check=yes shows no error for following method
    // (true for using direct free here or in separate function)
    // allocate storage for an array of pointers
    arr3 = calloc(NROWS , sizeof(double *));
    if ( arr3==NULL )
        myError( "could not (c)allocate arr3" );
    // allocate for each pointer memory for an array of doubles
    for ( ii=0 ; ii<NROWS ; ii++ ) {
        arr3[ii] = calloc(NCOLS , sizeof(double));
        if ( arr3[ii]==NULL )
            myError( "could not (c)allocate arr3[ii]" );
    }
    // Note that arr[i][j] is same as *(*(arr+i)+j) (also true for most of the other methods)
    for ( ii = 0; ii < NROWS; ii++ )
        for ( jj = 0; jj < NCOLS; jj++ )
            arr3[ii][jj] = 2.4*ii+.2*jj;
    printf( "arr3:\n" );
    print_2Darr( arr3, NROWS, NCOLS );
//    free_matrix_d( arr3, NROWS );
    for ( ii=0 ; ii<NROWS ; ++ii)
    // free for each pointer its array of doubles
        free( arr3[ii] ) ;
    // free array of pointers
    free( arr3 );

    // Using two pointers and one malloc call for all rows
    // valgrind --leak-check=yes shows no error for following method
    arr4    = calloc(sizeof(double *) , NROWS);
    if ( arr4==NULL )
        myError( "could not (c)allocate arr4" );
    arr4[0] = calloc(sizeof(double) , NCOLS * NROWS);
    if ( arr4[0]==NULL )
        myError( "could not (c)allocate arr4[0]" );
    for( ii = 0; ii < NROWS; ++ii )
        arr4[ii] = (*arr4 + NCOLS * ii);
    for ( ii = 0; ii < NROWS; ++ii )
        for ( jj = 0; jj < NCOLS; ++jj )
            arr4[ii][jj] = 0.1*ii+.7*jj;
    printf( "arr4:\n" );
    print_2Darr( arr4, NROWS, NCOLS );
    free( arr4[0] );
    free( arr4 );

    // method suggested from stackoverflow, "real" 2D arrays ?
    // valgrind --leak-check=yes shows no error for following method
    N_cols = 4;
    double (*arr5)[N_cols] = calloc(NROWS * N_cols, sizeof(double));
    if ( arr5==NULL )
        myError( "could not (c)allocate arr5" );
    for ( ii = 0; ii < NROWS; ++ii )
        for ( jj = 0; jj < N_cols; ++jj )
            arr5[ii][jj] = 0.1*ii+.7*jj; 
    printf( "arr5:\n" );
    // note, the print_2Darr function is not working as it expects **arr as argument    
    for ( ii = 0; ii < NROWS; ++ii ) {
        for ( jj = 0; jj < N_cols; ++jj )
            printf( "%f\t",arr5[ii][jj] );
        printf( "\n" );
    }
    free( arr5 );

    // method similar to the one which is used in numerical recipies
    // result is a pointer to an array of pointers to rows
    // valgrind --leak-check=yes shows no error for following method
	// allocate pointers to rows
	arr6 = calloc( (size_t)NROWS, sizeof *arr6 );
    if ( arr6==NULL )
        myError( "could not (c)allocate arr6" );
	// allocate rows ...
	arr6[0] = calloc( (size_t)(NROWS*NCOLS), sizeof **arr6 );
    if ( arr6[0]==NULL )
        myError( "could not (c)allocate arr6[0]" );
    // ... and set pointers to them
	for ( ii=1 ; ii<NROWS ; ++ii ) 
        arr6[ii] = arr6[ii-1] + NCOLS;
    for ( ii = 0; ii < NROWS; ++ii )
        for ( jj = 0; jj < NCOLS; ++jj )
            arr6[ii][jj] = 0.1*ii+.7*jj; 
    printf( "arr6:\n" );
    print_2Darr( arr6, NROWS, NCOLS );
    free( arr6[0] );
    free( arr6 );

    return EXIT_SUCCESS;
}


void print_2Darr( double **arr_2D, int N_rows, int N_cols ) {
    int
        ii, jj;

    // print 2D array to console
    for ( ii=0 ; ii<N_rows ; ++ii) {
        for ( jj=0 ; jj<N_cols; ++jj) {
            printf( "%f\t", arr_2D[ii][jj] );
        }
        printf( "\n" );
    }
    printf( "\n" );
}


void free_matrix_d( double** arr, long int Nrows ) {

    long int
        ii;

    for( ii=0 ; ii < Nrows; ii++)
        free( arr[ii] );
    free( arr );    
}


void myError(char error_text[]) {
    fprintf( stderr, "ERROR occured:\n" );
    fprintf( stderr, "%s\n",error_text );
    fprintf( stderr, "will exit to system\n" );
    exit( EXIT_FAILURE );
}
