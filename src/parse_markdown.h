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
    enum MyTokenType : char;
    class ParseMarkdown;

    // Instead of using namespace std which can cause conflicts or `std::`
    // I think this is a simpler way, but maybe not "standard" or "good" practice lol
    using _s   = std::string;
    using _ft  = std::function<void(std::string)>;
    using _t   = MyToken;

    using _vc_s  = std::vector<std::string>;
    using _vc_t  = std::vector<MyToken>;
    using _vc_os = std::vector<std::optional<std::string>>;

    using _st_t = std::set<MyToken>;
    using _mp_t = std::map<MyTokenType, std::tuple<std::regex, std::regex>>;
    using std::format;

    enum MyTokenType : char {
        h1     = '1' ,
        h2     = '2' ,
        h3     = '3' ,
        h4     = '4' ,
        h5     = '5' ,
        p      = 'p' ,
        code   = 'c' ,
        bold   = 'b' ,
        italic = 'i' ,
        text   = 't' ,
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
                this->str_files.push_back(o);
                if (!o.has_value()) {
                    this->errors.push_back(format("{}: file '{}' couldn't be opened.", "_read_in_files", (*i)));
                }
                else {
                    this->total_str += o.value();
                }
                return _read_in_files(f, ++i, e);
            }
            void handle_regex() {
            }
        public:
            ParseMarkdown(_vc_s files={}) : files(files) {
                read_in_files();
            }
            ParseMarkdown(_s file) { ParseMarkdown({file}); }


            _vc_t to_tokens(_st_t states);

            void read_in_files() {
                this->files = {}; this->str_files = {}; this->total_str = "";
                _read_in_files(this->files, this->files.begin(), this->files.end());
            }
    };
    inline const _mp_t ParseMarkdown::token_regex = {
        { MyTokenType::h1,      { std::regex(), std::regex() } },
        { MyTokenType::h2,      { std::regex(), std::regex() } },
        { MyTokenType::h3,      { std::regex(), std::regex() } },
        { MyTokenType::h4,      { std::regex(), std::regex() } },
        { MyTokenType::h5,      { std::regex(), std::regex() } },
        { MyTokenType::p,       { std::regex(), std::regex() } },
        { MyTokenType::code,    { std::regex(), std::regex() } },
        { MyTokenType::bold,    { std::regex(), std::regex() } },
        { MyTokenType::italic,  { std::regex(), std::regex() } },
        { MyTokenType::text,    { std::regex(), std::regex() } },
    };
}

