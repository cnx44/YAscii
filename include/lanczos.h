/*
 * Copyright (C) 2025  Oliver Quin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
 * lanczos_scale rescale an image using Lanczos interpolation.
 * - sample:        Pointer to an AsciiImageObject containing the original image data.
 * - scale_factor:  Integer factor by which the image will be downscaled.
 *
 * The function perform a downscaling of an RGBA image using separable Lanczos lanczos_point_convulution
 * 1. Allocate new buffer of Pixel structures to store the rescaled original_image
 * 2. For each Pixel in the output it identifies the corresponding floating point source coordinates and 
 * gathers the SAMPLE_SIZE * SAMPLE_SIZE neighbour around it
 * 3. Convoluted values are clamped to the [0:255] range as per 8bit using Integer
 *
 * The convultion is zero padded for pixel outside the borders
 *
 * return the pointer to the newly created Pixel struct array representing the scaled image
 *
 * The caller is responsible for freeing the returned memory.
 */
Pixel* lanczos_scale(AsciiImageObject* src, int scale_factor);

#endif
