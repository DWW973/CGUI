#include "../../include/window.h"
#include <windows.h>

namespace cgui {

DWORD MakeWindowStyle(unsigned int style) {
    DWORD window_style = 0;

    if(style & CguiWindowStyle::Default){
        window_style |= (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
    }
    if(style & CguiWindowStyle::Borderless){
        window_style |= WS_POPUP;
    }
    if(style & CguiWindowStyle::Resizable){
        window_style |= WS_THICKFRAME;
    }
    if(style & CguiWindowStyle::FixedSize){
        window_style |= WS_BORDER;
    }
    if(style & CguiWindowStyle::NoTitlebar){
        window_style |= WS_POPUP;
    }
    if(style & CguiWindowStyle::AlwaysOnTop){
        // 始终置顶通过SetWindowPos实现
    }
    if(style & CguiWindowStyle::Minimizable){
        window_style |= WS_MINIMIZEBOX;
    }
    if(style & CguiWindowStyle::Maximizable){
        window_style |= WS_MAXIMIZEBOX;
    }
    if(style & CguiWindowStyle::CloseButton){
        window_style |= WS_SYSMENU;
    }
    if(style & CguiWindowStyle::Disabled){
        window_style |= WS_DISABLED;
    }
    if(style & CguiWindowStyle::Visible){
        window_style |= WS_VISIBLE;
    }
    if(style & CguiWindowStyle::Popup){
        window_style |= WS_POPUP;
    }
    if(style & CguiWindowStyle::Dialog){
        window_style |= (WS_OVERLAPPED | WS_CAPTION | WS_DLGFRAME | WS_SYSMENU);
    }
    if(style & CguiWindowStyle::HorizontalScroll){
        window_style |= WS_HSCROLL;
    }
    if(style & CguiWindowStyle::VerticalScroll){
        window_style |= WS_VSCROLL;
    }
    if(style & CguiWindowStyle::SystemMenu){
        window_style |= WS_SYSMENU;
    }
    if(style & CguiWindowStyle::ThickFrame){
        window_style |= WS_THICKFRAME;
    }

    if(window_style == 0){
        window_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }

    return window_style;
}

} // namespace cgui
