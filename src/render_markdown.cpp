#include "render_markdown.h"
#include "parse_markdown.h"
#include "test.h"
#include <format>
#include <execution>
#include <boost/program_options.hpp>
#include <filesystem>

using ParseMarkdownNS::ParseMarkdown;
using namespace boost::program_options;
using std::filesystem::exists;
using filepath = std::filesystem::path;
namespace RenderMarkdownNS {

//-------------------------
void RenderMarkdown::initialize_options() {
    bool dflag = false;
//----- OPTIONS -----//
    this->flags.add_options()
    //--- Special -----------//
        ( "help,h" , bool_switch(&dflag) , "print help" )
        ( "test,t" , bool_switch(&dflag) , "run tests"  )
    //--- Flags/Options -----//
        (
            "width,W",    
                value<size_t>( &this->prog_args.img_width )->
                default_value(DEFAULT_WIDTH),
            "Canvas width"  )
        (   "height,H",   
                value<size_t>( &this->prog_args.img_height )->
                default_value(DEFAULT_HEIGHT),
            "Canvas height" )
        (
            "config,c",   
                value<_s>( &this->prog_args.config_file )->default_value(DEFAULT_CONFIG),
            "Config File (json)" )
        (
            "display,d",      
                bool_switch(&dflag)->
                notifier( [this](bool n) { if (n) { prog_args.output_files.insert("-"); } } ),
            "display to terminal" )
        (
            "overwrite,O",
                bool_switch(&dflag)->
                notifier( [this](bool n) { this->prog_args.overwrite = n; } ),
            "overwrite output file" )
    //--- Args --------------//
        (
            "input-file,i",
                value<_s>()->
                default_value("")->
                notifier( [this](_s f) { this->prog_args.input_files = {f}; } ),
            "input file" )
        (
            "output-file,o" , 
                value<_s>()->
                default_value("")->
                notifier( [this](_s f) { if (f != "") { this->prog_args.output_files.insert(f); } } ),  
            "output file" )
    ;
//----- END OPTIONS -----//
    this->posargs.add("input-file"  , 1);
    this->posargs.add("output-file" , 1);
}

void RenderMarkdown::run_program() {
    ParseMarkdown parse_m(prog_args.input_files);
    parse_m.create_image(prog_args.img_width, prog_args.img_height);
    std::for_each( std::execution::par_unseq, prog_args.output_files.begin(), prog_args.output_files.end(),
        [&parse_m](_s ofile) -> void {
            parse_m.save_image(ofile);
        }
    );
}

void exit_error(bool b, _s s, int exit_code) {
    if (b) { std::cerr << "Error: " << s << std::endl; exit(exit_code); }
}
template <typename... Args>
void exit_error(bool b, int exit_code, const std::format_string<Args...> n, Args&&... sl) {
    if (b) { exit_error(b, std::vformat(n.get(), std::make_format_args(sl...)), exit_code); }
}
void RenderMarkdown::check_files() {
    std::for_each( std::execution::par_unseq, prog_args.input_files.begin(), prog_args.input_files.end(), 
        [](_s f) -> void { 
            exit_error( !exists(f), 2, "input file: '{}' couldn't be found", f); 
        }
    );
    exit_error(prog_args.output_files.size() <= 0, "no output file provided", 2);
    std::for_each( std::execution::par_unseq, prog_args.output_files.begin(), prog_args.output_files.end(),
        [this](_s f) -> void {
            if (f != "-" && !this->prog_args.overwrite) {
                exit_error( exists(f), 2, "output file: '{}' already exists (use --overwrite to overwrite file)", f);
            }
        }
    );
}
void RenderMarkdown::help_exit(int exit_code) {
    std::cout << std::format( "RenderMarkdown [{}] [{}] [{}]\n", "options", "input-file", "output-file" );
    std::cout << this->flags;
    std::cout << std::format( "Errors:\n{}" , ERROR_CODES );
    exit(exit_code);
}
void RenderMarkdown::test_exit() {
    TestRenderMarkdownNS::TestRenderMarkdown test;
    test.run_all();
    exit(0);
}
void RenderMarkdown::handle_args() {
    if ( opt_map["help"].as<bool>() ) {
        help_exit();
    } else if ( opt_map["test"].as<bool>() ) {
        test_exit();
    }
    //_s infile = opt_map["input-file"].as<_s>();
    // _s outfile = prog_args.output_file == "-" ? "-" :
    //prog_args.input_files = {opt_map["input-file"].as<_s>()};
    check_files();
    return;
}
void RenderMarkdown::get_options(int argc, char** argv) {
    try {
        store(
            command_line_parser(argc, argv).options(this->flags).positional(this->posargs).run(),
            this->opt_map
        );
        notify(opt_map);
        handle_args();
    } catch (boost::program_options::error& e) {
        exit_error(true, 1, "{}: {}", "Handling Options/Arguments", e.what());
    }
}
RenderMarkdown::RenderMarkdown() {
    initialize_options();
}

}
