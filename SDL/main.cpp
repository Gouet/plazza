//
// Created by veyrie_f on 4/19/16.
//

#include "WindowManagerSDL.hpp"

int main()
{
    WindowManagerSDL *win;

    try
    {
        win = new WindowManagerSDL();
    }
    catch (WindowManagerSDL::SdlException e)
    {
        std::cout << e.what() << std::endl;
    }

    win->Loop(nullptr, nullptr);
    delete win;
    return 0;
}