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

#include <stdio.h>
#include <locale.h>
#include <png.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "commons.h"
#include "lanczos.h"
#include "asciifier.h" 

/*
 * array_mapping Computes the linear index in a 1D array from 2D matrix coordinates.
 * - row :  The row index of the element.
 * - col :  The column index of the element.
 * - cols:  The total number of columns in the matrix.
 *
 * This function implements the standard row-major bijection from N^2 to N,
 * mapping a finite 2D matrix into a 1D array.
 *
 * Returns: The linear index corresponding to the (row, col) coordinates.
 */
static inline int array_mapping(int row, int col, int cols){
	return (row * cols) + col;
}

/* 
 * input_validator attempts to validate a file as a PNG image.
 *
 * Parameters:
 * - input_path: a char* representing the path to the file to validate.
 *
 * The function performs the following steps:
 * - Opens the file in binary read mode ("rb")
 * - Reads the first 8 bytes into file_header
 * - Uses png_sig_cmp (from libpng) to check if the header matches a valid PNG signature
 *
 * Returns:
 * - A FILE* if the file is a valid PNG and was successfully opened and read
 * - NULL if the file cannot be opened, read, or fails the PNG signature check
 *
 * On error, an appropriate message is printed to stderr.
 * On successful validation, nothing is printed.
 */
FILE* input_validator(char* input_path){
	unsigned char file_header[PNG_HEADER_SIZE];	//first 8 byte of file in path
	FILE* file_ptr;
	size_t read_bytes;

	file_ptr = fopen(input_path, "rb");
	if(!file_ptr){
		fprintf(stderr, "Error while reading file");
		return NULL;
	}

	read_bytes = fread(file_header, 1, PNG_HEADER_SIZE, file_ptr);	// Read PNG_HEADER_SIZE bytes, one at a time (size = 1), and write them into file_header
	if (read_bytes != 8) {
		fprintf(stderr, "Failed to read PNG header\n");
		fclose(file_ptr);
		return NULL;	
    	}

	if (png_sig_cmp(file_header, 0, 8)) {
		fprintf(stderr, "File is not a valid PNG\n");
		fclose(file_ptr);
		return NULL;
	}

	return file_ptr;
}


/*
 *image_struct_init initializes an AsciiImageObject from PNG image data.
 * - width:      The width of the PNG image in pixels.
 * - height:     The height of the PNG image in pixels.
 * - png_ptr:    A pointer to the libpng read struct.
 * - info_ptr:   A pointer to the libpng info struct.
 * 
 * This function:
 * - Allocates memory for an AsciiImageObject structure.
 * - Normalizes the PNG image data to 8-bit RGBA format using libpng transformations.
 * - Reads the pixel data into the `original_image` field as an array of `Pixel` structs.
 * - Leaves the `edited_image` field initialized to NULL for future use.
 * 
 * The function assumes the PNG has been properly opened and validated before calling.
 * Returns: A pointer to a fully initialized AsciiImageObject on success.
 * Exits the program with EXIT_FAILURE on memory allocation failure.
 */
AsciiImageObject* image_struct_init(int width, int height, png_structp png_ptr, png_infop info_ptr){
	AsciiImageObject* return_ptr;	
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	png_byte bit_depth  = png_get_bit_depth(png_ptr, info_ptr);
	int memory_size = width * height;

	//Allocating memory for AsciiImageObject struct
	return_ptr = (AsciiImageObject*) malloc(sizeof(AsciiImageObject));
	if(!return_ptr) exit(EXIT_FAILURE);

	return_ptr->original_image 	= (Pixel*) malloc(sizeof(Pixel)*memory_size);
	return_ptr->ascii_image     	= (wchar_t*)  malloc(sizeof(char)* memory_size);
	//if(!return_ptr->edited_image || !return_ptr->original_image) exit(EXIT_FAILURE);
		
	//Normalization all PNG format into a RGBA 8-bit
	if (bit_depth == 16) 					png_set_strip_16(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE)		png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) 	png_set_tRNS_to_alpha(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
	
	png_read_update_info(png_ptr, info_ptr);
	
	//SUB-ROUTINE: store RGBA values in AsciiImageObject.original_image
	png_bytep* row_pointers;
	return_ptr->edited_image 	= NULL;
	return_ptr->width 		= width;
	return_ptr->height		= height;
	return_ptr->scale 		= 1;
	int row_bytes 			= png_get_rowbytes(png_ptr, info_ptr);

	row_pointers = malloc(sizeof(png_bytep) * height);
	if (!row_pointers) exit(EXIT_FAILURE);
	
	for (int row = 0; row< height; row++) {
		row_pointers[row] = malloc(row_bytes);
		if (!row_pointers[row]) exit(EXIT_FAILURE);
	}

	png_read_image(png_ptr, row_pointers);

	for(int row = 0; row < height; row++){
		for(int col = 0; col < width; col++){
			png_bytep row_data = row_pointers[row];
			png_bytep pixel = row_data + (col * 4);
			int idx = array_mapping(row, col, width);
			return_ptr->original_image[idx].red   = (uint8_t) pixel[0];
			return_ptr->original_image[idx].green = (uint8_t) pixel[1];
			return_ptr->original_image[idx].blue  = (uint8_t) pixel[2];
			return_ptr->original_image[idx].alpha = (uint8_t) pixel[3];
		}
	}

	for (int row = 0; row < height; row++) free(row_pointers[row]);
	free(row_pointers);

	return return_ptr;
}

