#include "parse_markdown.h"

using namespace ParseMarkdownNS;


ParseMarkdown::ParseMarkdown(vec<_s> files) : files(files) { read_in_files(); }
ParseMarkdown::ParseMarkdown(_s file) { ParseMarkdown({file}); }

const map< const TT, const TextData > ParseMarkdown::text_map = {
    { TT::CODE, TextData(  9, "Noto-Mono", "Green", "Black"       ) },
    { TT::H1,   TextData( 15, "Noto-Sans", "Black", "transparent" ) },
    { TT::H2,   TextData( 14, "Noto-Sans", "Black", "transparent" ) },
    { TT::H3,   TextData( 13, "Noto-Sans", "Black", "transparent" ) },
    { TT::H4,   TextData( 12, "Noto-Sans", "Black", "transparent" ) },
    { TT::H5,   TextData( 11, "Noto-Sans", "Black", "transparent" ) },
    { TT::TEXT, TextData( 10, "Noto-Sans", "Black", "transparent" ) }, 
};

const regex ParseMarkdown::full_regex = regex(
    R"((```(?<CODE>.*?)```))"
    R"(|(^|\n)(?<HEADER>[#]{1,5})[ ](?<CONTENT>.*?)(\n|$))"
);

const regex ParseMarkdown::bold_italic_regex = regex( R"(\*{3}(.+?)\*{3})" );
const regex ParseMarkdown::bold_regex        = regex( R"(\*{2}(.+?)\*{2})" );
const regex ParseMarkdown::italic_regex      = regex( R"(\*(.+?)\*)"       );

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
    else { this->total_str += o.value(); }
    return _read_in_files(f, ++i, e);
}

void ParseMarkdown::read_in_files() {
    this->str_files = {};
    this->total_str = "";
    _read_in_files(this->files, this->files.begin(), this->files.end());
}

_t ParseMarkdown::handle_inline(_s s) {
    _s out = boost::regex_replace(s, bold_italic_regex, "<b><i>\\1</i></b>");
    out = boost::regex_replace(out, bold_regex, "<b>\\1</b>");
    out = boost::regex_replace(out, italic_regex, "<i>\\1</i>");
    return Token(TT::TEXT, out, text_map.at(TT::TEXT));
}


vec<_t> ParseMarkdown::to_tokens() {
    vec<_t> tokens = {};
    _citer s = total_str.begin();
    _citer e = total_str.end();
    bmatch res;
    while (regex_search(s, e, res, full_regex)) {
        bmatch n;
        // tokens.push_back(handle_inline(s, res[0].first));
        tokens.push_back(handle_inline( res.prefix() ) );
        if      ( res["CODE"].matched   ) { handle_code(   res, tokens ); }
        else if ( res["HEADER"].matched ) { handle_header( res, tokens ); }
        s = res[0].second;
    }
    if (s != e) {
        tokens.push_back(handle_inline( _s(s, e)));
    }
    return tokens;
}

void ParseMarkdown::make_image(_s output_file) {
    vec<_t> tokens = to_tokens();
    MakeImage g;
    for (auto [ttype, text, text_data] : tokens) {
        g.write_text(text, text_data);
    }
    g.save_image(output_file);
}

void ParseMarkdown::ParseMarkdown::handle_code(const bmatch& res, vec<_t>& t) {
    t.push_back(Token(TT::CODE, res["CODE"], text_map.at(TT::CODE)));
}

void ParseMarkdown::handle_header(const bmatch& res, vec<_t>& t) {
    TT htype;
    switch (res["HEADER"].length()) {
        case 1: htype = TT::H1; break;
        case 2: htype = TT::H2; break;
        case 3: htype = TT::H3; break;
        case 4: htype = TT::H4; break;
        case 5: htype = TT::H5; break;
    }
    TextData g = text_map.at(htype);
    t.push_back(Token(htype, res["CONTENT"], text_map.at(htype)));
}

