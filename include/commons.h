#ifndef COMMONS_H
#define COMMONS_H

#include<stdint.h>
#include <wchar.h>

#define PNG_HEADER_SIZE 8

/**
 * struct Pixel Represents a single pixel with RGBA color components.
 * - red:   Red component (0–255).
 * - green: Green component (0–255).
 * - blue:  Blue component (0–255).
 * - alpha: Alpha (transparency) component (0–255), where 0 is fully transparent and 255 is fully opaque.
 *
 * This struct models a pixel using 8-bit per channel RGBA format.
 */
typedef struct Pixel{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} Pixel;

/*
 * AsciiImageObject represents an image and its ASCII conversion state.
 * -height:          Image height in pixels.
 * -width:           Image width in pixels.
 * -scale:           Scaling factor applied before asciifing process
 * -original_image:  Pointer to the original pixel data (RGBA).
 * -edited_image:    Pointer to the last version of the modified image.
 * -ascii_image:     Pointer to the ASCII art representation (wchar_t array).
 *
 * This struct encapsulates both the source image and any derived
 * representations, enabling the program to keep original and transformed
 * data together.
 */
typedef struct AsciiImageObject{
	int height, width;
	int scale;
	Pixel* original_image;
	Pixel* edited_image;
	wchar_t* ascii_image;
} AsciiImageObject;

#endif 
