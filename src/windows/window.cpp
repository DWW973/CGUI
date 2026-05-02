#include "../../include/window.h"
#include <iostream>
#include <windows.h>
#include "system.cpp"

namespace cgui {

// 全局窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 声明 MakeWindowStyle 函数
extern DWORD MakeWindowStyle(unsigned int style);

// 生成唯一的窗口类名
static const wchar_t* GetOnlyClassName() {
    static std::wstring class_name;
    if (class_name.empty()) {
        // 使用当前时间戳生成唯一类名
        time_t now = time(nullptr);
        struct tm local_time;
        #if defined(_WIN32) || defined(_WIN64)
            localtime_s(&local_time, &now); // Windows线程安全版本
        #else
            localtime_r(&now, &local_time); // POSIX线程安全版本
        #endif
        wchar_t buffer[64];
        swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"CGUI_WindowClass_%04d%02d%02d_%02d%02d%02d",
                 local_time.tm_year + 1900, local_time.tm_mon + 1, local_time.tm_mday,
                 local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
        class_name = buffer;
    }
    return class_name.c_str();
}

class Window::Impl {
public:
    HWND hwnd = nullptr;
    HWND parent_hwnd = nullptr; // 父窗口句柄
    HINSTANCE hInstance = nullptr;
    bool registered = false;
    std::wstring class_name; // 存储窗口类名

    // 窗口过程函数
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    ~Impl() {
        if (hwnd) {
            DestroyWindow(hwnd);
        }
    }
};

WinRes::WinRes(const String& id)
    : title(""), width(0), height(0), is_open(false), should_close_flag(false), id(id), logger("window-" + id) {
    logger.info("Creating Windows window");
    logger.info("Window properties: title='%s', width=%d, height=%d, id='%s'", title.c_str(), width, height, id.c_str());
}

WinRes::~WinRes() {
    logger.info("Destroying Windows window");
    logger.info("Window destroyed");
}

Window::Window(Window* parent)
    : res(GetOnlyId<Window>("window")), pImpl(new Impl()), parent(parent) {
    res.logger.info("Creating Windows window");
    res.logger.info("Window properties: title='%s', width=%d, height=%d, id='%s'", res.title.c_str(), res.width, res.height, res.id.c_str());
    if (parent) {
        res.logger.info("Parent window set");
    }

    // 获取当前实例
    pImpl->hInstance = GetModuleHandle(nullptr);
    res.logger.debug("Got module handle");

    // 生成并存储唯一的窗口类名
    pImpl->class_name = GetOnlyClassName();
    res.logger.debug("Generated window class name: %ls", pImpl->class_name.c_str());

    // 注册窗口类
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEXW) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Impl::WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = pImpl->hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = pImpl->class_name.c_str();
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (RegisterClassExW(&wcex)) {
        pImpl->registered = true;
        res.logger.info("Window class registered successfully: %ls", pImpl->class_name.c_str());
    } else {
        res.logger.error("Failed to register window class: %d", GetLastError());
    }
}

Window::Window(Window& parent)
    : Window(&parent) {
}

Window::~Window() {
    res.logger.info("Destroying Windows window");

    if (res.is_open) {
        res.logger.debug("Closing window before destruction");
        close();
    }

    // 注销窗口类
    if (pImpl->registered && !pImpl->class_name.empty()) {
        UnregisterClassW(pImpl->class_name.c_str(), pImpl->hInstance);
        res.logger.info("Window class unregistered: %ls", pImpl->class_name.c_str());
    }

    delete pImpl;
    res.logger.info("Window destroyed");
}

CguiStautsCode Window::start_window(const String& title, int width, int height, int style) {
    res.logger.info("Starting window");

    res.logger.info("Start parameters: title='%s', width=%d, height=%d, style=%d", title.c_str(), width, height, style);

    if (res.is_open) {
        res.logger.warning("Attempting to start an already open window");
        return CguiStautsCode::WINDOW_ALREADY_OPEN;
    }

    if (!pImpl->registered) {
        res.logger.error("Window class not registered, cannot create window");
        return CguiStautsCode::WINDOW_CLASS_REGISTER_FAILED;
    }

    // 检查是否需要设置为最大尺寸
    cgui::Screen screen = cgui::ComputerInfo::get_screen_info();

    if (width == FULLWIDTH) {
        width = screen.width;
        res.logger.info("Setting width to maximum: %d", width);
    }

    if (height == FULLHEIGHT) {
        height = screen.height;
        res.logger.info("Setting height to maximum: %d", height);
    }

    // 更新窗口属性
    res.title = title;
    res.width = width;
    res.height = height;

    // 转换标题为宽字符
    int title_len = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0);
    if (title_len == 0) {
        res.logger.error("Failed to calculate wide string length: %d", GetLastError());
        // 使用默认标题作为 fallback
        title_len = MultiByteToWideChar(CP_UTF8, 0, DEFAULT_WINDOW_TITLE, -1, nullptr, 0);
        if (title_len == 0) {
            res.logger.error("Failed to calculate default wide string length: %d", GetLastError());
            // 使用空字符串作为最后的 fallback
            title_len = 1;
        }
    }
    std::wstring wide_title(title_len, L'\0');
    int result = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wide_title[0], title_len);
    if (result == 0) {
        res.logger.error("Failed to convert title to wide string: %d", GetLastError());
        // 使用默认标题作为 fallback
        result = MultiByteToWideChar(CP_UTF8, 0, DEFAULT_WINDOW_TITLE, -1, &wide_title[0], title_len);
        if (result == 0) {
            res.logger.error("Failed to convert default title to wide string: %d", GetLastError());
            // 使用空字符串作为最后的 fallback
            wide_title = L"";
        }
    }

    // 创建窗口
    res.logger.debug("Creating window");

    // 使用MakeWindowStyle函数转换窗口样式
    DWORD window_style = MakeWindowStyle(style);

    // 全屏模式标记
    bool is_fullscreen = (width == screen.width && height == screen.height);

    // 获取父窗口句柄
    HWND parent_hwnd = nullptr;
    if (this->parent && this->parent->is_window_open()) {
        parent_hwnd = static_cast<HWND>(this->parent->get_handle());
    }

    pImpl->hwnd = CreateWindowExW(
        0,                              // 扩展样式
        pImpl->class_name.c_str(),       // 窗口类名
        wide_title.c_str(),              // 窗口标题
        window_style,                    // 窗口样式
        0, 0,                           // 位置
        width,                           // 宽度
        height,                          // 高度
        parent_hwnd,                     // 父窗口
        nullptr,                         // 菜单
        pImpl->hInstance,                // 实例
        this                             // 用户数据
    );

    if (pImpl->hwnd) {
        // 显示窗口
        int showCmd = is_fullscreen ? SW_MAXIMIZE : SW_SHOW;
        ShowWindow(pImpl->hwnd, showCmd);
        UpdateWindow(pImpl->hwnd);

        res.is_open = true;
        res.logger.info("Window started successfully: title='%s', width=%d, height=%d", title.c_str(), width, height);
        return CguiStautsCode::SUCCESS;
    } else {
        res.logger.error("Failed to create window: %d", GetLastError());
        return CguiStautsCode::WINDOW_CREATE_FAILED;
    }
}

