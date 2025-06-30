#pragma once
#include "macros_defines.h"
#include <string>
#include <map>
#include <Magick++.h>

namespace MakeImageNS {

typedef struct TextData TextData;
typedef struct MdSettings MdSettings;
using Magick::Color;
using Magick::ColorRGB;
using Magick::Image;
using Magick::Geometry;
// using Magick::StyleType;

using _stype = std::string::size_type;

// using TT = enum TokenType;
typedef enum TokenType {
    NONE   =-1,
    CODE   = 0,
    H1=1, H2=2, H3=3, H4=4, H5=5,
    INLINE = 6,
    IMAGE  = 7,
    END    = 8,
} TT;

// Store text settings
struct ImageData {
    _s   img_file;
    _s   alt_text = "";
    Geometry size {0, 0}; // Default {0, 0} for size from image.
};

struct TextData {
    double   font_size   = 12;
    // Font is dependent on what is available on system.
    _s       font        = "Noto-Sans"; 
    ColorRGB fg          = ColorRGB(0, 0, 0);
    Color    bg          = Color("transparent");
    bool     wrap        = false;
};

struct MdSettings {
    size_t width      = DEFAULT_WIDTH;
    size_t height     = DEFAULT_HEIGHT;
    Color canvas_bg   = DEFAULT_CANVAS_BG;
    ssize_t padding   = DEFAULT_PADDING;
    int line_spacing  = DEFAULT_LINE_SPACING;
    size_t sub_width() {
        return width - (padding * 2);
    }
    const std::map< const TT, const TextData > text_map = DEFAULT_TEXT_MAP;
    MdSettings() = default;
    // MdSettings(size_t width, size_t height, ssize_t padding=DEFAULT_PADDING) : width(width), height(height), sub_width(width-(padding*2)) {}
};

/* |---------------------------------------------------------|
   |  MakeImage (notes)                                      |
   | `Image::rows()` is height, `Image::columns()` is width  |
   |---------------------------------------------------------|
   | Text is rendered with pango for styling                 |
   | (e.g., `regular <b>bold</b>, <i>italic</i>`)            |
   |---------------------------------------------------------| */
class MakeImage {
    private:
        /* ------------------- VARS --------------------- */
        Image    canvas;
        Geometry canvas_size;
        ssize_t  offset_y;
        MdSettings settings;

        Image image_from_data(_s text, const TextData& t);
        Image image_from_data_unwrapped(_s text, const TextData& t);

        void write_image(Image& img);
    public:
        static void send_data(const _s& s, _stype i = 0, _stype size=0, bool start=true);
        static void setup_magick(char* arg);
        static double get_height(Image& img, const _s& text);
        MakeImage(MdSettings& s);
        /* Not using const for Image, because it causes issues.
           Maybe reason why have to create new image since I am passing by
           reference And FontMetrics is modifying the Image in some way */
        void add_line_to_canvas();
        void add_text_to_canvas(_s text, const TextData& t);
        void add_text_to_canvas(_s text, TT token);
        void add_image_to_canvas(const ImageData&);
        void reset_image();
        void save_image(const _s& filename);
        /* More information here: https://sw.kovidgoyal.net/kitty/graphics-protocol/ */
        void display_image_kitty();
};

}
