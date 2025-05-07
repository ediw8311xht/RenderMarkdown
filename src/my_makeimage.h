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
// using Magick::StyleType;
using Magick::Image;
using Magick::Geometry;
struct TextData {
    double  font_size   = 12;
    _s      font        = "Noto-Sans";
    Color   fg          = Color("black");
    Color   bg          = Color("transparent");
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
        size_t   offset_y;
        size_t   offset_x;
        size_t   padding_y;
        static const int line_spacing = 30;

        Image image_from_data(_s text, TextData t);

        void write_image(Image& img);
    public:
        MakeImage(size_t x=500, size_t y=500, Color canvas_bg="white", int offset_y=0, int offset_x=0 );
        static void initialize(char* arg);
        void write_text(_s text, TextData t);

        void reset_image(int offset_y=0);

        void save_image(const _s& filename);
};

}
