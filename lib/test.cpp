#include "../include/window.h"
using namespace cgui;
int main()
{
    Window window;
    window.start_window();
    while(!window.should_close())
    {
        window.update();
        Sleep(100);
    }
    return 0;
}