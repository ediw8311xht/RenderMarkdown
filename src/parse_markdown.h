#pragma once

#include "my_make_image.h"
#include "macros_defines.h"
#include <set>
#include <string>
#include <optional>
#include <map>
#include <boost/regex.hpp>

namespace ParseMarkdownNS {

// Declare before defining.
enum class TokenType : int;
class ParseMarkdown;

enum class TokenType : int {
    NONE = -1   ,
    CODE = 0    ,
    H1=1, H2=2, H3=3, H4=4, H5=5,
    TEXT=6      ,
    END=7       ,
};


class ParseMarkdown {
    private:
//----------------------------- FORMATTING ----------------------------------------//
        static const std::map< const TokenType, const MakeImageNS::TextData > text_map;
//----------------------------- IMAGE ---------------------------------------------//
        std::unique_ptr<MakeImageNS::MakeImage> mimg;
//----------------------------- Maps
        // static const regex full_regex;
        static const boost::regex replace_chars;
        static const boost::regex block_regex;
        static const std::array< std::pair<boost::regex, _s>, 4> inline_regex;

//----------------------------- Vars
        // Names of markdown files
        std::set<_s> files = {};
        // Lines of files
        std::set<std::optional<_s>> str_files;
        // Concenated contents of all files
        _s total_str; 

//----------------------------- Functions
        std::optional<_s> file_as_string(_s file_string);
        // Internal function for read_in_files()
        // optional<string> so value returned can indicate when there is an
        // issue with opening the file.
        void _read_in_files( std::set<_s>& f,
                             std::set<_s>::iterator i,
                             std::set<_s>::iterator e );
        void handle_code(   const boost::match_results<_s::const_iterator>& res );
        void handle_header( const boost::match_results<_s::const_iterator>& res );
        // Text must be cleaned since both pango and Magick++ use certain characters for formatting 
        _s clean_text(_s s);
        // Inline for formatting that is done within the text using pango
        void handle_inline(_s s);
    public:
        // Explicit since reading in files happens when intialized.
        explicit ParseMarkdown(std::set<_s> files={});
        explicit ParseMarkdown(_s file);

        void read_in_files();
        void make_image(size_t image_width, size_t image_height);
        void save_image(_s output_file);
};

 
}
