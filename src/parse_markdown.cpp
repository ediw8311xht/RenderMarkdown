#include "parse_markdown.h"

using namespace ParseMarkdownNS;
using Magick::ColorRGB;
using Magick::Color;;


ParseMarkdown::ParseMarkdown(vec<_s> files) : files(files) { read_in_files(); }
ParseMarkdown::ParseMarkdown(_s file) {
    this->files = vec<_s>({file});
    read_in_files();
}

const map< const TT, const TextData > ParseMarkdown::text_map = {
    { TT::CODE, TextData(  9, "Noto-Mono", ColorRGB(0, 0, 0) , Color("transparent") ) },
    { TT::H1,   TextData( 15, "Noto-Sans", ColorRGB(0, 0, 0) , Color("transparent") ) },
    { TT::H2,   TextData( 14, "Noto-Sans", ColorRGB(0, 0, 0) , Color("transparent") ) },
    { TT::H3,   TextData( 13, "Noto-Sans", ColorRGB(0, 0, 0) , Color("transparent") ) },
    { TT::H4,   TextData( 12, "Noto-Sans", ColorRGB(0, 0, 0) , Color("transparent") ) },
    { TT::H5,   TextData( 11, "Noto-Sans", ColorRGB(0, 0, 0) , Color("transparent") ) },
    { TT::TEXT, TextData( 10, "Noto-Sans", ColorRGB(0, 0, 0) , Color("transparent") ) }, 
};

const regex ParseMarkdown::block_regex = regex(
    R"((```(?<CODE>.*?)```))"
    R"(|(^|\n)(?<HEADER>[#]{1,5})[ ](?<CONTENT>.*?)(\n|$))"
);

const array< pair<regex, _s>, 4> ParseMarkdown::inline_regex = {
    pair<regex, _s>{ regex( R"(\*{3}(.+?)\*{3})" ), "<b><i>\\1</i></b>" },
    pair<regex, _s>{ regex( R"(\*{2}(.+?)\*{2})" ), "<b>\\1</b>"        },
    pair<regex, _s>{ regex( R"(\*(.+?)\*)"       ), "<i>\\1</i>"        },
    // for inline code
    pair<regex, _s>{ regex( R"(`(.+?)`)"         ),
        "<span font_family='Noto-Mono' fgcolor='#00FF00' bgcolor='#000000' >\\1</span>"
    },
    // pair<regex, _s>{ regex( R"(\*(.+?)\*)"       ), ""                  }
};

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
        std::cerr << format("{}: file '{}' couldn't be opened.", "_read_in_files", (*i)) << std::endl;
        exit(2);
    }
    else { this->total_str += o.value(); }
    return _read_in_files(f, ++i, e);
}

void ParseMarkdown::read_in_files() {
    this->str_files = {};
    this->total_str = "";
    _read_in_files(this->files, this->files.begin(), this->files.end());
}

void ParseMarkdown::generate_tokens() {
    tokens = {};
    _citer s = total_str.begin();
    _citer e = total_str.end();
    bmatch res;
    while (regex_search(s, e, res, block_regex)) {
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
}

void ParseMarkdown::make_image(_s output_file) {
    generate_tokens();
    MakeImage g;
    for (auto [ttype, text, text_data] : tokens) {
        if (text != "") {
            g.write_text(text, text_data);
        }
    }
    g.save_image(output_file);
}

_t ParseMarkdown::handle_inline(_s s) {
    _s out = s;
    for (auto& [reg, repl] : inline_regex) {
        out = boost::regex_replace(out, reg, repl);
    }
    return Token(TT::TEXT, out, text_map.at(TT::TEXT));
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

