#include "render_markdown.h"
#include "make_image.h"

using RenderMarkdownNS::RenderMarkdown;
int main(int argc, char** argv) {
    MakeImageNS::MakeImage::setup_magick(*argv);
    RenderMarkdown r;
    r.get_options(argc, argv);
    r.get_config();
    r.run_program();
    return 0;
}
