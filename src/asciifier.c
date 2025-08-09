#include <stdlib.h>
#include <wchar.h>
#include "asciifier.h"

/*
 * ascii_palettes wide-character string representing the set of symbols used to map greyscale values to ASCII art.
 *
 * Each character in the string corresponds to a different brightness level,
 * starting from the darkest symbol at index 0 to the brightest at the end.
 *
 * Declared as const to prevent accidental modification and stored as a
 * wide-character literal (L"") to support Unicode Braille, Legacy and block symbols.
 */
const wchar_t* ascii_palettes = L"⠀⠁⠂⠄⠆⠇⡂⡃⡇⣂⣃⣇⣧⣷⣾⣿⣿";
//const char* ascii_palettes = " .,:;+*?0S#@";

/*
 * greyscale_converter converts a pixel's RGB values to a greyscale intensity.
 * -pixel: Input Pixel struct containing red, green, blue and alpha channel values.
 *
 * Each color channel is normalized to the range [0, 1] by dividing by 255.
 * The normalized values are then weighted by predefined constants for red,
 * green, and blue, which reflect human perception sensitivity to each channel.
 *
 * Returns: greyscale intensity in the range [0, 1].
 */
static double greyscale_converter(Pixel pixel){
	double linear_red, linear_green, linear_blue;

	linear_red   = (double) pixel.red   / 255;
	linear_green = (double) pixel.green / 255;
	linear_blue  = (double) pixel.blue  / 255;

	return linear_red * RED_CONVERSION_CONSTANT + linear_green * GREEN_CONVERSION_CONSTANT + linear_blue * BLUE_CONVERSION_CONSTANT;
}

/*
 * asciify_image converts an image to an ASCII representation using a palette.
 * -image:   Pointer to the array of pixels (input image data).
 * -height:  Number of rows in the image.
 * -width:   Number of columns in the image.
 *
 * This function maps each pixel's greyscale value to a corresponding
 * character from the global wide-character palette 'ascii_palettes'.
 * The output is a buffer of wchar_t containing one character per pixel.
 *
 * Returns: pointer to a newly allocated buffer on success, or NULL on failure.
 */
wchar_t* asciify_image(Pixel* image, int height, int width){
	size_t byte = (size_t) height * (size_t) width * sizeof(wchar_t); 
	int palette_size = (int) wcslen(ascii_palettes);
	wchar_t* asciified_image = malloc(byte);

	if(!asciified_image) return NULL;

	for(int row = 0; row < height; row++){
		for(int col = 0; col < width; col++){
			double pixel_grey_level = greyscale_converter(image[row * width + col]);
			pixel_grey_level = (pixel_grey_level < 0) ? 0 : (pixel_grey_level > 1) ? 1 : pixel_grey_level;
			int palette_index = (int) pixel_grey_level * (double)(palette_size - 1);
			asciified_image[row * width + col] = ascii_palettes[palette_index];
		}
	}

	return asciified_image;
}
