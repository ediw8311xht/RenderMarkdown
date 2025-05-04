#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <Magick++.h>
// #include <opencv2/opencv.hpp>
// using namespace cv;

namespace MakeImageNS {


typedef struct TextData TextData;
using _tp_i = std::tuple<int, int>;
using _s    = std::string;
using Magick::Color;
using Magick::StyleType;
using Magick::Image;
using Magick::Geometry;
namespace Mag = Magick;

// struct TextData {
//     Color     stroke_color;
//     Color       fill_color;
//     double           width;
//     bool        anti_alias;
//     const _s&         font;
// };

struct TextData {
    _s      text;
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
        Image image_from_data(TextData t) {
            Image new_img(subimg_geo, t.bg);
            new_img.fontPointsize(t.font_size);
            new_img.fillColor(t.fg);
            // `pango:` allowing pango formatting wwth `<b>`, `<i>`, etc.
            new_img.read("pango:" + t.text);
            return new_img;
        }
        void write_image(Image& img) {
            canvas.composite(img, offset_x, offset_y, Mag::OverCompositeOp);
            // Update offset to adjust for written image
            offset_y += img.rows() + padding_y;
        }
    public:
        MakeImage(size_t x=500, size_t y=500, Color canvas_bg="white", int offset_y=0, int offset_x=0 )
            : canvas_size(x, y)     ,
              subimg_geo(x, 0)      ,
              canvas_bg(canvas_bg)  ,
              offset_y(offset_y)    ,
              offset_x(offset_x)    ,
              canvas({x, y}, canvas_bg) {}
        static void initialize(char* arg) {
            // Must be called before making image objects
            // Pass first argument (path where program is being run)
            // See: http://www.graphicsmagick.org/Magick++/
            Mag::InitializeMagick(arg);
        }
        void write_text(TextData t) {
            Image img = image_from_data(t);
        }
        void reset_image(int offset_y=0) {
            canvas.erase();
            this->offset_y = offset_y;
        }
        void save_image(const _s& filename) {
            canvas.write(filename);
        }
};

}
