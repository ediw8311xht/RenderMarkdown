#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <set>
#include <regex>
#include <fstream>
#include <functional>


namespace ParseMarkdownNS {

    // Declare before defining.
    typedef struct MyToken MyToken;
    enum MyTokenType : char;
    class ParseMarkdown;

    // Instead of using namespace std which can cause conflicts or `std::`
    // I think this is a simpler way, but maybe not "standard" or "good" practice lol
    using _vs = std::vector<std::string>;
    using _s  = std::string;
    using _tf = std::function<void(std::string)>;
    using _vt = std::vector<MyToken>;

    enum MyTokenType : char {
        h1      = '1' ,
        h2      = '2' ,
        h3      = '3' ,
        h4      = '4' ,
        h5      = '5' ,
        p       = 'p' ,
        code    = 'c' ,
        bold    = 'b' ,
        italics = 'i' ,
    };

    struct MyToken {
        _s            data;
        MyTokenType   type;
    };

    class ParseMarkdown {

        private:
            _vs       files; // Names of markdown files
            _vs lines_files; // Lines of files
            _s    str_files; // Concenated contents of all files
            // Internal function for read_in_files()
            void _read_in_files(_vs& f, _vs::iterator i, _vs::iterator e) {
            }
        public:
            ParseMarkdown(_vs files={}) : files(files) {
                read_in_files();
            }

            void read_in_files() {
                _read_in_files(this->files, this->files.begin(), this->files.end());
            }
    };

}
