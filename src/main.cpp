#include <iostream>
#include <stdlib.h>
#include "parse_markdown.h"
#include "my_makeimage.h"
#include <format>


using ParseMarkdownNS::ParseMarkdown;
using MakeImageNS::MakeImage;

using std::cout;
using std::endl;
using std::format;
using std::strcmp;

void print_help() {
}

void handle_args(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help")) { print_help(); exit(0); }
    }
}

int main(int argc, char** argv)
{
    MakeImage::initialize(*argv);
    handle_args(argc, argv);
    return 0;
}

