#pragma once
#include <map>
#include <functional>
#include <string>

namespace TestRenderMarkdownNS {
    class TestRenderMarkdown;

    typedef std::function<void (TestRenderMarkdown&)> test_function;

    class TestRenderMarkdown {
        private:
            static const std::map<std::string, test_function> test_map;
            void test_bold_italic();
            void test_code();
            void test_header();
            void test_together();
            void test_display();
        public:
            void run(std::string test);
            void run_all();

    };
}
