#ifndef ASCIIFIER_H
#define ASCIIFIER_H

#include "commons.h"

#define RED_CONVERSION_CONSTANT 	0.2126
#define GREEN_CONVERSION_CONSTANT 	0.7152
#define BLUE_CONVERSION_CONSTANT 	0.0722

extern const char* ascii_palettes; 

void asciifier(Pixel* image, int height, int width);

#endif 
