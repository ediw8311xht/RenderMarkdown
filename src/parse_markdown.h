#pragma once

#include "my_makeimage.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <regex>
#include <functional>
#include <optional>
#include <format>
#include <map>
#include <tuple>
#include <Magick++.h>
#include <boost/regex.hpp>

#define P(A) std::cout << A << std::endl;

namespace ParseMarkdownNS {

// Declare before defining.
typedef struct Token Token;
enum class TokenType : int;
class ParseMarkdown;

using MakeImageNS::MakeImage;
using MakeImageNS::TextData;

// STD
template <typename T>
using vec    = std::vector<T>;
using _s     = std::string;
using _pr    = std::pair< std::string, std::string >;
using _citer = std::string::const_iterator;
using mfunc  = std::function<vec<Token>(boost::smatch&)>;
using std::optional;
using std::map;
using std::format;

// Boost
using bmatch = boost::match_results<std::string::const_iterator>;
using boost::regex;
using boost::regex_search;

// Mine
using _t     = Token;
using TT     = TokenType;

enum class TokenType : int {
    NONE = -1   ,
    CODE        ,
    H1, H2, H3, H4, H5,
    TEXT        ,
    END         ,
};

struct Token {
    TT        type       = TT::NONE;
    _s        text       = "";
    TextData  text_data  = TextData();

    Token(_s text) : text(text) {}
    Token(TT type=TT::TEXT, _s text="", TextData text_data = TextData())
        : type(type), text(text), text_data(text_data) {}
};

class ParseMarkdown {
    private:
//----------------------------- Maps
        // static const regex full_regex;
        static const regex full_regex;
        static const regex bold_regex;
        static const regex italic_regex;
        static const regex bold_italic_regex;
        static const std::map< const TT, const TextData > text_map;

//----------------------------- Vars
        vec<_s>           files; // Names of markdown files
        vec<optional<_s>> str_files; // Lines of files
        vec<_s>           errors;
        _s                total_str; // Concenated contents of all files

//----------------------------- Functions
        std::optional<_s> file_as_string(_s file_string);
        // Internal function for read_in_files()
        // optional<string> so value returned can indicate when there is an
        // issue with opening the file.
        void _read_in_files(vec<_s>& f, vec<_s>::iterator i, vec<_s>::iterator e);
        void handle_code(const bmatch& res, vec<_t>& t);
        void handle_header(const bmatch& res, vec<_t>& t);
        vec<_t> to_tokens();
        // Inline for formatting that is done within the text using pango
        _t handle_inline(_s s);
    public:
        ParseMarkdown(vec<_s> files={});
        ParseMarkdown(_s file);

        void read_in_files();
        void make_image(_s output_file);

};

 
}
