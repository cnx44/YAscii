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

#include <stdlib.h>
#include "lanczos.h"

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
Pixel* lanczos_scale(AsciiImageObject* sample, int scale_factor){
    int height  = sample->height / scale_factor;
    int width   = sample->width / scale_factor;
    Pixel* scaled_picture       = (Pixel*) malloc(height * width * sizeof(Pixel)); 
    double red_sample_matrix[SAMPLE_SIZE * SAMPLE_SIZE]; 
    double green_sample_matrix[SAMPLE_SIZE * SAMPLE_SIZE]; 
    double blue_sample_matrix[SAMPLE_SIZE * SAMPLE_SIZE];
    double alpha_sample_matrix[SAMPLE_SIZE * SAMPLE_SIZE];


    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++){
            double source_row_fp = row * (double)sample->height / height;
			double source_col_fp = col * (double)sample->width / width;
			
            int source_row_int = (int)source_row_fp;
			int source_col_int = (int)source_col_fp;

            for(int neighbour = 0; neighbour<SAMPLE_SIZE*SAMPLE_SIZE; neighbour++){
                int neighbour_row = source_row_int + neighbor_offset[neighbour][0];
                int neighbour_col = source_col_int + neighbor_offset[neighbour][1];
                if(is_inbound(neighbour_row, neighbour_col, sample->height, sample->width)){
                    Pixel current_pixel = sample->original_image[neighbour_row * sample->width + neighbour_col];
                    red_sample_matrix[neighbour]    = (double) current_pixel.red;
                    green_sample_matrix[neighbour]  = (double) current_pixel.green;
                    blue_sample_matrix[neighbour]   = (double) current_pixel.blue;
                    alpha_sample_matrix[neighbour]  = (double) current_pixel.alpha;
                }else{
                    red_sample_matrix[neighbour]    =  0.0;    
                    green_sample_matrix[neighbour]  =  0.0;
                    blue_sample_matrix[neighbour]   =  0.0;
                    alpha_sample_matrix[neighbour]  =  0.0;    
                }
            }
            double red_convoluted_value     = lanczos_point_convulution(red_sample_matrix);
            double green_convoluted_value   = lanczos_point_convulution(green_sample_matrix);   
            double blue_convoluted_value    = lanczos_point_convulution(blue_sample_matrix);
            double alpha_convoluted_value   = lanczos_point_convulution(alpha_sample_matrix);   

            red_convoluted_value    = (red_convoluted_value > 255)   ? 255 : (red_convoluted_value < 0)   ? 0 : red_convoluted_value;
            green_convoluted_value  = (green_convoluted_value > 255) ? 255 : (green_convoluted_value < 0) ? 0 : green_convoluted_value;
            blue_convoluted_value   = (blue_convoluted_value > 255)  ? 255 : (blue_convoluted_value < 0)  ? 0 : blue_convoluted_value;
            alpha_convoluted_value  = (alpha_convoluted_value > 255) ? 255 : (alpha_convoluted_value < 0) ? 0 : alpha_convoluted_value;
            
            scaled_picture[row * width + col].red   = (uint8_t) red_convoluted_value;
            scaled_picture[row * width + col].green = (uint8_t) green_convoluted_value;
            scaled_picture[row * width + col].blue  = (uint8_t) blue_convoluted_value;
            scaled_picture[row * width + col].alpha = (uint8_t) alpha_convoluted_value;
        }
    }
    return scaled_picture;
}
