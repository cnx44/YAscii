#include "lanczos.h"

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
