#include "render_markdown.h"

using RenderMarkdownNS::RenderMarkdown;
int main(int argc, char** argv) {
    RenderMarkdown r;
    r.run_program(argc, argv);
    return 0;
}
