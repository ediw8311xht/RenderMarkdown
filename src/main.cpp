#include <iostream>
#include <stdlib.h>
#include "parse_markdown.h"
#include <format>


using ParseMarkdownNS::ParseMarkdown;
using ParseMarkdownNS::MyToken;
using ParseMarkdownNS::_vs;
using ParseMarkdownNS::_s;

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
    const std::string STATIC_FILES = getenv( "STATIC_FILES" );
    output_error(STATIC_FILES != "", "STATIC_FILES environment var not found");
    _s test_file = conc(STATIC_FILES, "test.md");
    _vs n({test_file});
    ParseMarkdown a(n);
    cout << "Success" << endl;
}

int main()
{
    test_1();
    std::cout << "HI" << std::endl;
    return 0;
}

