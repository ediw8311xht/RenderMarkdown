#include "macros_defines.h"
#include "test.h"
#include "parse_markdown.h"
#include <iostream>
// #include <filesystem>


using TestRenderMarkdownNS::test_function;
namespace TestRenderMarkdownNS {

const std::map<std::string, test_function> TestRenderMarkdown::test_map = {
    {"bold italic", &TestRenderMarkdown::test_bold_italic},
    {"code",        &TestRenderMarkdown::test_code},
    {"header",      &TestRenderMarkdown::test_header},
    {"together",    &TestRenderMarkdown::test_together},
    {"display",     &TestRenderMarkdown::test_display},
};

void TestRenderMarkdown::test_bold_italic() {
    ParseMarkdownNS::ParseMarkdown a("TEST_FILES/test_bold_italic.md");
    a.make_image( DEFAULT_WIDTH , DEFAULT_HEIGHT );
    a.save_image("TEST_FILES/test_bold_italic.jpeg");
}
void TestRenderMarkdown::test_code() {
    ParseMarkdownNS::ParseMarkdown a("TEST_FILES/test_code.md");
    a.make_image( DEFAULT_WIDTH , DEFAULT_HEIGHT );
    a.save_image("TEST_FILES/test_code.jpeg");

}
void TestRenderMarkdown::test_header() {
    ParseMarkdownNS::ParseMarkdown a("TEST_FILES/test_header.md");
    a.make_image( DEFAULT_WIDTH , DEFAULT_HEIGHT );
    a.save_image("TEST_FILES/test_header.jpeg");
}
void TestRenderMarkdown::test_together() {
    ParseMarkdownNS::ParseMarkdown a("TEST_FILES/test_together.md");
    a.make_image( DEFAULT_WIDTH , DEFAULT_HEIGHT );
    a.save_image("TEST_FILES/test_together.jpeg");
}

void TestRenderMarkdown::test_display() {
    ParseMarkdownNS::ParseMarkdown a("TEST_FILES/test_together.md" );
    a.make_image( 300 , 300 );
    a.save_image("-");
}

void TestRenderMarkdown::run(std::string test) {
    std::cout << "Running test: " + test << std::endl;
    test_map.at(test)(*this);
}
void TestRenderMarkdown::run_all() {
    for (auto& [i, _f] : test_map) {
        run(i);
    }
}
}
