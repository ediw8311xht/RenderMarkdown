#pragma once
#include "macros_defines.h"
#include <string>
// #include <vector>
// #include <tuple>
#include <Magick++.h>

namespace MakeImageNS {


typedef struct TextData TextData;
typedef struct MdSettings MdSettings;
using Magick::Color;
using Magick::ColorRGB;
// using Magick::StyleType;
using Magick::Image;
using Magick::Geometry;

using _stype = std::string::size_type;

// Store text settings
struct TextData {
    double   font_size   = 12;
    // Font is dependent on what is available on system.
    _s       font        = "Noto-Sans"; 
    ColorRGB fg          = ColorRGB(0, 0, 0);
    Color    bg          = Color("transparent");
    bool     wrap        = false;
};

struct ImageData {
    _s   img_file;
    _s   alt_text = "";
    Geometry size {0, 0}; // Default {0, 0} for size from image.
};
struct MdSettings {
    size_t width      = DEFAULT_WIDTH;
    size_t height     = DEFAULT_HEIGHT;
    Color canvas_bg   = DEFAULT_CANVAS_BG;
    ssize_t padding   = DEFAULT_PADDING;
    int line_spacing  = DEFAULT_LINE_SPACING;
    size_t sub_width  = 700;
    // MdSettings(size_t width, size_t height) : width(width), height(height) {}
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
        const MdSettings settings;

        Image image_from_data(_s text, const TextData& t);
        Image image_from_data_unwrapped(_s text, const TextData& t);

        void write_image(Image& img);
    public:
        static void send_data(const _s& s, _stype i = 0, _stype size=0, bool start=true);
        static void setup_magick(char* arg);
        static double get_height(Image& img, const _s& text);
        MakeImage(const MdSettings& s);
        /* Not using const for Image, because it causes issues.
           Maybe reason why have to create new image since I am passing by
           reference And FontMetrics is modifying the Image in some way */
        void add_line_to_canvas();
        void add_text_to_canvas(_s text, const TextData& t);
        void add_image_to_canvas(const ImageData&);
        void reset_image();
        void save_image(const _s& filename);
        /* More information here: https://sw.kovidgoyal.net/kitty/graphics-protocol/ */
        void display_image_kitty();
};

}
