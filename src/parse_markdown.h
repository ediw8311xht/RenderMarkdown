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

// For now all functions will be inline, so it's easier to write and test.
// Will update later.

namespace ParseMarkdownNS {

    // Declare before defining.
    typedef struct MyToken MyToken;
    enum MyTokenType : int;
    class ParseMarkdown;

    // Instead of using namespace std which can cause conflicts or `std::`
    // I think this is a simpler way, but maybe not "standard" or "good" practice lol
    using _s   = std::string;
    using _ft  = std::function<std::vector<MyToken>(std::string)>;
    using _t   = MyToken;

    using _vc_s  = std::vector<std::string>;
    using _vc_t  = std::vector<MyToken>;
    using _vc_os = std::vector<std::optional<std::string>>;

    using _st_t = std::set<MyToken>;
    // Stores token the regex to find it and the lambda function to call with it.
    using _mp_t = std::map< MyTokenType, std::tuple<const std::regex, _ft>
                        >;
    using _mtt  = MyTokenType;
    using std::format;

    // Order to check state
    enum MyTokenType : int {
        none   = 0 ,
        header     ,
        code       ,
        bold       ,
        italic     ,
        text       ,
    };

    struct MyToken {
        _s            data;
        bool         start;
        MyTokenType   type;
    };

    class ParseMarkdown {

        private:
            static const _mp_t token_regex;
            _vc_s       files; // Names of markdown files
            _vc_os  str_files; // Lines of files
            _s      total_str; // Concenated contents of all files
            _vc_s      errors;
            _vc_t   token_out; // Where tokenized markdown is store
            inline static constexpr auto header_lambda = [](_s s) -> _vc_t { return {}; };
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
            void _read_in_files(_vc_s& f, _vc_s::iterator i, _vc_s::iterator e) {
                if (i == e) { return; }
                std::optional<_s> o = file_as_string(*i);
                std::cout << "HI" << std::endl;
                if (!o.has_value()) {
                    this->str_files.push_back("");
                    this->errors.push_back(format("{}: file '{}' couldn't be opened.", "_read_in_files", (*i)));
                }
                else {
                    this->str_files.push_back(o);
                    this->total_str += o.value();
                }
                return _read_in_files(f, ++i, e);
            }
            void handle_regex() {
            }
            void _to_tokens(_st_t states, _mtt curr_toke=_mtt::none);
        public:
            ParseMarkdown(_vc_s files={}) : files(files) {
                read_in_files();
            }
            ParseMarkdown(_s file) { ParseMarkdown({file}); }


            void to_tokens(_st_t states) { this->token_out = {}; _to_tokens({}); }

            void read_in_files() {
                this->files = {}; this->str_files = {}; this->total_str = "";
                _read_in_files(this->files, this->files.begin(), this->files.end());
            }
    };
    inline const _mp_t ParseMarkdown::token_regex = {
        { MyTokenType::header, { std::regex("") , header_lambda                    }  },                        
        { MyTokenType::code,   { std::regex("") , [](_s s) -> _vc_t { return {}; } } },
        { MyTokenType::bold,   { std::regex("") , [](_s s) -> _vc_t { return {}; } } },
        { MyTokenType::italic, { std::regex("") , [](_s s) -> _vc_t { return {}; } } },
        { MyTokenType::text,   { std::regex("") , [](_s s) -> _vc_t { return {}; } } },
    };

    inline void ParseMarkdown::_to_tokens(_st_t states, _mtt curr_toke) {
        if (curr_toke == _mtt::none) {
        }
    }
}


