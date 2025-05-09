```
cout << "Success?" << endl;
Let's see if spaces work correctly:

cout << "Success?" << endl;
This long line shouldn't be wrapped.
No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping No wrapping

#include <iostream>
#include "test.h"
#include <filesystem>
#include "my_makeimage.h"
#include "parse_markdown.h"

using ParseMarkdownNS::ParseMarkdown;
using std::filesystem::exists;

using namespace TestRenderMarkdownNS;
const map<string, test_function> TestRenderMarkdown::test_map = {
    {"bold italic", &TestRenderMarkdown::test_bold_italic},
    {"code",        &TestRenderMarkdown::test_code},
    {"header",      &TestRenderMarkdown::test_header},
    {"together",    &TestRenderMarkdown::test_together},
};

void test_bold_italic() {
    ParseMarkdown a("TEST_FILES/test_bold_italic.md");
    a.make_image("TEST_FILES/test_bold_italic.jpeg");
}
void test_code() {
    ParseMarkdown a("TEST_FILES/test_code.md");
    a.make_image("TEST_FILES/test_code.jpeg");

}
void test_header() {
    ParseMarkdown a("TEST_FILES/test_header.md");
    a.make_image("TEST_FILES/test_header.jpeg");
}
void test_together() {
    ParseMarkdown a("TEST_FILES/test_together.md");
    a.make_image("TEST_FILES/test_together.jpeg");
}

void TestRenderMarkdown::run(string test) {
    test_map.at(test)(*this);
}
```
