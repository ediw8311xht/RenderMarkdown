#include "parse_markdown.h"
#include "macros_defines.h"
#include <iostream>
#include <format>
#include <fstream>
#include <Magick++.h>
#include <sstream> // For stringstream

//----------------------------------------------------- Mine -------------------//
using TT = ParseMarkdownNS::TokenType;
//----------------------------------------------------- Standard Library -------//
using std::pair;
using std::set;
using std::format;
//----------------------------------------------------- Boost ------------------//
using boost::regex;
using bmatch = boost::match_results<_s::const_iterator>;
//----------------------------------------------------- Magick++ ---------------//
using Magick::ColorRGB;
using Magick::Color;

namespace ParseMarkdownNS {

//------------------------------------------------------------------------------//
//------------------------------- VARS -----------------------------------------//
//------------------------------------------------------------------------------//
// TextData - font size, family, fg, bg, text wrap
const std::map< const TT, const MakeImageNS::TextData > ParseMarkdown::text_map = {
    { TT::CODE,   MakeImageNS::TextData( 12, "Noto-Mono", ColorRGB(0,   255, 0) , ColorRGB(0, 0, 0),    false ) },
    { TT::H1,     MakeImageNS::TextData( 18, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H2,     MakeImageNS::TextData( 17, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H3,     MakeImageNS::TextData( 16, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H4,     MakeImageNS::TextData( 15, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::H5,     MakeImageNS::TextData( 14, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) },
    { TT::INLINE, MakeImageNS::TextData( 12, "Noto-Sans", ColorRGB(0,   0,   0) , Color("transparent"), true  ) }, 
};
const regex ParseMarkdown::block_regex = regex(
    R"((```[\n]?(?<CODE>.*?)[\n]?```))"
    R"(|(^|\n)(?<HEADER>[#]{1,5})[ ](?<CONTENT>.*?)(\n|$))"
    R"(|!\[(?<ALT_TEXT>.*?)\]\((?<IMAGE>.*?)\))"
    R"(|(^|\n)(?<LINE>[-]{3,}[ \t]*)(\n|$))"
);
const reg_con ParseMarkdown::inline_regex = { 
    pair<regex, _s>{ regex( R"(`(.+?)`)" ),                                 // ------- inline code
        "<span " STYLE_INLINE_CODE " > \\1 </span>"
    },
    pair<regex, _s>{ regex( R"(\[(.+?)\]\((.+?)\))" ),                      // ------- inline link
        "<b>\\1</b>: <span " STYLE_INLINE_LINK " >\\2</span>"
    },
    pair<regex, _s>{ regex( R"(\*{3}(.+?)\*{3})" ), "<b><i>\\1</i></b>" } , // ------- bold & italic
    pair<regex, _s>{ regex( R"(\*{2}(.+?)\*{2})" ), "<b>\\1</b>"        } , // ------- bold
    pair<regex, _s>{ regex( R"(\*(.+?)\*)"       ), "<i>\\1</i>"        } , // ------- italic
};
const regex ParseMarkdown::replace_chars = regex( "[&<>'\"%]" );

//------------------------------------------------------------------------------//
//------------------------------- CONSTRUCTORS ---------------------------------//
//------------------------------------------------------------------------------//
ParseMarkdown::ParseMarkdown(set<_s> files) : files( files )            { read_in_files(); }
ParseMarkdown::ParseMarkdown(_s file)       : files( set<_s>({file}) )  { read_in_files(); }

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
        std::cerr << format("{}: file '{}' couldn't be opened.", "_read_in_files", (*i)) << std::endl;
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
void ParseMarkdown::save_image(_s output_file) {
    if (output_file == "-") { mimg->display_image_kitty();   }
    else                    { mimg->save_image(output_file); }
}

void ParseMarkdown::create_image(size_t image_width, size_t image_height) {
    _s::const_iterator s = total_str.begin();
    _s::const_iterator e = total_str.end();
    bmatch res;
    mimg = std::make_unique<MakeImageNS::MakeImage>(image_width, image_height);
    while (boost::regex_search(s, e, res, block_regex)) {
        bmatch n;
        handle_inline( res.prefix() );
        if      ( res["CODE"].matched   ) { handle_code(   res ); }
        else if ( res["HEADER"].matched ) { handle_header( res ); }
        else if ( res["IMAGE"].matched  ) { handle_image( res );  }
        else if ( res["LINE"].matched   ) { handle_line( res );   }
        s = res[0].second;
    }
    if (s != e) {
        handle_inline( _s(s, e));
    }
}

//------------------------------------------------------------------------------//
//------------------------------- PARSERS --------------------------------------//
//------------------------------------------------------------------------------//

/* ------------- BLOCK -------------- */
void ParseMarkdown::ParseMarkdown::handle_code(const bmatch& res) {
    mimg->add_text_to_canvas(res["CODE"], text_map.at(TT::CODE));
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
    mimg->add_text_to_canvas(clean_text(res["CONTENT"]), text_map.at(htype));
}

void ParseMarkdown::handle_image(const bmatch& res) {
    MakeImageNS::ImageData sub_img( res["IMAGE"] , res["ALT_TEXT"] );
    mimg->add_image_to_canvas(sub_img);
}

void ParseMarkdown::handle_line(const bmatch& res) {
    mimg->add_line_to_canvas();
}

/* ------------- INLINE-------------- */
/* Special characters need to be replaced, but `&amp;` gets turned into `&`
   So this is the work around */
_s ParseMarkdown::clean_text(_s s) {
    return regex_replace(s, replace_chars, [](const bmatch& m) -> _s {
        switch (*m[0].first) {
            case '&'  : return "&amp;amp;";
            case '<'  : return "&amp;lt;";
            case '>'  : return "&amp;gt;";
            case '\'' : return "&amp;apos;";
            case '%'  : return "&amp;#37;";
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
    mimg->add_text_to_canvas(out, text_map.at(TT::INLINE));
}
}
