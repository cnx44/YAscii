#include <stdio.h>
#include <string.h>
#include "asciifier.h"

const char* ascii_palettes = "@#S%?*+;:,. ";

static double greyscale_converter(Pixel pixel){
	double linear_red, linear_green, linear_blue;

	linear_red   = (double) pixel.red   / 255;
	linear_green = (double) pixel.green / 255;
	linear_blue  = (double) pixel.blue  / 255;

	return linear_red * RED_CONVERSION_CONSTANT + linear_green * GREEN_CONVERSION_CONSTANT + linear_blue * BLUE_CONVERSION_CONSTANT;
}

void asciifier(Pixel* image, int height, int width){
	int palette_size = strlen(ascii_palettes);

	for(int row = 0; row < height; row++){
		for(int col = 0; col < width; col++){
			double pixel_grey_level = greyscale_converter(image[row * width + col]);
			int palette_index = (int)(pixel_grey_level * (palette_size - 1));
			char current_char = ascii_palettes[palette_index];
			printf("%c", current_char);
		}
		printf("\n");
	}
}
