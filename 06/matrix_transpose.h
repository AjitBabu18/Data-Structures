/*
 *  This file is #include'd inside the definition of a matrix class
 *  like this:
 *
 *  	class ClassName {
 *          // Number of rows and columns of the matrix
 *          unsigned N;
 *
 *          // Swap elements (i1,j1) and (i2,j2)
 *          void swap(unsigned i1, unsigned j1, unsigned i2, unsigned j2);
 *
 *          // Your code
 *          #include "matrix_transpose.h"
 *      }
 */

/*
void transpose() { 
    unsigned i = 0;
    unsigned j = 0;
    //unsigned y = 0
    for(unsigned i=0; i<N; i++)
        for (unsigned j=0; j<i; j++){

    unsigned x = (N / 2);
    unsigned y = N - x;
    goto jump;
    trans_swap(i + y, j, x, y);
    goto jump1;
    i+=y;j+=y;
    jump:
    if(y > 1) {
        x = y / 2;
        y -=  x;}
    jump1:
    if(x > 1) {
        x = x / 2;
        y -=  x;}}
}
*/
void _transpose(unsigned i, unsigned j, unsigned k) {
    if(k > 1) {
        unsigned x = k / 2; 
        unsigned y = k - x;
        _transpose(i, j, y);
        _swap(i + y, j, x, y);
        _transpose(i + y, j + y, x);
    }
}

void transpose() { 
    _transpose(0, 0, N); 
}

void _swap(unsigned i, unsigned j, unsigned k, unsigned l) {
    if(l + k == 2) {swap(i, j, j, i);} 
    else if(l >= k) {
        unsigned x = l / 2;
        unsigned y = l - x;
        _swap(j, i, y, k); 
        _swap(j + y, i, x, k);} 
    else { _swap(j, i, l, k);}
}