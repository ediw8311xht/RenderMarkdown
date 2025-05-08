#include "my_makeimage.h"


using namespace MakeImageNS;

namespace Mag = Magick;
Image MakeImage::image_from_data(_s text, TextData t) {
    Image new_img(subimg_geo, t.bg);
    new_img.fontFamily(t.font);
    new_img.fontPointsize(t.font_size);
    new_img.fillColor(t.fg);
    // `pango:` allowing pango formatting wwth `<b>`, `<i>`, etc.
    new_img.read("pango:" + text);
    return new_img;
}

void MakeImage::write_image(Image& img) {
    canvas.composite(img, offset_x, offset_y, Mag::OverCompositeOp);
    // Update offset to adjust for written image
    offset_y += img.rows() + 30;
}

MakeImage::MakeImage(size_t x, size_t y, Color canvas_bg, ssize_t offset_y, ssize_t offset_x)
    : canvas_size(x, y)     , subimg_geo(x, 0)      ,
      canvas_bg(canvas_bg)  , offset_y(offset_y)    ,
      offset_x(offset_x)
{
  canvas = Image({x, y}, canvas_bg);
}

void MakeImage::initialize(char* arg) {
    // Must be called before making image objects
    // Pass first argument (path where program is being run)
    // See: http://www.graphicsmagick.org/Magick++/
    Mag::InitializeMagick(arg);
}
void MakeImage::write_text(_s text, TextData t) {
    Image img = image_from_data(text, t);
    write_image(img);
}

void MakeImage::reset_image(size_t offset_y) {
    canvas.erase();
    this->offset_y = offset_y;
}

void MakeImage::save_image(const _s& filename) {
    canvas.write(filename);
}

