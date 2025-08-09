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

#ifndef ASCIIFIER_H
#define ASCIIFIER_H

#include <wchar.h>
#include "commons.h"

/*
 * Conversion constants for greyscale calculation (Rec. 709 luminance weights).
 *
 * These values represent the relative contribution of each color channel
 * (red, green, blue) to perceived brightness in a linear color space.
 *
 * Source: ITU-R BT.709 standard.
 */
#define RED_CONVERSION_CONSTANT 	0.2126
#define GREEN_CONVERSION_CONSTANT 	0.7152
#define BLUE_CONVERSION_CONSTANT 	0.0722

/*
 * ascii_palettes wide-character string representing the set of symbols used to map greyscale values to ASCII art.
 *
 * Each character in the string corresponds to a different brightness level,
 * starting from the darkest symbol at index 0 to the brightest at the end.
 *
 * Declared as const to prevent accidental modification and stored as a
 * wide-character literal (L"") to support Unicode Braille, Legacy and block symbols.
 */
extern const wchar_t* ascii_palettes; 

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
wchar_t* asciify_image(Pixel* image, int height, int width);

#endif 
