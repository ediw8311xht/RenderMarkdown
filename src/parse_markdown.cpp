#include "parse_markdown.h"
#include "macros_defines.h"
#include <fstream>
#include <sstream> // For stringstream
#include <iostream>
#include <format>
#include <Magick++.h>

namespace {
    //----------------------------------------------------- Mine -------------------//
    using IMG = MakeImageNS::MakeImage;
    using MakeImageNS::TT;
    //----------------------------------------------------- Standard Library -------//
    using std::pair;
    using std::set;
    using std::format;
    //----------------------------------------------------- Boost ------------------//
    using boost::regex;
    using boost::smatch;
    //----------------------------------------------------- Magick++ ---------------//
    using Magick::ColorRGB;
    using Magick::Color;
}

namespace ParseMarkdownNS {

//------------------------------------------------------------------------------//
//------------------------------- VARS -----------------------------------------//
//------------------------------------------------------------------------------//
// TextData - font size, family, fg, bg, text wrap
const regex ParseMarkdown::block_regex = regex(
    R"((```[\n]?(?<CODE>.*?)[\n]?```))"
    R"(|(^|\n)(?<HEADER>[#]{1,5})[ ](?<CONTENT>.*?)(\n|$))"
    R"(|!\[(?<ALT_TEXT>.*?)\]\((?<IMAGE>.*?)\))"
    R"(|(^|\n)(?<LINE>[-]{3,}[ \t]*)(\n|$))"
);
const reg_con ParseMarkdown::inline_regex = { 
    pair<regex, _s>{ regex( R"(`(.+?)`)" ),                                 // ------- inline code
        "<span " + STYLE_INLINE_CODE + " > \\1 </span>"
    },
    pair<regex, _s>{ regex( R"(\[(.+?)\]\((.+?)\))" ),                      // ------- inline link
        "<b>\\1</b>: <span " + STYLE_INLINE_LINK + " >\\2</span>"
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

void ParseMarkdown::_read_in_files(set<_s>& f, set<_s>::iterator i, set<_s>::iterator e) {
    if (i == e) { return; }
    std::ifstream file(*i);
    if ( file.is_open() ) {
        std::stringstream s;
        s << file.rdbuf();
        file.close();
        this->total_str += s.str();
    } else {
        std::cerr << format("{}: file '{}' couldn't be opened.", "_read_in_files", (*i)) << std::endl;
        exit(3);
    }
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
void ParseMarkdown::save_image(IMG& mimg, _s output_file) {
    if (output_file == "-") { mimg.display_image_kitty();   }
    else                    { mimg.save_image(output_file); }
}

void ParseMarkdown::create_image(IMG& mimg) {
    _s::const_iterator s = total_str.begin();
    _s::const_iterator e = total_str.end();
    smatch res;
    while (boost::regex_search(s, e, res, block_regex)) {
        smatch n;
        handle_inline(mimg, res.prefix() );
        if      ( res["CODE"].matched   ) { handle_code(   mimg, res ); }
        else if ( res["HEADER"].matched ) { handle_header( mimg, res ); }
        else if ( res["IMAGE"].matched  ) { handle_image(  mimg, res ); }
        else if ( res["LINE"].matched   ) { handle_line(   mimg, res ); }
        s = res[0].second;
    }
    if (s != e) {
        handle_inline(mimg, _s(s, e));
    }
}

//------------------------------------------------------------------------------//
//------------------------------- PARSERS --------------------------------------//
//------------------------------------------------------------------------------//

/* ------------- BLOCK -------------- */
void ParseMarkdown::handle_code(IMG& mimg, const smatch& res) {
    mimg.add_text_to_canvas(res["CODE"], TT::CODE);
}

void ParseMarkdown::handle_header(IMG& mimg, const smatch& res) {
    TT htype;
    switch (res["HEADER"].length()) {
        case 1: htype = TT::H1; break;
        case 2: htype = TT::H2; break;
        case 3: htype = TT::H3; break;
        case 4: htype = TT::H4; break;
        case 5: htype = TT::H5; break;
    }
    mimg.add_text_to_canvas(clean_text(res["CONTENT"]), htype);
}

void ParseMarkdown::handle_image(IMG& mimg, const smatch& res) {
    MakeImageNS::ImageData sub_img( res["IMAGE"] , res["ALT_TEXT"] );
    mimg.add_image_to_canvas(sub_img);
}

void ParseMarkdown::handle_line(IMG& mimg, const smatch& res) {
    mimg.add_line_to_canvas();
}

/* ------------- INLINE-------------- */
/* Special characters need to be replaced, but `&amp;` gets turned into `&`
   So this is the work around */
_s ParseMarkdown::clean_text(_s s) {
    return regex_replace(s, replace_chars, [](const smatch& m) -> _s {
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
void ParseMarkdown::handle_inline(IMG& mimg, _s s) {
    _s out = clean_text(s);
    for (auto& [reg, repl] : inline_regex) {
        out = boost::regex_replace(out, reg, repl);
    }
    mimg.add_text_to_canvas(out, TT::INLINE);
}
}
