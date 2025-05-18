#include <iostream>
#include <boost/program_options.hpp>
#include "render_markdown.h"

using namespace RenderMarkdownNS;
using namespace boost::program_options;

namespace RenderMarkdownNS {
    using std::filesystem::exists;
    using std::format;
    using _s = std::string;
    // options_description opts.add_options() 
    //     ("help", "show help")
    //     ;
    //options_description RenderMarkdown::opts = options_description("HI");
    void RenderMarkdown::initialize_options() {
        bool dflag = false;
        this->flags.add_options()
            ( "help,h"        , bool_switch(&dflag), "print help"               )
            ( "test,t"        , bool_switch(&dflag), "run tests"                )
            ( "display,d"     , bool_switch(&dflag), "display"                  )
            ( "overwrite,o"   , bool_switch(&dflag), "overwrite output file"    )
            ( "input-file,i"  , value<_s>(),         "input file"               )
            ( "output-file,o" , value<_s>(),         "output file"              )
        ;
        this->posargs.add("input-file"  , 1);
        this->posargs.add("output-file" , 1);
    }
    void RenderMarkdown::get_options(int argc, char** argv) {
        try {
            store(
                command_line_parser(argc, argv).options(this->flags).positional(this->posargs).run(),
                this->opt_map
            );
            notify(opt_map);
        // Explicit with error type even though included in namespace.
        } catch (boost::program_options::error& e) {
            exit_error(true, format("{}: {}", "Error Handling Options/Arguments", e.what()));
        }
    }
    RenderMarkdown::RenderMarkdown() {
        initialize_options();
    }
    void RenderMarkdown::exit_error(bool f, _s s, int exit_code) const {
        if (f) { std::cerr << s << std::endl; exit(exit_code); }
    }
}
/*
// #include <iostream>
// #include <boost/program_options.hpp>
// #include <Magick++.h>
// #include "parse_markdown.h"
// #include "test.h"
// #include "my_make_image.h"
// #include <filesystem>
// #define HELP_STRING \
//  "RenderMarkdown: [options] [input-file] [output-file]\n" \
//  "\tOptions \n" \
//  "\t\t--help      - print help \n" \
//  "\t\t--test      - run tests  \n" \
//  "\t\t--display   - display    \n" \
//  "\t\t--overwrite - overwrite output file \n" \
//  "\tArguments \n" \
//  "\t\t[input-file]  : Markdown formatted input file.\n" \
//  "\t\t[output-file] : (Image file to output) or ('-') to output to terminal (must have kitty).\n" \
//  "\tExit Status \n" \
//  "\t\t0\tGood \n" \
//  "\t\t1\tArgument Error\n" \
//  "\t\t2\tFile Error\n" \
//  "\t\t3\tWrite Error\n"
//
// using ParseMarkdownNS::ParseMarkdown;
// using TestRenderMarkdownNS::TestRenderMarkdown;
//
// using std::cout;
// using std::endl;
//
//
// // |-------------------------------------|
// // | exit_TYPE: Exit points for program. |
// // |-------------------------------------|
// void exit_error(bool f, string s, int exit_code=1) {
//     if (f) { std::cerr << s << endl; exit(exit_code); }
// }
// void exit_help(int exit_code=0) {
//     cout << HELP_STRING << endl;
//     exit(exit_code);
// }
// void exit_tests() {
//     TestRenderMarkdown a;
//     a.run_all();
//     exit(0);
// }
//
// void exit_run(ProgArgs pa) {
//     ParseMarkdown f(pa.input_files);
//     f.make_image(pa.output_file, pa.img_width, pa.img_height);
//     exit(0);
// }
//
// // |-------------------------------------|
// // | Handles options and arguments.      |
// // |-------------------------------------|
// void handle_options(ProgArgs& pa, string s) {
//     if      (s == "--test")         {  exit_tests();  }   
//     else if (s == "--help")         {  exit_help(); }
//     else if (s == "--display")      {  pa.output_file = "-"; }
//     else if (s == "--overwrite")    {  pa.overwrite   = true; }
//     else {
//         exit_error(true, format("Unknown Option: '{}'.", s), 1);
//     }
// }
// void handle_args(vector<string>& arguments) {
//
//     ProgArgs pa;
//     auto i = arguments.begin();
//     auto e = arguments.end();
//     for (; i < e; i++) {
//         if ( i->starts_with("--") ) { handle_options(pa, *i); }
//         else if (pa.output_file != "-" && i == (e-1)) {
//             exit_error( exists(*i), format("Ouput file '{}' already exists.", *i), 2);
//             pa.output_file = *i;
//         }
//         else {
//             exit_error( !exists(*i), format("Input file '{}' doesn't exists.", *i), 2);
//             pa.input_files.push_back(*i);
//         }
//     }
//     exit_error( pa.input_files.size() <= 0, "No input files provided.", 1);
//     exit_run(pa);
// }
//
// int main(int argc, char** argv)
// {
//     // This must be called first
//     MakeImageNS::MakeImage::initialize(*argv);
//     vector<string> arguments = {};
//     if (argc <= 1) { exit_help(); }
//     for (int i = 1; i < argc; i++) { arguments.push_back(string(argv[i])); }
//     handle_args(arguments);
//     return 0;
// }
//
*/
