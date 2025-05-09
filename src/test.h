#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <string>

namespace TestRenderMarkdownNS {
    class TestRenderMarkdown;
    using std::vector;
    using std::map;
    using std::string;
    using std::function;
    typedef struct Result Result;

    typedef function<void (TestRenderMarkdown&)> test_function;

    class TestRenderMarkdown {
        private:
            static const map<string, test_function> test_map;
            void test_bold_italic();
            void test_code();
            void test_header();
            void test_together();
            void test_display();
        public:
            void run(string test);
            void run_all();

    };
}
