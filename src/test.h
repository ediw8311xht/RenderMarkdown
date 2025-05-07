#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <string>

namespace TestParseMarkdown {
    using std::vector;
    using std::map;
    using std::string;
    using std::function;
    typedef struct Result Result;

    class TestParseMarkdown {
        private:
            using test_function = function<void (TestParseMarkdown::*)()>;
            void _run_test();
            static map<string, test_function> test_map;
            vector<Result> test_results;
        public:
            void run(vector<string> tests);
            void run(string test);
            void run_all();

            void test_bold_italic();
            void test_code();
            void test_header();
    };
}
