#include "my_makeimage.h"
#include <cmath> // for std::ceil

using namespace MakeImageNS;

namespace Mag = Magick;
using Magick::TypeMetric;

double MakeImage::get_height(Image& img, const _s& text) {
    TypeMetric g;
    img.fontTypeMetricsMultiline(text, &g);
    return g.textHeight();
}

Image MakeImage::image_from_data_unwrapped(_s text, TextData t) {
    Image check_image({1, 1}, t.bg);
    check_image.font(t.font);
    check_image.fontPointsize(t.font_size);
    check_image.fillColor(t.fg);
    check_image.textAntiAlias(true);
    size_t text_height = std::ceil(get_height(check_image, text));
    Image new_img({subimg_geo.width(), text_height}, t.bg);
    new_img.font(t.font);
    new_img.fontPointsize(t.font_size);
    new_img.fillColor(t.fg);
    new_img.textAntiAlias(true);
    new_img.backgroundColor(t.bg);
    new_img.read("label:" + text);
    return new_img;
}
Image MakeImage::image_from_data(_s text, TextData t ) {
    Image new_img(subimg_geo, t.bg);
    new_img.font(t.font);
    new_img.fontPointsize(t.font_size);
    new_img.fillColor(t.fg);
    new_img.textAntiAlias(true);
    new_img.read("pango:" + text); 
    return new_img;
}

void MakeImage::write_image(Image& img) {
    canvas.composite(img, 0, offset_y, Mag::OverCompositeOp);
    // Update offset to adjust for written image
    offset_y += img.rows() + line_spacing;
}

MakeImage::MakeImage(size_t width, size_t height, Color canvas_bg, size_t padding )
    : canvas_size(width, height)     , subimg_geo(width-(padding*2), 0)      ,
      canvas_bg(canvas_bg)  , offset_y(padding)
{
  canvas = Image({width, height}, canvas_bg);
}

void MakeImage::initialize(char* arg) {
    // Must be called before making image objects
    // Pass first argument (path where program is being run)
    // See: http://www.graphicsmagick.org/Magick++/
    Mag::InitializeMagick(arg);
}
void MakeImage::write_text(_s text, TextData t) {
    Image img = t.wrap ? image_from_data(text, t) : image_from_data_unwrapped(text, t);
    write_image(img);
}

void MakeImage::reset_image(size_t offset_y) {
    canvas.erase();
    this->offset_y = offset_y;
}

void MakeImage::save_image(const _s& filename) {
    canvas.write(filename);
}

