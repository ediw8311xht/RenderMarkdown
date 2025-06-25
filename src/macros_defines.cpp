#include "macros_defines.h"
#include <fstream>
#include <sstream> // For stringstream

std::optional<_s> file_as_string(_s file_string) {
    std::ifstream file(file_string);
    if ( file.is_open() ) {
        std::stringstream s;
        s << file.rdbuf();
        file.close();
        return s.str();
    }
    return std::nullopt;
}
