#pragma once
#include <boost/program_options.hpp>
#include <string>
#include <vector>
#include <format>
#include <filesystem>

namespace RenderMarkdownNS {

using boost::program_options::options_description;
using boost::program_options::positional_options_description;
using boost::program_options::variables_map;
typedef struct ProgArgs {
    std::vector<std::string> input_files =    {};
    // For terminal output use "-"
    std::string              output_file =    "";
    size_t                   img_width   =   800;
    size_t                   img_height  =  1000;
    bool                     overwrite   = false;
} ProgArgs;

class RenderMarkdown {

    public:
        options_description            flags = options_description("Options");
        positional_options_description posargs;
        void get_options(int argc, char** argv);

        RenderMarkdown();
    private:
        void initialize_options();
        void exit_error(bool f, std::string s, int exit_code=1) const;
        variables_map opt_map;
};

};
