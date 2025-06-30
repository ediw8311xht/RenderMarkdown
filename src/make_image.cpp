#include "macros_defines.h"
#include "make_image.h"
#include <iostream>
#include <cmath> // for std::ceil

#define P(A) std::cout << A
namespace {
    // Helpful macro
    const int CHUNK_SIZE = 4096;
    // Begins each chunk
    const _s CHUNK_SEP = "\e_G";
    // Ends each chunk
    const _s CHUNK_END = "\e\\";
    // a=T automatically sets position and size of image to display f=100 is for PNG
    const _s DISPLAY_SETTING = "a=T,f=100,";
    using Magick::TypeMetric;
    using Magick::Blob;
}
namespace MakeImageNS {
namespace Mag = Magick;

MakeImage::MakeImage(MdSettings& s) : settings(s)
    // : canvas_size(width, height),
    //   subimg_geo(width-(padding*2), 0),
    //   canvas_bg(canvas_bg),
    //   offset_y(padding),
    //   padding(padding),
    //   line_spacing(line_spacing)
{
    // Initialize canvas
    offset_y = settings.padding;
    canvas = Image({settings.width, settings.height}, settings.canvas_bg);
    // Using density for base image ONLY
    canvas.density({300, 300});
    canvas.resolutionUnits(Mag::PixelsPerInchResolution);
}

void MakeImage::setup_magick(char* arg) {
    /* Must be called before making image objects
       Pass first argument (path where program is being run)
       See: http://www.graphicsmagick.org/Magick++/ */
    Mag::InitializeMagick(arg);
    // Weird how this isn't mentioned in documentation???
    // Deallocates dynamically assigned memory used by magick
    // Maybe TerminateMagick is for C only (?)
    // std::atexit(Mag::TerminateMagick);
}
/* Get height using Magick::TypeMetric
   Image Geometry doesn't matter, but settings for image like fontPointsize
   must be set This Magick::TypeMetric also shows other information such as the
   width the text would be when rendered. DOESN'T take into account of height
   with text wrapping (such as with "caption:" or "pango:). */
double MakeImage::get_height(Image& img, const _s& text) {
    TypeMetric g;
    img.fontTypeMetricsMultiline(text, &g);
    return g.textHeight();
}

/*  Make Image with the background fit to the text. For: non-wrapped text
    ("label:") Method: Magick::TypeMetric Note: Not using Density - Text to
    Large Nor PixelsPerInchResolution - Error when creating image/calculating
    height. */
Image MakeImage::image_from_data_unwrapped(_s text, const TextData& t) {
    Image check_image({1, 1}, t.bg);
    check_image.font(t.font);
    check_image.fontPointsize(t.font_size);
    check_image.fillColor(t.fg);
    check_image.textAntiAlias(true);
    // Height of text.
    size_t text_height = std::ceil(get_height(check_image, text));
    // Resizing & drawing text on resized image causes issues. So making new
    // image.
    Image new_img({settings.sub_width(), text_height}, t.bg);
    new_img.font(t.font);
    new_img.fontPointsize(t.font_size);
    new_img.fillColor(t.fg);
    new_img.textAntiAlias(true);
    new_img.backgroundColor(t.bg);
    new_img.read("label:" + text);
    return new_img;
}
/* Make Image with the background fit to the text. For: wrapped text
   ("caption:" or "pango:") Method: Sets height to 0 so the image will
   automatically adjust to fit text. Note: Ensure text wrapping the width is
   set to the size of the canvas (minus padding on both side) */
Image MakeImage::image_from_data(_s text, const TextData& t ) {
    Image new_img(Geometry(settings.sub_width(), 0), t.bg);
    new_img.resolutionUnits(Mag::PixelsPerInchResolution);
    new_img.font(t.font);
    new_img.fontPointsize(t.font_size);
    new_img.fillColor(t.fg);
    new_img.textAntiAlias(true);
    new_img.read("pango:" + text); 
    return new_img;
}

void MakeImage::write_image(Image& img) {
    canvas.composite(img, settings.padding, this->offset_y, Mag::OverCompositeOp);
    // Update offset to adjust for written image
    // img.rows() is  the height of the image.
    this->offset_y += img.rows() + settings.line_spacing;
}

void MakeImage::reset_image() {
    canvas.erase();
    this->offset_y = settings.padding;
}

void MakeImage::add_text_to_canvas(_s text, const TextData& t) {
    Image sub_img = t.wrap ? image_from_data(text, t) : image_from_data_unwrapped(text, t);
    write_image(sub_img);
}
void MakeImage::add_text_to_canvas(_s text, TT token) {
    add_text_to_canvas(text, settings.text_map.at(token));
}

void MakeImage::add_image_to_canvas(const ImageData& i) {
    Image sub_img;
    try {
        sub_img.read(i.img_file);
    // https://www.imagemagick.org/Magick++/Exception.html
    } catch(Magick::ErrorFileOpen& e) {
        std::cerr << "Error Opening Image: '" << i.img_file << "'" << std::endl;
        std::cerr << "\t" << e.what() << std::endl;
        return;
    } catch (Magick::Exception&  e) {
        std::cerr << "Error Image: " << e.what() << std::endl;
    }
    if (i.size.width() != 0 || i.size.height() != 0) {
        // Setting width/height to 0 will keep aspect ratio when resizing
        sub_img.resize(i.size);
    }
    else {
        sub_img.resize(Geometry(settings.sub_width(), 0));
    }
    write_image(sub_img);
}

void MakeImage::save_image(const _s& filename) {
    canvas.write(filename);
}

void MakeImage::add_line_to_canvas() {
    canvas.strokeColor("black");
    canvas.fillColor("black");
    canvas.strokeWidth(DRAWN_LINE_SIZE);
    canvas.draw(Magick::DrawableLine(settings.padding, offset_y + DRAWN_LINE_MARGIN, settings.sub_width(), offset_y));
    offset_y += DRAWN_LINE_SIZE + DRAWN_LINE_MARGIN + settings.line_spacing;
}

/* send_data (const, _stype, _stype, bool)
   This chunks data in to 4096 chars. Start sending streamed data Data is sent
   straight to terminal using escape sequences which are interpreted by Kitty.
   Data must be chunked in 4096 chars */
void MakeImage::send_data(const _s& s, _stype i, _stype size, bool start) {
    P(CHUNK_SEP);
    if (start) {
        P(DISPLAY_SETTING);
        size = s.length();
    }
    // If not full chunk no m=1
    _stype ni = i + CHUNK_SIZE;
    P( (ni <= size ? "m=1;" : ";") );
    P(s.substr(i, CHUNK_SIZE));
    P(CHUNK_END);
    return ni < size ? send_data(s, ni, size, false) : ( P("\n") , void() );
}
/* display_image_kitty() [display to terminal] */
void MakeImage::display_image_kitty() {
    Blob b;
    /* We need to PNG this format for matching Kitty Graphics Protocol Specs
       Note: KGP supports sending rgb/rgba data as well (more difficult tbh) */
    canvas.magick("PNG");
    canvas.write(&b);
    _s s = b.base64(); // Needs to be base64 encoded
    send_data(s);
}
}
