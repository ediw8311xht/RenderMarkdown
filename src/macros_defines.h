#pragma once

#include <string>

using _s = std::string;
// Defaults
const size_t  DEFAULT_WIDTH        = 900;
const size_t  DEFAULT_HEIGHT       = 900;
const _s      DEFAULT_CANVAS_BG    = "white";
const ssize_t DEFAULT_PADDING      = 5;
const size_t  DEFAULT_LINE_SPACING = 1;
const double  DRAWN_LINE_SIZE      = 4;
const size_t  DRAWN_LINE_MARGIN    = 2;
const _s      STYLE_INLINE_CODE    = "font_family='Noto-Mono' fgcolor='#00FF00' bgcolor='#000000'";
const _s      STYLE_INLINE_LINK    = "font_family='Noto-Sans' fgcolor='#0044FF'";


// #define DEFAULT_CONFIG    getenv("XDG_CONFIG_HOME") ? "${XDG_CONFIG_HOME:-${HOME}/.config}/render_markdown/config.json"

// #include <optional>
// std::optional<_s> file_as_string(_s file_string);
#define DEFAULT_TEXT_MAP  \
    { \
            { TT::CODE,   TextData( 12, "Noto-Mono", ColorRGB(0,   255, 0) , ColorRGB(0, 0, 0),    false ) }, \
            { TT::H1,     TextData( 18, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, \
            { TT::H2,     TextData( 17, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, \
            { TT::H3,     TextData( 16, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, \
            { TT::H4,     TextData( 15, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, \
            { TT::H5,     TextData( 14, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, \
            { TT::INLINE, TextData( 12, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, \
    }

constexpr const char *defaultConfig = R"(
[canvas]
width = 800
height = 1000
bg = "white"
padding = 5
line_spacing = 1

[style]
drawn_line_size = 4
drawn_line_margin = 2
inline_code = "font_family='Noto-Mono' fgcolor='#00FF00' bgcolor='#000000'"
inline_link = "font_family='Noto-Sans' fgcolor='#0044FF'"

[style.text.code]
font_size = 12
font_family = "Noto-Mono"
)";
