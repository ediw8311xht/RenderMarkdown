#pragma once

#include "make_image.h"
#include "macros_defines.h"
#include <set>
#include <string>
#include <boost/regex.hpp>

namespace ParseMarkdownNS {
using reg_con = std::array< std::pair<boost::regex, _s>, 5>;


class ParseMarkdown {
    private:
        //----------------------------- FORMATTING ---------//
        // static const std::map< const TokenType, const MakeImageNS::TextData > text_map;
        //----------------------------- Maps ---------------//
        // static const regex full_regex;
        static const boost::regex replace_chars;
        static const boost::regex block_regex;
        static const reg_con inline_regex;
        //----------------------------- Vars ---------------//
        std::set<_s> files = {};               // Names of markdown files
        // std::set<std::optional<_s>> str_files; // Lines of files
        _s total_str;                          // Concenated contents of all files
        //----------------------------- Functions ----------//
        /* Internal function for read_in_files()
           optional<string> so value returned can indicate when there is an
           issue with opening the file. */
        _s clean_text(_s s);
        void _read_in_files( std::set<_s>& f, std::set<_s>::iterator i, std::set<_s>::iterator e );
        void handle_code(   MakeImageNS::MakeImage& mimg, const boost::match_results<_s::const_iterator>& res );
        void handle_header( MakeImageNS::MakeImage& mimg, const boost::match_results<_s::const_iterator>& res );
        void handle_image(  MakeImageNS::MakeImage& mimg, const boost::match_results<_s::const_iterator>& res );
        void handle_line(   MakeImageNS::MakeImage& mimg, const boost::match_results<_s::const_iterator>& res );
        /* Text must be cleaned since both pango and Magick++ use certain
           characters for formatting Inline for formatting that is done within
           the text using pango */
        void handle_inline(MakeImageNS::MakeImage& mimg, _s s);
    public:
        // Explicit since reading in files happens when intialized.
        explicit ParseMarkdown(std::set<_s> files={});
        explicit ParseMarkdown(_s file);

        void read_in_files();
        void create_image(MakeImageNS::MakeImage& mimg);
        void save_image(MakeImageNS::MakeImage& mimg, _s output_file);
};

 
}