/*
 * Global variable holding the currently selected rendering palette.
 * Initialized to BRAILLE by default; may be overridden via command-line options.
 */
Palette g_palette = BRAILLE;

uint8_t g_scale_factor = 1;

/*
 * args_parser parses command-line arguments and configures the program's
 * global settings.
 * - argc: number of arguments passed to main().
 * - argv: array of strings containing the arguments passed to main().
 *
 * Behavior:
 *  - Ensures that at least one file path is provided as a required argument.
 *  - Handles optional flags:
 *      - "-p" / "--palette": sets the rendering palette ('BRAILLE', 'BLOCK', 'DENSE', 'SMOOTH').
 *      - "-s" / "--scale": sets the scale factor as an integer.
 *  - Any unknown option or missing/invalid value causes the program
 *    to terminate immediately with an error message on stderr.
 *
 * Side effects:
 *  - Modifies the global variables 'g_palette' and 'g_scale_factor'
 *    according to the provided options.
 *  - Terminates the program with exit(EXIT_FAILURE) on invalid input.
 */
static inline void args_parser(int argc, char* argv[]){
	if(argc < 2){
		fprintf(stderr, "A single file path argument is required\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i = 2; i < argc; i++){
		char* arg = argv[i];

		if(strcmp(arg, "-p") == 0 || strcmp(arg, "--palette") == 0){	//Palette argument
			if(i+1>= argc){ //update before controll
				fprintf(stderr, "Missing value for option %s", arg);
				exit(EXIT_FAILURE);
			}

			char* palette = argv[++i];

			if(strcmp(palette, "br") == 0 || strcmp(palette, "braille") == 0) g_palette = BRAILLE; 
			else if(strcmp(palette, "bl") == 0 || strcmp(palette, "block") == 0) g_palette = BLOCK; 
			else if(strcmp(palette, "ds") == 0 || strcmp(palette, "dense") == 0) g_palette = DENSE; 
			else if(strcmp(palette, "sm") == 0 || strcmp(palette, "smoth") == 0) g_palette = SMOOTH; 
			else{
				fprintf(stderr, "Unknown palette: %s\n", palette);
				exit(EXIT_FAILURE);
			}	
		}else if(strcmp(arg, "-s") == 0 || strcmp(arg, "--scale") == 0){	//Scale argument
			if(i+1>= argc){ //update before controll
				fprintf(stderr, "Missing value for option %s", arg);
				exit(EXIT_FAILURE);
			}
			
			char* endptr;
			g_scale_factor = (int)strtol(argv[++i], &endptr, 10);
			
			if(*endptr != '\0'){
				fprintf(stderr, "Invalid scale factor %s", argv[i]);
				exit(EXIT_FAILURE);;
			}					
		}else{
			fprintf(stderr, "Unknown option: %s", arg);
			exit(EXIT_FAILURE);
		}		
	}
}

int main(int argc, char* argv[]){
	args_parser(argc, argv);

	FILE* file_ptr = input_validator(argv[1]);
	if(!file_ptr){
		printf("Error during the file reading");
		exit(EXIT_FAILURE);
	}

	// SUB-ROUTINE: Create libpng structures
	int width, height;
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);

	// SUB-ROUTINE: Register libpng error handler and recover on failure
	if (setjmp(png_jmpbuf(png_ptr))){
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(file_ptr);
		fprintf(stderr, "Error occurred while processing file\n");
		exit(EXIT_FAILURE);
	}

	// SUB-ROUTINE: Read PNG header, metadata and populate structures
	AsciiImageObject* image_struct;
	png_init_io(png_ptr, file_ptr);		// associate file_ptr with png_ptr
	png_set_sig_bytes(png_ptr, 8);		// inform libpng that 8 bytes were already read
	png_read_info(png_ptr, info_ptr);	// read metadata into info_ptr

	width 	= png_get_image_width(png_ptr, info_ptr);
	height	= png_get_image_height(png_ptr, info_ptr);
	
	//TODO: refactor after cli command are completed
	image_struct = image_struct_init(width, height, png_ptr, info_ptr);
	image_struct->edited_image = lanczos_scale(image_struct, g_scale_factor);
	image_struct->ascii_image = asciify_image(image_struct->edited_image, height/g_scale_factor, width/g_scale_factor, g_palette);
	

	//Peak production code, jokes aside, it's just for testing until the interface is finished.
	setlocale(LC_CTYPE, "");
	fwide(stdout, 1);
	for(int row = 0; row < height/g_scale_factor; row++){
		for(int col = 0; col < width/g_scale_factor; col++){
			wchar_t current_char = image_struct->ascii_image[array_mapping(row, col, width/g_scale_factor)]; 
			wprintf(L"%lc", current_char);
		}
		wprintf(L"\n");
	}

	fclose(file_ptr);
	return EXIT_SUCCESS;
}
