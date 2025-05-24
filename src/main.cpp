#include "render_markdown.h"

using RenderMarkdownNS::RenderMarkdown;
int main(int argc, char** argv) {
    RenderMarkdown r;
    r.get_options(argc, argv);
    r.run_program();
    return 0;
}
