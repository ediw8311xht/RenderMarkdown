#include "render_markdown.h"
#include "my_make_image.h"

using RenderMarkdownNS::RenderMarkdown;
int main(int argc, char** argv) {
    MakeImageNS::MakeImage::initialize(*argv);
    RenderMarkdown r;
    r.get_options(argc, argv);
    r.run_program();
    return 0;
}
