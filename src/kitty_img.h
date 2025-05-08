#pragma once
#include "my_makeimage.h"
#include <sys/ioctl.h>


namespace MakeImageNS {
class KittyDisplay {
    private:
        const char* terminal_name = nullptr;
        struct winsize term_size;
        void get_window_size();
    public:
        void write_image(size_t posx, size_t posy, Image f);
        bool check_terminal();
};
}
