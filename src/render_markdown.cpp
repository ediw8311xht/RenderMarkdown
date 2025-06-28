#include "render_markdown.h"
#include "parse_markdown.h"
#include "test.h"
#include <format>
#include <execution>
#include <filesystem>

using ParseMarkdownNS::ParseMarkdown;
using std::filesystem::exists;
using filepath = std::filesystem::path;

namespace RenderMarkdownNS {

RenderMarkdown::RenderMarkdown() {
    initialize_options();
}

void exit_error(bool b, _s s, int exit_code) {
    if (b) { std::cerr << "Error: " << s << std::endl; exit(exit_code); }
}
template <typename... Args>
void exit_error(bool b, int exit_code, const std::format_string<Args...> n, Args&&... sl) {
    if (b) { exit_error(b, std::vformat(n.get(), std::make_format_args(sl...)), exit_code); }
}

void RenderMarkdown::help_exit(int exit_code, bool config) {
    if (config) {
        std::cout << this->p_config;
    } else {
        std::cout << std::format( "RenderMarkdown [{}] [{}] [{}]\n", "options", "input-file", "output-file" );
        std::cout << this->p_cli;
        std::cout << std::format( "Errors:\n{}" , ERROR_CODES );
    }
    exit(exit_code);
}
void RenderMarkdown::test_exit() {
    TestRenderMarkdownNS::TestRenderMarkdown test;
    test.run_all();
    exit(0);
}
void RenderMarkdown::run_program() {
    ParseMarkdown parse_m(prog_args.input_files);
    // MdSettings settings(prog_args.img_width, prog_args.img_height);
    MakeImageNS::MakeImage img(settings);
    parse_m.create_image(img);
    std::for_each( std::execution::par_unseq, prog_args.output_files.begin(), prog_args.output_files.end(),
        [&img, &parse_m](_s ofile) -> void {
            parse_m.save_image(img, ofile);
        }
    );
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
        ( "help,h" , po::bool_switch(&dflag) , "print help" )
        ( "help-config,H", po::bool_switch(&dflag) , "print configuration file help" )
        ( "test,t" , po::bool_switch(&dflag) , "run tests"  )
    //--- Flags/Options -----//
        (
            "config,c",
                po::value<_s>( &this->prog_args.config_file )->default_value(DEFAULT_CONFIG),
                "Config File"
        )
        (
            "input-file,i",
                po::value<_s>()->
                default_value("")->
                notifier( [this](_s f) { this->prog_args.input_files = {f}; } ),
                "input file"
        )
        (
            "output-file,o" ,
                po::value<_s>()->
                default_value("")->
                notifier( [this](_s f) { if (f != "") { this->prog_args.output_files.insert(f); } } ),
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
                notifier( [this](bool n) { if (n) { prog_args.output_files.insert("-"); } } ),
                "display to terminal"
        )
    ;
//------------------------------------- CLI & CONFIG OPTIONS ----------------------------//
    this->cli_config_options.add_options()
        (
            "width,W",
                po::value<size_t>( &this->settings.width )->
                default_value(DEFAULT_WIDTH),
                "Canvas width"
        )
        (
            "height,H",
                po::value<size_t>( &this->settings.height )->
                default_value(DEFAULT_HEIGHT),
                "Canvas height"
        )
    ;
    this->config_options.add_options()
    ;
//------------------------------------- CONFIG OPTIONS ----------------------------------//
//------------------------------------- ARGS --------------------------------------------//
    this->posargs.add("input-file"  , 1);
    this->posargs.add("output-file" , 1);
    this->p_cli.add(this->cli_options).add(cli_config_options);
    this->p_config.add(this->cli_config_options).add(config_options);
}

void RenderMarkdown::get_config() {
    try {
        store( po::parse_config_file(this->prog_args.config_file.c_str(), p_config), this->opt_map);
        notify(opt_map);
    } catch (boost::program_options::error& e) {
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

} //------------- END NAMESPACE -------------//
