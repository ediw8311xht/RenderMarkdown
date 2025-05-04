#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <set>
#include <regex>
#include <fstream>
#include <functional>
#include <optional>
#include <format>
#include <map>
#include <tuple>
#include <Magick++.h>
#include "my_makeimage.h"

// For now all functions will be inline, so it's easier to write and test.
// Will update later.

namespace ParseMarkdownNS {

// Declare before defining.
typedef struct Token Token;
enum TokenType : int;
class ParseMarkdown;

using _s   = std::string;
using _t   = Token;
using mfunc = std::function<void(std::smatch, std::vector<Token>& tokens)>;


template <typename T>
using vec   = std::vector<T>;
using std::optional;
using std::map;
using std::regex;
using MakeImageNS::MakeImage;
using MakeImageNS::TextData;
using std::format;

// Order to check state
enum TokenType : int {
    none   = -1,
    code       ,
    header     ,
    bold       ,
    italic     ,
    text       ,
};

struct Token {
    TokenType type       = TokenType::none;
    _s        text       = "";
    TextData  text_data  = TextData();

    Token(_s text) : text(text) {}

    Token(TokenType type=TokenType::none, _s text="", TextData text_data = TextData())
        : type(type), text(text), text_data(text_data) {}
};

class ParseMarkdown {
    private:
//----------------------------- Maps
        static const map< const TokenType, const regex >         token_regex;
        static const map< const TokenType, mfunc >               token_funcs;
        static const std::map< const TokenType, const TextData > text_map;

//----------------------------- Vars
        vec<_s>           files; // Names of markdown files
        vec<optional<_s>> str_files; // Lines of files
        _s                total_str; // Concenated contents of all files
        vec<_s>           errors;
        vec<_t> _to_tokens(_s curr);

//----------------------------- Functions
        // Internal function for read_in_files()
        // optional<string> so value returned can indicate when there is an
        // issue with opening the file.
        std::optional<_s> file_as_string(_s file_string) {
            std::ifstream file(file_string);
            if ( file.is_open() ) {
                std::stringstream s;
                return s << file.rdbuf(), s.str();
            }
            return std::nullopt;
        }
        void _read_in_files(vec<_s>& f, vec<_s>::iterator i, vec<_s>::iterator e) {
            if (i == e) { return; }
            std::optional<_s> o = file_as_string(*i);
            this->str_files.push_back(o);
            if (!o.has_value()) {
                this->errors.push_back(format("{}: file '{}' couldn't be opened.", "_read_in_files", (*i)));
            }
            else {
                this->total_str += o.value();
            }
            return _read_in_files(f, ++i, e);
        }
    public:
        ParseMarkdown(vec<_s> files={}) : files(files)  { read_in_files();       }
        ParseMarkdown(_s file)                          { ParseMarkdown({file}); }

        void read_in_files() {
            this->files = {}; this->str_files = {}; this->total_str = "";
            _read_in_files(this->files, this->files.begin(), this->files.end());
        }

        vec<_t> to_tokens() { return _to_tokens(total_str); }

        void handle_code(_s total_str, vec<_t> tokens) {
            auto i = std::sregex_iterator(total_str.begin(), total_str.end(), token_regex.at(TokenType::code));
            _s pre = "";
            _s post = total_str;
            for (; i != std::sregex_iterator(); i++) {
                std::smatch m = *i;
                if ((pre = m.prefix()) != "") { tokens.push_back( { pre } ); }

                tokens.push_back( { TokenType::code, m[0], text_map.at(TokenType::code) } );
                post = m.suffix();
            }
            if (post != "") { tokens.push_back( { post } ); }
        }
        void handler(_s total_str, vec<_t> tokens, TokenType token_type) {
            auto i = std::sregex_iterator(total_str.begin(), total_str.end(), token_regex.at(token_type));
            _s pre = "";
            _s post = total_str;
            for (; i != std::sregex_iterator(); i++) {
                std::smatch m = *i;
                if ((pre = m.prefix()) != "") { tokens.push_back( { pre } ); }

                tokens.push_back( { TokenType::code, m[0], text_map.at(TokenType::code) } );
                post = m.suffix();
            }
            if (post != "") { tokens.push_back( { post } ); }
        }
};

inline vec<_t> ParseMarkdown::_to_tokens(_s curr) {
    vec<_t> f = {};
    handle_code(curr, f);

    return {};
}
 
inline const std::map< const TokenType, const TextData > ParseMarkdown::text_map = {
    { TokenType::code, TextData(12, "Noto-Mono", "Black", "transparent") }
    // { TokenType::code,   TextData(
};

inline const map<const TokenType, const regex> ParseMarkdown::token_regex = {
    { TokenType::code,    std::regex("```([\\s\\S]*?)```")        },
    { TokenType::header,  std::regex("^([#]{1,5})[ ](.*)$")       },
    { TokenType::bold,    std::regex("([\\*]{2})(.*)([\\*]{2})")  },
    { TokenType::italic,  std::regex("([*])(.*)([*])")            },
    { TokenType::text,    std::regex("")                          },
};

inline const map<const TokenType, mfunc> ParseMarkdown::token_funcs {
    { TokenType::code, 
        [](std::smatch m, vec<Token>& tokens) {
            tokens.push_back( {TokenType::code, m[0], text_map.at(TokenType::code)});
        }
    },
};

}
