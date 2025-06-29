#pragma once
#include "macros_defines.h"
#include "make_image.h"
#include <boost/program_options.hpp>
#include <cstdlib>

#define HOME std::string(getenv("HOME"))
#define HOME_PATH(f) HOME + "/" + f
#define DEFAULT_CONFIG HOME_PATH(".config/render_markdown/config.ini")

namespace {
    const _s ERROR_CODES = R"(
ERRORS:
    1 - Argument
    2 - File
    3 - Write
    4 - Config
)";
    namespace po = boost::program_options; 
    using MakeImageNS::MdSettings;
}

namespace RenderMarkdownNS {


typedef struct ProgArgs {
    _s input_file  = "";
    _s output_file = "";
    bool         overwrite    = false;
    _s           config_file  = DEFAULT_CONFIG;
    ProgArgs() {}
} ProgArgs;

class RenderMarkdown {

    private:
        po::variables_map opt_map;
        ProgArgs prog_args = ProgArgs();
        MdSettings settings;

        void test_exit();
        void check_files();
        void handle_args();
        void initialize_options();

    public:
        po::options_description            cli_options          = po::options_description("Basic Options");
        po::options_description            cli_config_options   = po::options_description("Other Options");
        po::options_description            config_options       = po::options_description("Config Specific Options");
        po::positional_options_description posargs;
        // Parent configs housing options
        po::options_description            p_cli              = po::options_description("Allowed Options");
        po::options_description            p_config           = po::options_description("Config File Options");

        void get_options(int argc, char** argv);
        void get_config();
        void help_exit(int exit_code=0, bool config=false);
        void run_program();
        RenderMarkdown();
        RenderMarkdown(ProgArgs prog_args) : prog_args(prog_args) {};
};

};
