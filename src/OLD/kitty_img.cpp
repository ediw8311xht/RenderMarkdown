#include "kitty_img.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using MakeImageNS::KittyDisplay;
using MakeImageNS::Image;


bool KittyDisplay::check_terminal() {
    if (terminal_name == nullptr) {
        terminal_name = ttyname(STDIN_FILENO);
    }
    return true;
}
void KittyDisplay::get_window_size() {
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term_size);
}

void KittyDisplay::write_image(size_t posx, size_t posy, Image f) {
}


