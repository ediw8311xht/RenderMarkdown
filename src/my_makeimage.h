#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <Magick++.h>

namespace MakeImageNS {


typedef struct TextData TextData;
using _s    = std::string;
using Magick::Color;
using Magick::ColorRGB;
// using Magick::StyleType;
using Magick::Image;
using Magick::Geometry;
struct TextData {
    double   font_size   = 12;
    _s       font        = "Noto-Sans";
    ColorRGB fg          = ColorRGB(0, 0, 0);
    Color    bg          = Color("transparent");
    bool     wrap        = false;
};

//|--------------------------------------------------------------------------------------|
//|                              Notes:                                                  |
//|--------------------------------------------------------------------------------------|
//| `Image::rows()` is height, `Image::columns()` is width                               |
//|--------------------------------------------------------------------------------------|
//| Text is rendered with pango for styling (e.g., `regular <b>bold</b> <i>italic</i>`)  |
//|--------------------------------------------------------------------------------------|
class MakeImage {
    private:
        Image    canvas;
        Geometry canvas_size;
        // `subimg_geo`:
        // Width is same as `canvas`, height is 0 so it will be automatically
        // update from written data. This allows getting the height of the
        // written data for `offset_y`.
        Geometry subimg_geo;
        Color    canvas_bg;
        ssize_t   offset_y;
        ssize_t    padding;
        static const int line_spacing = 1;

        Image image_from_data(_s text, TextData t);
        Image image_from_data_unwrapped(_s text, TextData t);

        void write_image(Image& img);
    public:
        MakeImage(size_t width=500, size_t height=500, Color canvas_bg="white",  ssize_t padding=5);
        static void initialize(char* arg);
        double get_height(Image& img, const _s& text);
        void write_text(_s text, TextData t);
        void reset_image(size_t offset_y=0);
        void save_image(const _s& filename);
};

}
