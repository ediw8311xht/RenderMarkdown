#pragma once

#include "my_make_image.h"
#include <vector>
#include <set>
#include <string>
#include <optional>
#include <map>
#include <boost/regex.hpp>

namespace ParseMarkdownNS {

// Declare before defining.
typedef struct Token Token;
enum class TokenType : int;
class ParseMarkdown;

enum class TokenType : int {
    NONE = -1   ,
    CODE = 0    ,
    H1=1, H2=2, H3=3, H4=4, H5=5,
    TEXT=6      ,
    END=7       ,
};

struct Token {
    TokenType   type = TokenType::NONE;
    std::string text = "";

    Token(std::string text) : text(text) {}
    Token(TokenType type=TokenType::TEXT, std::string text="", MakeImageNS::TextData text_data = MakeImageNS::TextData())
        : type(type), text(text) {}
};

class ParseMarkdown {
    private:
//----------------------------- FORMATTING ----------------------------------------//
        static const std::map< const TokenType, const MakeImageNS::TextData > text_map;
//----------------------------- IMAGE ---------------------------------------------//
        std::unique_ptr<MakeImageNS::MakeImage> mimg;
//----------------------------- Maps
        // static const regex full_regex;
        static const boost::regex replace_chars;
        static const boost::regex block_regex;
        static const std::array< std::pair<boost::regex, std::string>, 4> inline_regex;

//----------------------------- Vars
        // Names of markdown files
        std::set<std::string> files = {};
        // Lines of files
        std::set<std::optional<std::string>> str_files;
        // Concenated contents of all files
        std::string total_str; 
        std::vector<Token> tokens = {};

//----------------------------- Functions
        std::optional<std::string> file_as_string(std::string file_string);
        // Internal function for read_in_files()
        // optional<string> so value returned can indicate when there is an
        // issue with opening the file.
        void _read_in_files( std::set<std::string>& f,
                             std::set<std::string>::iterator i,
                             std::set<std::string>::iterator e );
        void handle_code(   const boost::match_results<std::string::const_iterator>& res );
        void handle_header( const boost::match_results<std::string::const_iterator>& res );
        void generate_tokens();
        // Text must be cleaned since both pango and Magick++ use certain characters for formatting 
        std::string clean_text(std::string s);
        // Inline for formatting that is done within the text using pango
        void handle_inline(std::string s);
    public:
        // Explicit since reading in files happens when intialized.
        explicit ParseMarkdown(std::set<std::string> files={});
        explicit ParseMarkdown(std::string file);

        void read_in_files();
        void make_image(size_t image_width=1000, size_t image_height=800);
        void save_image(std::string output_file);
};

 
}
