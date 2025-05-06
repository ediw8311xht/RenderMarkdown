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

// For now all functions will be inline, so it's easier to write and test.
// Will update later.

namespace ParseMarkdownNS {

// Declare before defining.
typedef struct Token Token;
// Order to check state
enum class TokenType : int {
    NONE   = -1,
    CODE       ,
    H1         ,
    H2         ,
    H3         ,
    H4         ,
    H5         ,
    BOLD       ,
    ITALIC     ,
    TEXT       ,
    END        ,
};
class ParseMarkdown;

using MakeImageNS::MakeImage;
using MakeImageNS::TextData;

template <typename T>
using vec    = std::vector<T>;
using _s     = std::string;
using _t     = Token;
using _pr    = std::pair< std::string, std::string >;
using _citer = std::string::const_iterator;
using TT     = TokenType;
using mfunc  = std::function<vec<Token>(boost::smatch&)>;
using bmatch = boost::match_results<std::string::const_iterator>;
using boost::regex_search;


using std::optional;
using std::map;
using boost::regex;
using std::format;


struct Token {
    TT type       = TT::NONE;
    _s        text       = "";
    TextData  text_data  = TextData();
    Token(_s text) : text(text) {}
    Token(TT type=TT::NONE, _s text="", TextData text_data = TextData())
        : type(type), text(text), text_data(text_data) {}
};

class ParseMarkdown {
    private:
//----------------------------- Maps
        // static const regex full_regex;
        static const regex full_regex;
        static const regex inline_regex;
        static const map< const TT, mfunc >               token_funcs;
        static const std::map< const TT, const TextData > text_map;

//----------------------------- Vars
        vec<_s>           files; // Names of markdown files
        vec<optional<_s>> str_files; // Lines of files
        vec<_s>           errors;

//----------------------------- Functions
        std::optional<_s> file_as_string(_s file_string);
        // Internal function for read_in_files()
        // optional<string> so value returned can indicate when there is an
        // issue with opening the file.
        void _read_in_files(vec<_s>& f, vec<_s>::iterator i, vec<_s>::iterator e);
    public:
        _s                total_str; // Concenated contents of all files
        ParseMarkdown(vec<_s> files={});
        ParseMarkdown(_s file);

        void read_in_files();


        void make_image(_s output_file) {
            vec<_t> tokens = to_tokens();
            MakeImage g;
            for (auto [ttype, text, text_data] : tokens) {
                P(text);
                g.write_text(text, text_data);
            }
            g.save_image(output_file);
        }
        void handle_code(bmatch res, vec<_t>& t) {
            t.push_back(Token(TT::CODE, res["CODE"], text_map.at(TT::CODE)));
        }
        void handle_header(bmatch res, vec<_t>& t) {
            TT htype;
            switch (res["HEADER"].length()) {
                case 1: htype = TT::H1; break;
                case 2: htype = TT::H2; break;
                case 3: htype = TT::H3; break;
                case 4: htype = TT::H4; break;
                case 5: htype = TT::H5; break;
            }
            TextData g = text_map.at(htype);
            t.push_back(Token(htype, res["CONTENT"], text_map.at(htype)));
        }

        // Inline for formatting that is done within the text using pango
        _s handle_inline(_citer s, _citer e) {
            if (s == e) { return ""; }
            return "";
        }
        vec<_t> to_tokens() {
            vec<_t> tokens = {};
            _s curr_str = total_str;
            _citer s = total_str.begin();
            _citer e = total_str.end();
            bmatch res;
            while (regex_search(s, e, res, full_regex)) {
                tokens.push_back(handle_inline(s, res[0].first));
                if ( res["CODE"].matched) { 
                    handle_code(res, tokens);
                }
                else if ( res["HEADER"].matched ) {
                    handle_header(res, tokens);
                }
                s = res[0].second;
            }
            if (s != e) {
                tokens.push_back(handle_inline(s, e));
            }
            return tokens;
        }
};

 
}
