#pragma once

#include <string>

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 1000
#define ERROR_CODES \
    "   1  Argument Error\n" \
    "   2      File Error\n" \
    "   3     Write Error\n"
// Helpful macro
#define P(A) std::cout << A
// Begins each chunk
#define CHUNK_SEP "\e_G"
// Ends each chunk
#define CHUNK_END "\e\\"
// a=T automatically sets position and size of image to display f=100 is for PNG
#define DISPLAY_SETTING "a=T,f=100,"
#define BEGIN_DATA_SIZE 13

#define DEFAULT_LINE_SPACING 1

using _s = std::string;
