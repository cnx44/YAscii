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
#include <wchar.h>
#include "asciifier.h"

/*
 * ascii_palettes array of wide-character strings representing symbol sets
 *                used to map greyscale values to ASCII art.
 *
 * Each string in the array is a complete palette 
 *
 * Declared as const to prevent accidental modification and stored as
 * wide-character literals (L"...") to support Unicode Braille, block, and
 * other extended symbols. The palette selection is determined at runtime
 * based on the active Palette enum value.
 */
const wchar_t* ascii_palettes[] = { 
	L"⣿⣿⣾⣷⣧⣇⣃⣂⡇⡃⡂⠇⠆⠄⠂⠁⠀",
	L"  ░▒▓█",
	L" .:-=+*#%@",
	L" .,:;+*?0S#@",
};

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
 * asciify_image converts an image to an ASCII representation using a given palette.
 * -image:    Pointer to the array of pixels (input image data).
 * -height:   Number of rows in the image.
 * -width:    Number of columns in the image.
 * -palette:  Palette enum value specifying which character set to use for mapping.
 *
 * This function maps each pixel's greyscale value to a corresponding
 * character from the selected wide-character palette in 'ascii_palettes'.
 * The output is a buffer of wchar_t containing one character per pixel.
 *
 * Returns: Pointer to a newly allocated buffer on success, or NULL on failure.
 *          The caller is responsible for freeing the returned buffer.
 */
wchar_t* asciify_image(Pixel* image, int height, int width, Palette palette){
	size_t byte = (size_t) height * (size_t) width * sizeof(wchar_t); 
	const wchar_t* palette_string = ascii_palettes[palette];
	int palette_size = (int) wcslen(palette_string);
	wchar_t* asciified_image = malloc(byte);

	if(!asciified_image) return NULL;

	for(int row = 0; row < height; row++){
		for(int col = 0; col < width; col++){
			double pixel_grey_level = greyscale_converter(image[row * width + col]);
			pixel_grey_level = (pixel_grey_level < 0) ? 0 : (pixel_grey_level > 1) ? 1 : pixel_grey_level;
			int palette_index = (int) (pixel_grey_level * (double)(palette_size - 1));
			asciified_image[row * width + col] = palette_string[palette_index];
		}
	}

	return asciified_image;
}
