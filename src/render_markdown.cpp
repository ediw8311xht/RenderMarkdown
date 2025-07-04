#include "render_markdown.h"
#include "parse_markdown.h"
#include "test.h"
#include <Magick++.h>
#include <format>
#include <iostream>
#include <filesystem>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#define STR(s) #s
#define DOUBLE_ZERO_TO_ONE (1([.]0+)?|0([.][0-9]+)?)
#define REGEX_GROUP(groupname, group) "(?<"#groupname">" STR(group) ")"

using std::filesystem::exists;
using filepath = std::filesystem::path;
using ParseMarkdownNS::ParseMarkdown;
using boost::regex;
using boost::smatch;
//---------------------------------------------------------
const regex color_regex(
        R"([^\\d.]*)" 
        REGEX_GROUP(N1, DOUBLE_ZERO_TO_ONE)
        R"([^\\d.]+)"
        REGEX_GROUP(N2, DOUBLE_ZERO_TO_ONE)
        R"([^\\d.]+)"
        REGEX_GROUP(N3, DOUBLE_ZERO_TO_ONE) 
        R"([^\\d.]*)"
);
Magick::ColorRGB parse_color(_s col_arr) {
    smatch m;
    if (boost::regex_match(col_arr, m, color_regex)) {
        return Magick::ColorRGB( std::stod(m["N1"].str()), std::stod(m["N2"]), std::stod(m["N3"].str()) );
    } else {
        throw std::invalid_argument( "In parse_color, invalid argument: " + col_arr );
    }
    // return Magick::ColorRGB(r.at(0), r.at(1), r.at(2));
}
//---------------------------------------------------------
void exit_error(bool b, _s s, int exit_code) {
    if (b) { std::cerr << "Error: " << s << std::endl; exit(exit_code); }
}
// https://en.cppreference.com/w/cpp/utility/format/vformat.html
template <typename... Args>
void exit_error(bool b, int exit_code, const std::format_string<Args...> n, Args&&... sl) {
    if (b) { exit_error(b, std::vformat(n.get(), std::make_format_args(sl...)), exit_code); }
}

namespace RenderMarkdownNS {

RenderMarkdown::RenderMarkdown() {
    initialize_options();
}

void RenderMarkdown::help_exit(int exit_code, bool config) {
    if (config) {
        std::cout << this->p_config;
    } else {
        std::cout << std::format( "RenderMarkdown [{}] [{}] [{}]\n", "options", "input-file", "output-file" );
        std::cout << this->p_cli;
        std::cout << R"(
ERRORS:
    1 - Argument
    2 - File
    3 - Write
    4 - Config
)";
    }
    exit(exit_code);
}
void RenderMarkdown::test_exit() {
    TestRenderMarkdownNS::TestRenderMarkdown test;
    test.run_all();
    exit(0);
}

void RenderMarkdown::check_files() {
    exit_error( !exists(prog_args.input_file), 2, "input file: '{}' couldn't be found", prog_args.input_file);
    exit_error( prog_args.output_file == "", "no output file provided", 2);
    exit_error( 
        prog_args.output_file != "-" && !this->prog_args.overwrite && exists(prog_args.output_file),
        2, "output file: '{}' already exists (use --overwrite to overwrite file)", prog_args.output_file
    );
}

void RenderMarkdown::handle_args() {
    if ( opt_map["help"].as<bool>() ) {
        help_exit();
    } else if (opt_map["help-config"].as<bool>() ) {
        help_exit(0, true);
    } else if ( opt_map["test"].as<bool>() ) {
        test_exit();
    }
    check_files();
    return;
}

//-------------------------
void RenderMarkdown::initialize_options() {
    bool dflag = false;
//------------------------------------- CLI OPTIONS -------------------------------------//
    this->cli_options.add_options()
    //--- Special -----------//
        ( "help,h"        , po::bool_switch(&dflag) , "print help"                    )
        ( "help-config,H" , po::bool_switch(&dflag) , "print configuration file help" )
        ( "test,t"        , po::bool_switch(&dflag) , "run tests"                     )
    //--- Flags/Options -----//
        (
            "config,c",
                po::value<_s>( &this->prog_args.config_file )->default_value(DEFAULT_CONFIG),
                "Config File"
        )
        (
            "input-file,i",
                po::value<_s>(&this->prog_args.input_file),
                "input file"
        )
        (
            "output-file,o" ,
                po::value<_s>(&this->prog_args.output_file),
                "output file"
        )
        (
            "overwrite,R",
                po::bool_switch(&dflag)->
                notifier( [this](bool n) { this->prog_args.overwrite = n; } ),
                "overwrite output file"
        )
        (
            "display,d",
                po::bool_switch(&dflag)->
                notifier( [this](bool n) { if (n) { prog_args.output_file = "-"; } } ),
                "display to terminal"
        )
    ;
//------------------------------------- CLI & CONFIG OPTIONS ----------------------------//
    this->cli_config_options.add_options()
        (
            "width,W",
                po::value<size_t>( &this->settings.width ),
                "Canvas width"
        )
        (
            "height,H",
                po::value<size_t>( &this->settings.height ),
                "Canvas height"
        )
    ;
//------------------------------------- CONFIG OPTIONS ----------------------------------//
    this->config_options.add_options()
        (
            "canvasbg",
            po::value<_s>()->
            notifier( [this](_s col_arr) { 
                this->settings.canvas_bg = parse_color(col_arr);
            }),
            "Canvas background (rgb)\n"
            "rgb values (between 0.0 and 1.1)\n"
            "example: 0.673 0.845 0.902 (light blue)"
        )
    ;
//------------------------------------- ARGS --------------------------------------------//
    this->posargs.add("input-file"  , 1);
    this->posargs.add("output-file" , 1);
    this->p_cli.add(this->cli_options).add(cli_config_options);
    this->p_config.add(this->config_options).add(cli_config_options);
}

void RenderMarkdown::get_config() {
    if (!exists(prog_args.config_file)) {
        std::cout << format("Config file: {}, couldn't be found.\n", prog_args.config_file);
        return;
    }
    try {
        store( po::parse_config_file(this->prog_args.config_file.c_str(), p_config), this->opt_map);
        notify(opt_map);
    } catch (boost::program_options::error& e) {
        exit_error(true, 4, "{}: {}", "Parsing Config", e.what());
    }
}
void RenderMarkdown::get_options(int argc, char** argv) {
    try {
        store(
            po::command_line_parser(argc, argv).options(this->p_cli).positional(this->posargs).run(),
            this->opt_map
        );
        notify(opt_map);
        handle_args();
    } catch (boost::program_options::error& e) {
        exit_error(true, 1, "{}: {}", "Handling Options/Arguments", e.what());
    }
}

void RenderMarkdown::run_program(int argc, char** argv) {
    MakeImageNS::MakeImage::setup_magick(*argv);
    get_options(argc, argv);
    get_config();
    ParseMarkdown parse_m(prog_args.input_file);
    MakeImageNS::MakeImage img(settings);
    parse_m.create_image(img);
    parse_m.save_image(img, prog_args.output_file);
}

} //------------- END NAMESPACE -------------//
