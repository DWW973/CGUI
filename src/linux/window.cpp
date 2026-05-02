#include "../../include/window.h"
#include <iostream>

namespace cgui {

class Window::Impl {
public:
    // Linux-specific window handle
    void* window_handle = nullptr;

    ~Impl() {
        // Linux-specific cleanup code
    }
};

Window::Window()
    : title(""), width(0), height(0), is_open(false), should_close_flag(false), id(GetOnlyId<Window>("window")), pImpl(new Impl()) {
    LOG_INFO_FMT("Creating Linux window: title='%s', width=%d, height=%d, id='%s'", title.c_str(), width, height, id.c_str());
}

Window::~Window() {
    if (is_open) {
        close();
    }
    
    delete pImpl;
    LOG_INFO("Destroying Linux window");
}

CguiStautsCode Window::open(int width, int height, const String& title) {
    if (is_open) {
        LOG_WARNING("Attempting to open an already open window");
        return CguiStautsCode::WINDOW_ALREADY_OPEN;
    }
    
    // 更新窗口属性
    this->width = width;
    this->height = height;
    this->title = title;
    
    // Linux-specific window creation code here (e.g., X11, Wayland)
    is_open = true;
    LOG_INFO_FMT("Opening Linux window: title='%s', width=%d, height=%d", title.c_str(), width, height);
    return CguiStautsCode::SUCCESS;
}

void Window::close() {
    if (is_open) {
        is_open = false;
        LOG_INFO_FMT("Closing Linux window: title='%s'", title.c_str());
        // Linux-specific window closing code here
    } else {
        LOG_WARNING("Attempting to close an already closed window");
    }
}

void Window::update() {
    if (is_open) {
        LOG_DEBUG_FMT("Updating Linux window: title='%s'", title.c_str());
        // Linux-specific window update code here
    } else {
        LOG_WARNING("Attempting to update a closed window");
    }
}

bool Window::is_window_open() const {
    return is_open;
}

void Window::set_title(const String& title) {
    String old_title = this->title;
    this->title = title;
    LOG_INFO_FMT("Setting Linux window title: old='%s', new='%s'", old_title.c_str(), title.c_str());
    // Linux-specific title update code here
}

String Window::get_title() const {
    return title;
}

void Window::set_size(int width, int height) {
    int old_width = this->width;
    int old_height = this->height;
    this->width = width;
    this->height = height;
    LOG_INFO_FMT("Setting Linux window size: old=%dx%d, new=%dx%d", old_width, old_height, width, height);
    // Linux-specific size update code here
}

int Window::get_width() const {
    return width;
}

int Window::get_height() const {
    return height;
}

// 检查窗口是否应该关闭
bool Window::should_close() const {
    return should_close_flag;
}

// 用于内部设置窗口关闭标志
void Window::set_should_close(bool should_close) {
    should_close_flag = should_close;
}

// 获取窗口的唯一ID
const String& Window::get_id() const {
    return id;
}

} // namespace cgui