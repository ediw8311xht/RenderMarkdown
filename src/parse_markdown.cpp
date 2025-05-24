#include "parse_markdown.h"
#include "macros_defines.h"
#include <iostream>
#include <format>
#include <fstream>
#include <Magick++.h>
#include <regex> // For stringstream

//----------------------------------------------------- Mine -------------------//
using TT = ParseMarkdownNS::TokenType;
//----------------------------------------------------- Standard Library -------//
using std::pair;
using std::set;
//----------------------------------------------------- Boost ------------------//
using boost::regex;
using bmatch = boost::match_results<_s::const_iterator>;
//----------------------------------------------------- Magick++ ---------------//
using Magick::ColorRGB;
using Magick::Color;

namespace ParseMarkdownNS {

ParseMarkdown::ParseMarkdown(set<_s> files) : files( files )            { read_in_files(); }
ParseMarkdown::ParseMarkdown(_s file)       : files( set<_s>({file}) )  { read_in_files(); }
//------------------------------------------------------------------------------//
//------------------------------- VARS -----------------------------------------//
//------------------------------------------------------------------------------//
// TextData - font size, family, fg, bg, text wrap
const std::map< const TT, const MakeImageNS::TextData > ParseMarkdown::text_map = {
    { TT::CODE, MakeImageNS::TextData( 12, "Noto-Mono", ColorRGB(0,   255, 0) , ColorRGB(0, 0, 0),   false ) },
    { TT::H1,   MakeImageNS::TextData( 18, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H2,   MakeImageNS::TextData( 17, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H3,   MakeImageNS::TextData( 16, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H4,   MakeImageNS::TextData( 15, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H5,   MakeImageNS::TextData( 14, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::TEXT, MakeImageNS::TextData( 12, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, 
};

const regex ParseMarkdown::block_regex = regex(
    R"((```[\n](?<CODE>.*?)[\n]```))"
    R"(|(^|\n)(?<HEADER>[#]{1,5})[ ](?<CONTENT>.*?)(\n|$))"
);

const std::array< pair<regex, _s>, 4> ParseMarkdown::inline_regex = {
    pair<regex, _s>{ regex( R"(\*{3}(.+?)\*{3})" ), "<b><i>\\1</i></b>" },
    pair<regex, _s>{ regex( R"(\*{2}(.+?)\*{2})" ), "<b>\\1</b>"        },
    pair<regex, _s>{ regex( R"(\*(.+?)\*)"       ), "<i>\\1</i>"        },
    // for inline code
    pair<regex, _s>{ regex( R"(`(.+?)`)"         ),
        "<span font_family='Noto-Mono' fgcolor='#00FF00' bgcolor='#000000' > \\1 </span>"
    },
    // pair<regex, _s>{ regex( R"(\*(.+?)\*)"       ), ""                  }
};

//------------------------------------------------------------------------------//
//------------------------------- UTILITY --------------------------------------//
//------------------------------------------------------------------------------//
std::optional<_s> ParseMarkdown::file_as_string(_s file_string) {
    std::ifstream file(file_string);
    if ( file.is_open() ) {
        std::stringstream s;
        s << file.rdbuf();
        file.close();
        return s.str();
    }
    return std::nullopt;
}

void ParseMarkdown::_read_in_files(set<_s>& f, set<_s>::iterator i, set<_s>::iterator e) {
    if (i == e) { return; }
    std::optional<_s> o = file_as_string(*i);
    // this->str_files.push_back(o);
    if (!o.has_value()) {
        std::cerr << std::format("{}: file '{}' couldn't be opened.", "_read_in_files", (*i)) << std::endl;
        exit(2);
    }
    else { this->total_str += o.value(); }
    return _read_in_files(f, ++i, e);
}

void ParseMarkdown::read_in_files() {
    // this->str_files = {};
    this->total_str = "";
    _read_in_files(this->files, this->files.begin(), this->files.end());
}

//------------------------------------------------------------------------------//
//------------------------------- CORE FUNCTIONS -------------------------------//
//------------------------------------------------------------------------------//

void ParseMarkdown::make_image(size_t image_width, size_t image_height) {
    generate_tokens();
    mimg = std::make_unique<MakeImageNS::MakeImage>(image_width, image_height);
    for (auto& [type, text] : tokens) {
        mimg->write_text(text, text_map.at(type));
    }
}
void ParseMarkdown::save_image(_s output_file) {
    if (output_file == "-") { mimg->display_image_kitty();   }
    else                    { mimg->save_image(output_file); }
}

void ParseMarkdown::generate_tokens() {
    tokens = {};
    _s::const_iterator s = total_str.begin();
    _s::const_iterator e = total_str.end();
    bmatch res;
    while (boost::regex_search(s, e, res, block_regex)) {
        bmatch n;
        // tokens.push_back(handle_inline(s, res[0].first));
        handle_inline( res.prefix() );
        if      ( res["CODE"].matched   ) { handle_code(   res ); }
        else if ( res["HEADER"].matched ) { handle_header( res ); }
        s = res[0].second;
    }
    if (s != e) {
        handle_inline( _s(s, e));
    }
}

//------------------------------------------------------------------------------//
//------------------------------- PARSERS --------------------------------------//
//------------------------------------------------------------------------------//

const regex ParseMarkdown::replace_chars = regex( "[&<>'\"]");
// THIS IS SO DUMB. PANGO IS DUMB.
_s ParseMarkdown::clean_text(_s s) {
    return regex_replace(s, replace_chars, [](const bmatch& m) -> _s {
        switch (*m[0].first) {
            case '&'  : return "&amp;amp;";
            case '<'  : return "&amp;lt;";
            case '>'  : return "&amp;gt;";
            case '\'' : return "&amp;apos;";
            case '"'  : return "&amp;quot;";
        }
        return m.str();
    });
}
void ParseMarkdown::handle_inline(_s s) {
    _s out = clean_text(s);
    for (auto& [reg, repl] : inline_regex) {
        out = boost::regex_replace(out, reg, repl);
    }
    tokens.push_back({TT::TEXT, out});
}

void ParseMarkdown::ParseMarkdown::handle_code(const bmatch& res) {
    tokens.push_back({TT::CODE, res["CODE"]});
}

void ParseMarkdown::handle_header(const bmatch& res) {
    TT htype;
    switch (res["HEADER"].length()) {
        case 1: htype = TT::H1; break;
        case 2: htype = TT::H2; break;
        case 3: htype = TT::H3; break;
        case 4: htype = TT::H4; break;
        case 5: htype = TT::H5; break;
    }
    tokens.push_back({htype, clean_text(res["CONTENT"])});
}

}
