#include <iostream>
#include <Magick++.h>
#include "parse_markdown.h"
#include "test.h"
#include "my_makeimage.h"
#include <filesystem>
#include <vector>
#define HELP_STRING \
 "RenderMarkdown: [options] [input-file] [output-file]\n" \
 "\tOptions \n" \
 "\t\t--help    - print help \n" \
 "\t\t--test    - run tests  \n" \
 "\t\t--display - display    \n" \
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
using TestRenderMarkdownNS::TestRenderMarkdown;

using std::filesystem::exists;
using std::cout;
using std::endl;
using std::string;
using std::format;
using std::vector;

typedef struct ProgArgs {
    vector<string> input_files = {};
    // For terminal output use "-"
    string output_file = "";
    size_t img_width   = 800;
    size_t img_height  = 1000;
} ProgArgs;

// |-------------------------------------|
// | exit_TYPE: Exit points for program. |
// |-------------------------------------|
void exit_error(bool f, string s, int exit_code=1) {
    if (f) { std::cerr << s << endl; exit(exit_code); }
}
void exit_help(int exit_code=0) {
    cout << HELP_STRING << endl;
    exit(exit_code);
}
void exit_tests() {
    TestRenderMarkdown a;
    a.run_all();
    exit(0);
}

void exit_run(ProgArgs pa) {
    ParseMarkdown f(pa.input_files);
    f.make_image(pa.output_file, pa.img_width, pa.img_height);
    exit(0);
}

// |-------------------------------------|
// | Handles options and arguments.      |
// |-------------------------------------|
void handle_options(ProgArgs& pa, string s) {
    if      (s == "--test")    {  exit_tests();  }   
    else if (s == "--help")    {  exit_help(); }
    else if (s == "--display") {  pa.output_file = "-"; }
    else {
        exit_error(true, format("Unknown Option: '{}'.", s), 1);
    }
}
void handle_args(vector<string>& arguments) {
    
    ProgArgs pa;
    auto i = arguments.begin();
    auto e = arguments.end();
    for (; i < e; i++) {
        if ( i->starts_with("--") ) { handle_options(pa, *i); }
        else if (pa.output_file != "-" && i == (e-1)) {
            exit_error( exists(*i), format("Ouput file '{}' already exists.", *i), 2);
            pa.output_file = *i;
        }
        else {
            exit_error( !exists(*i), format("Input file '{}' doesn't exists.", *i), 2);
            pa.input_files.push_back(*i);
        }
    }
    exit_error( pa.input_files.size() <= 0, "No input files provided.", 1);
    exit_run(pa);
}

int main(int argc, char** argv)
{
    // This must be called first
    MakeImageNS::MakeImage::initialize(*argv);
    vector<string> arguments = {};
    if (argc <= 1) { exit_help(); }
    for (int i = 1; i < argc; i++) { arguments.push_back(string(argv[i])); }
    handle_args(arguments);
    return 0;
}

