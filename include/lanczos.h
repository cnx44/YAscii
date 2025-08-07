#ifndef LANCZOS_H
#define LANCZOS_H

#include <stdint.h>
#include <stdbool.h>
#include "commons.h" 

#define KERNEL_RADIUS 2
#define SAMPLE_SIZE  (2 * KERNEL_RADIUS + 1)

/*
 * lanczos_kernel precomputed 1D Lanczos kernel of size SAMPLE_SIZE.
 *
 * This kernel is symmetric and used for separable 2D convolution.
 * It approximates the sinc function multiplied by a Lanczos window.
 * The coefficients are centered around the middle element.
 */
static const double lanczos_kernel[SAMPLE_SIZE] = {
   -0.03628802,
    0.14171904,
    0.78913800,
    0.14171904,
   -0.03628802,
};

/*
 * neighbor_offset relative (row, col) offsets forming a SAMPLE_SIZE × SAMPLE_SIZE grid.
 *
 * These offsets are used to collect pixel values from a neighborhood centered at
 * a given coordinate during Lanczos filtering. Each entry represents the offset
 * from the center pixel, where:
 *   neighbor_offset[i][0] = row offset
 *   neighbor_offset[i][1] = column offset
 *
 * The grid is assumed to be square and symmetric.
 */
static const int neighbor_offset[SAMPLE_SIZE*SAMPLE_SIZE][2] = {
    { -2, -2 }, { -2, -1 }, { -2,  0 }, { -2,  1 }, { -2,  2 },
    { -1, -2 }, { -1, -1 }, { -1,  0 }, { -1,  1 }, { -1,  2 },
    {  0, -2 }, {  0, -1 }, {  0,  0 }, {  0,  1 }, {  0,  2 },
    {  1, -2 }, {  1, -1 }, {  1,  0 }, {  1,  1 }, {  1,  2 },
    {  2, -2 }, {  2, -1 }, {  2,  0 }, {  2,  1 }, {  2,  2 }
};

/*
 * is_inbound checks whether given matrix coordinates are within bounds.
 * - row:   row index to check
 * - col:   col index to check
 * - rows:  total numbers of rows in the matrix
 * - cols:  total numbers of cols in the matrix
 *
 * return true iff (row, col) is a valid coordinate in in matrix of size rows x cols
 */
static inline bool is_inbound(int row, int col, int rows, int cols){
    return (row >= 0 && col >= 0 && col < cols && row < rows); 
}


/*
 * lanczos_point_convulution applies the 2D convultion to the sample matrix
 * - sample_matrix: Pointer to a flattened 2D matrix of size SAMPLE_SIZE x SAMPLE_SIZE holding the neighbours
 *                  of the target pixel
 *
 * This function perform a separable 2D convolution using the precomputed Lanczos Kenrnel by doing the following:
 * 1. For each row compute the 1D convultion with the Kenrnel
 * 2. applies the 1D convultion on each convulted value using the same kernel
 *
 * This assumes both lanczos_kern and sample_matrix have consistent dimesnion with SAMPLE_SIZE
 *
 * return the scalar valure from the 2D convultion of the given point
 */
static double lanczos_point_convulution(double* sample_matrix){
    double convoluted_value = 0; 

    for(int row = 0; row < SAMPLE_SIZE; row++){
        double temp_convoluted_row=0;
        for(int col = 0; col < SAMPLE_SIZE; col++){
            temp_convoluted_row += lanczos_kernel[col] * sample_matrix[row * SAMPLE_SIZE + col];
        }
        convoluted_value += lanczos_kernel[row] * temp_convoluted_row;
    }
    
    return convoluted_value;
}



Pixel* lanczos_scale(AsciiImageObject* src, int scale_factor);

#endif
