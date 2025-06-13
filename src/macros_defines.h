#pragma once

#include <string>
#include <iostream>

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
#define DRAWN_LINE_SIZE      4
#define DRAWN_LINE_MARGIN    2
#define STYLE_INLINE_CODE " font_family='Noto-Mono' fgcolor='#00FF00' bgcolor='#000000' "
#define STYLE_INLINE_LINK " font_family='Noto-Sans' fgcolor='#0044FF' "


using _s = std::string;
