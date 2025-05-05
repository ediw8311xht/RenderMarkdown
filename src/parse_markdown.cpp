#include "parse_markdown.h"


using namespace ParseMarkdownNS;

std::optional<_s> ParseMarkdown::file_as_string(_s file_string) {
    std::ifstream file(file_string);
    if ( file.is_open() ) {
        std::stringstream s;
        s << file.rdbuf();
        return s.str();
    }
    return std::nullopt;
}

void ParseMarkdown::_read_in_files(vec<_s>& f, vec<_s>::iterator i, vec<_s>::iterator e) {
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
void ParseMarkdown::read_in_files() {
    this->str_files = {};
    this->total_str = "";
    _read_in_files(this->files, this->files.begin(), this->files.end());
}

ParseMarkdown::ParseMarkdown(vec<_s> files)
 : files(files) {
  read_in_files();
}

ParseMarkdown::ParseMarkdown(_s file) {
    ParseMarkdown({file});
}
const std::map< const TT, const TextData > ParseMarkdown::text_map = {
    { TT::CODE, TextData( 9, "Noto-Mono", "Black", "transparent") },
    { TT::H1,   TextData(15, "Noto-Mono", "Black", "transparent") },
    { TT::H2,   TextData(14, "Noto-Mono", "Black", "transparent") },
    { TT::H3,   TextData(13, "Noto-Mono", "Black", "transparent") },
    { TT::H4,   TextData(12, "Noto-Mono", "Black", "transparent") },
    { TT::H5,   TextData(11, "Noto-Mono", "Black", "transparent") },
    // { TT::CODE,   TextData(
};

const map<const TT, mfunc> ParseMarkdown::token_funcs {
    { TT::CODE,
        [](boost::smatch& m) -> vec<_t> {
             return { _t(TT::CODE, m[0], text_map.at(TT::CODE)) };
        }
    },
};

const regex ParseMarkdown::full_regex = regex(
    R"((```(?<CODE>.*?)```))"
    R"(|(^|\n)(?<HEADER>[#]{1,5})[ ](?<CONTENT>.*?)(\n|$))"
    R"(|(?<BOLD>(\*\*)))"
    R"(|(?<ITALIC>(\*)))"
// "|"     "(?<LINE>^$)"
);

// void handle_code(_s total_str, vec<_t> tokens) {
//     auto i = std::sregex_iterator(total_str.begin(), total_str.end(), token_regex.at(TT::CODE));
//     _s pre = "";
//     _s post = total_str;
//     for (; i != std::sregex_iterator(); i++) {
//         std::smatch m = *i;
//         if ((pre = m.prefix()) != "") { tokens.push_back( { pre } ); }
//
//         tokens.push_back( { TT::CODE, m[0], text_map.at(TT::CODE) } );
//         post = m.suffix();
//     }
//     if (post != "") { tokens.push_back( { post } ); }
// }
