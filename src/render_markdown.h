#pragma once
#include "macros_defines.h"
#include <boost/program_options.hpp>
#include <set>

namespace RenderMarkdownNS {

typedef struct ProgArgs {
    std::set<_s> input_files  = {};
    std::set<_s> output_files = {}; // For terminal output use "-"
    size_t       img_width    =   800;
    size_t       img_height   =  1000;
    bool         overwrite    = false;
    ProgArgs() {}
} ProgArgs;

class RenderMarkdown {

    private:
        boost::program_options::variables_map opt_map;
        ProgArgs prog_args = ProgArgs();

        void test_exit();
        void check_files();
        void handle_args();
        void initialize_options();

    public:
        boost::program_options::options_description flags = boost::program_options::options_description("Options");
        boost::program_options::positional_options_description posargs;
        void get_options(int argc, char** argv);
        void help_exit(int exit_code=0);
        void run_program();
        RenderMarkdown();
        RenderMarkdown(ProgArgs prog_args) : prog_args(prog_args) {};
};

};
