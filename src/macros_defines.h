#pragma once

#include <string>
#include <iostream>
#include <optional>

// Defaults
#define DEFAULT_WIDTH           800
#define DEFAULT_HEIGHT          1000
#define DEFAULT_CANVAS_BG       "white";
#define DEFAULT_PADDING         5;
#define DEFAULT_CONFIG          ""
#define DEFAULT_LINE_SPACING    1

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


#define DRAWN_LINE_SIZE      4
#define DRAWN_LINE_MARGIN    2
#define STYLE_INLINE_CODE " font_family='Noto-Mono' fgcolor='#00FF00' bgcolor='#000000' "
#define STYLE_INLINE_LINK " font_family='Noto-Sans' fgcolor='#0044FF' "
// #define DEFAULT_CONFIG    getenv("XDG_CONFIG_HOME") ? "${XDG_CONFIG_HOME:-${HOME}/.config}/render_markdown/config.json"

using _s = std::string;
std::optional<_s> file_as_string(_s file_string);
