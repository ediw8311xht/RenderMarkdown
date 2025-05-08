#include <iostream>
#include <stdlib.h>
#include "parse_markdown.h"
#include "my_makeimage.h"
#include <filesystem>
#define HELP_STRING \
 "ParseMarkdown: [options] [input-file] [output-file]\n" \
 "    [options]     : --help    - print help \n" \
 "                    --xpos    - Not implemented \n" \
 "                    --xpos    - Not implemented \n" \
 "                    --width:  - Not implemented \n" \
 "                    --height  - Not implemented \n" \
 "    [input-file]  : Markdown formatted input file.\n" \
 "    [output-file] : (Image file to output) or ('-') to output to terminal (must have kitty).\n"



using ParseMarkdownNS::ParseMarkdown;
using MakeImageNS::MakeImage;

using std::cout;
using std::endl;
using std::strcmp;
using std::string;

void print_help(int exit_code=0) {
    cout << HELP_STRING << endl;
    exit(exit_code);
}

typedef struct ProgArgs {
    string input_file  = "";
    // For terminal output use "-"
    string output_file = "out.jpeg";
    size_t img_width   = 1000;
    size_t img_height  = 800;
} ProgArgs;

bool comp(char* s, const char* e) {
    while (*s != '\0' && *e != '\0' ) {
        if (*s != *e) { return false; }
        s++; e++;
    }
    return true;
}
void handle_args(int argc, char** argv) {
    using std::filesystem::exists;
    cout << argv[1] << endl;
    if (argc >= 2 && comp(argv[1], "--help"))  {
        cout << argv[1] << endl;
        print_help(); exit(0);
    }
    ProgArgs pa;
    for (int i = 1; i < argc; i++) {
        switch (i) {
            case (1) : pa.input_file=argv[i]; break;
            case (2) : pa.output_file=argv[i]; break;
            default  : print_help(1);
        }
    }
}

int main(int argc, char** argv)
{
    cout << argc << endl;
    MakeImage::initialize(*argv);
    handle_args(argc, argv);
    return 0;
}

