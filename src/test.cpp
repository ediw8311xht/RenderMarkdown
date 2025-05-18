#include <iostream>
#include "test.h"
#include <filesystem>
#include "my_make_image.h"
#include "parse_markdown.h"

using ParseMarkdownNS::ParseMarkdown;
using std::filesystem::exists;

using namespace TestRenderMarkdownNS;
const map<string, test_function> TestRenderMarkdown::test_map = {
    {"bold italic", &TestRenderMarkdown::test_bold_italic},
    {"code",        &TestRenderMarkdown::test_code},
    {"header",      &TestRenderMarkdown::test_header},
    {"together",    &TestRenderMarkdown::test_together},
    {"display",     &TestRenderMarkdown::test_display},
};

void TestRenderMarkdown::test_bold_italic() {
    ParseMarkdown a("TEST_FILES/test_bold_italic.md");
    a.make_image("TEST_FILES/test_bold_italic.jpeg");
}
void TestRenderMarkdown::test_code() {
    ParseMarkdown a("TEST_FILES/test_code.md");
    a.make_image("TEST_FILES/test_code.jpeg");

}
void TestRenderMarkdown::test_header() {
    ParseMarkdown a("TEST_FILES/test_header.md");
    a.make_image("TEST_FILES/test_header.jpeg");
}
void TestRenderMarkdown::test_together() {
    ParseMarkdown a("TEST_FILES/test_together.md");
    a.make_image("TEST_FILES/test_together.jpeg");
}

void TestRenderMarkdown::test_display() {
    ParseMarkdown a("TEST_FILES/test_together.md");
    a.make_image("-");
}

void TestRenderMarkdown::run(string test) {
    std::cout << "Running test: " + test << std::endl;
    test_map.at(test)(*this);
}
void TestRenderMarkdown::run_all() {
    for (auto& [i, _f] : test_map) {
        run(i);
    }
}
