#include <iostream>
#include <stdlib.h>
#include "parse_markdown.h"
#include "my_makeimage.h"
#include <format>
// #include <Magick++.h>


// namespace Mag = Magick;
using ParseMarkdownNS::ParseMarkdown;
using ParseMarkdownNS::Token;
using ParseMarkdownNS::TokenType;
using MakeImageNS::MakeImage;
using MakeImageNS::TextData;

using _s = std::string;
template <typename T>
using vec = std::vector<T>;
using std::cout;
using std::endl;
using std::cerr;
using std::format;

_s conc(_s a, _s b) { return format("{}/{}", a, b); }

_s my_get_env(const char* s) {
    char* o;
    return ( (o = getenv(s)) != nullptr ? static_cast<_s>(o) : "");
}

// Checks bool
// Optionally exit with status_code (>= 1)
// [since <= 0 is non-error status code]
void output_error(bool check, const char* error_message="", int exit_status=1) {
    if (check) { return; }

    cerr << error_message << endl;
    if ( exit_status >= 1) {
        exit(exit_status);
    }
}



void test_1() {
    MakeImage a;
}
void test_3() {
    const std::string STATIC_FILES = my_get_env( "STATIC_FILES" );
    output_error(STATIC_FILES != "", "STATIC_FILES environment var not found");
    _s test_file = conc(STATIC_FILES, "test.md");
    _s out_file = conc("STATIC_FILES", "out.jpeg");
    vec<_s> n({test_file});
    ParseMarkdown pmark(n);
    pmark.make_image(out_file);
    // vec<Token> tokens = pmark.to_tokens();
    // for (auto [i, j, k] : tokens) {
    //     // Enum
    //     cout << static_cast<int>(i) << endl;
    //     cout << j << endl;
    //     cout << endl;
    // }
    // cout << "Success" << endl;
}

void test2() {
    return;
}

int main(int argc, char** argv)
{
    
    //MakeImage::initialize(*argv);
    test_3();
    //test2();
    std::cout << "HI" << std::endl;
    return 0;
}

