#include <iostream>
#include <Magick++.h>
#include "parse_markdown.h"
#include "my_makeimage.h"
#include <filesystem>
#define HELP_STRING \
 "ParseMarkdown: [options] [input-file] [output-file]\n" \
 "\tOptions \n" \
 "\t\t--help    - print help \n" \
 "\t\t--xpos    - Not implemented \n" \
 "\t\t--xpos    - Not implemented \n" \
 "\t\t--width:  - Not implemented \n" \
 "\t\t--height  - Not implemented \n" \
 "\tArguments \n" \
 "\t\t[input-file]  : Markdown formatted input file.\n" \
 "\t\t[output-file] : (Image file to output) or ('-') to output to terminal (must have kitty).\n" \
 "\tExit Status \n" \
 "\t\t0\tGood \n" \
 "\t\t1\tArgument Error\n" \
 "\t\t2\tFile Error\n" \
 "\t\t3\tWrite Error\n"


using Magick::InitializeMagick;
using ParseMarkdownNS::ParseMarkdown;
// using MakeImageNS::MakeImage;
using namespace MakeImageNS;

using std::filesystem::exists;
using std::cout;
using std::endl;
using std::string;
using std::format;

void print_help(int exit_code=0) {
    cout << HELP_STRING << endl;
    exit(exit_code);
}

void handle_error(bool f, string s, int exit_code=1) {
    if (f) { std::cerr << s << endl; exit(exit_code); }
}
typedef struct ProgArgs {
    string input_file  = "";
    // For terminal output use "-"
    string output_file = "out.jpeg";
    size_t img_width   = 1000;
    size_t img_height  = 800;
} ProgArgs;

void handle_args(int argc, char** argv) {
    // if (argc == 0 || (argc >= 1 && string("--help") == argv[1]))  {
    //     print_help(); exit(0);
    // }
    // ProgArgs pa;
    // for (int i = 1; i < argc+1; i++) {
    //     switch (i) {
    //         case (1) :
    //             pa.input_file=string(argv[i]); break;
    //         case (2) :
    //             pa.output_file=string(argv[i]); break;
    //         default  : print_help(1);
    //     }
    // }
    // handle_error( !exists(pa.input_file),
    //         format("Input file '{}' doesn't exists.", pa.input_file), 2);
    // handle_error( pa.output_file != "-" && exists(pa.output_file),
    //         format("Output file '{}' already exists.", pa.output_file), 2);
    // ParseMarkdown f(pa.input_file);
    // f.make_image(pa.output_file, pa.img_width, pa.img_height);
}

int main(int argc, char** argv)
{
    InitializeMagick(*argv);
    //handle_args(argc-1, argv);
    string test_file = "/home/maceurt/Desktop/SCHOOL/COMP_SCI_II/final_proj/STATIC_FILES/test.md";
    string out_file = "/home/maceurt/Desktop/SCHOOL/COMP_SCI_II/final_proj/STATIC_FILES/out.jpg";
    ParseMarkdown f(test_file);
    f.make_image(out_file);

    return 0;
}