void Window::close() {
    res.logger.info("Closing window");

    if (res.is_open && pImpl->hwnd) {
        res.logger.debug("Destroying window with title: '%s'", res.title.c_str());
        DestroyWindow(pImpl->hwnd);
        pImpl->hwnd = nullptr;
        res.is_open = false;
        res.logger.info("Window closed successfully: title='%s'", res.title.c_str());
    } else {
        res.logger.warning("Attempting to close an already closed window");
    }
}

void Window::update() {
    res.logger.debug("Updating window");

    if (res.is_open && pImpl->hwnd) {
        // 处理窗口消息
        MSG msg;
        while (PeekMessage(&msg, pImpl->hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        res.logger.debug("Window updated: title='%s'", res.title.c_str());
    } else {
        res.logger.warning("Attempting to update a closed window");
    }
}

bool Window::is_window_open() const {
    return res.is_open;
}

void Window::set_title(const String& title) {
    res.logger.info("Setting window title");
    String old_title = res.title;
    res.title = title;

    if (res.is_open && pImpl->hwnd) {
        // 转换标题为宽字符
        int title_len = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0);
        std::wstring wide_title(title_len, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wide_title[0], title_len);

        SetWindowTextW(pImpl->hwnd, wide_title.c_str());
        res.logger.debug("Window title updated in OS: old='%s', new='%s'", old_title.c_str(), title.c_str());
    }

    res.logger.info("Window title set: old='%s', new='%s'", old_title.c_str(), title.c_str());
}

String Window::get_title() const {
    return res.title;
}

void Window::set_size(int width, int height) {
    res.logger.info("Setting window size");
    int old_width = res.width;
    int old_height = res.height;
    res.width = width;
    res.height = height;

    if (res.is_open && pImpl->hwnd) {
        SetWindowPos(pImpl->hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
        res.logger.debug("Window size updated in OS: old=%dx%d, new=%dx%d", old_width, old_height, width, height);
    }

    res.logger.info("Window size set: old=%dx%d, new=%dx%d", old_width, old_height, width, height);
}

int Window::get_width() const {
    res.logger.debug("Getting window width: %d", res.width);
    return res.width;
}

int Window::get_height() const {
    res.logger.debug("Getting window height: %d", res.height);
    return res.height;
}

// 检查窗口是否应该关闭
bool Window::should_close() const {
    res.logger.debug("Checking if window should close: %s", res.should_close_flag ? "true" : "false");
    return res.should_close_flag;
}

// 用于内部设置窗口状态
void Window::set_open(bool open) {
    bool old_state = res.is_open;
    res.is_open = open;
    res.logger.info("Window open state changed: old=%s, new=%s", old_state ? "true" : "false", open ? "true" : "false");
}

// 用于内部设置窗口关闭标志
void Window::set_should_close(bool should_close) {
    bool old_state = res.should_close_flag;
    res.should_close_flag = should_close;
    res.logger.info("Window close flag changed: old=%s, new=%s", old_state ? "true" : "false", should_close ? "true" : "false");
}

// 获取窗口的唯一ID
const String& Window::get_id() const {
    res.logger.debug("Getting window ID: %s", res.id.c_str());
    return res.id;
}

// 获取原始窗口句柄
NativeWindowHandle Window::get_handle() const {
    return pImpl->hwnd;
}

// 窗口过程函数
LRESULT CALLBACK Window::Impl::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = nullptr;

    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    switch (uMsg) {
    case WM_DESTROY:
        if (window) {
            window->set_open(false);
            window->set_should_close(true);
        }
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        if (window) {
            window->close();
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

} // namespace cgui
