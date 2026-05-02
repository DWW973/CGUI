#ifndef CGUI_WINDOW_H
#define CGUI_WINDOW_H

#include "string.hpp"
#include "logger.hpp"
#include "computer_info.h"
#include "config.hpp"
#include "enum.hpp"
// 条件包含Windows API头文件
#ifdef _WIN32
#include <windows.h>
#endif

namespace cgui {

// 获取唯一ID的函数
/**
 * @brief GetOnlyId - 获取唯一标识符
 * @param name 基础名称
 * @return 唯一标识符
 */
template<typename T>
String GetOnlyId(const String& name);

// 获取唯一文件名的函数
/**
 * @brief GetOnlyFile - 获取唯一文件名
 * @param base_name 基础文件名
 * @param extension 文件扩展名（可选）
 * @return 唯一文件名
 */
String GetOnlyFile(const String& base_name, const String& extension = "");

// 平台相关的窗口句柄类型
#ifdef _WIN32
using NativeWindowHandle = HWND; // Windows平台使用HWND
#else
using NativeWindowHandle = void*; // 其他平台使用void*
#endif



// 私有资源结构体
/**
 * @brief WinRes - 窗口私有资源
 * 
 * 这个结构体包含了窗口运行时所需的所有私有数据。
 * 用户不应该直接操作这些字段，而是通过Window类的公共接口来操作。
 */
struct WinRes {
    String title;               // 窗口标题
    int width;                   // 窗口宽度
    int height;                  // 窗口高度
    bool is_open;                // 窗口是否打开
    bool should_close_flag;      // 窗口是否应该关闭的标志
    String id;                   // 窗口唯一ID
    Logger logger;               // 内置日志器

    /**
     * @brief WinRes构造函数
     * @param id 窗口唯一ID
     */
    WinRes(const String& id);
    /**
     * @brief WinRes析构函数
     */
    ~WinRes();
};

/**
 * @class Window
 * @brief 窗口类 - 用于创建和管理窗口
 * 
 * 这个类提供了跨平台的窗口创建和管理功能。
 * 支持窗口创建、关闭、更新、设置标题、调整大小等基本操作。
 * 
 * @par 使用示例
 * @code
 * // 基本使用
 * Window mainWindow;
 * 
 * // 创建窗口
 * if (mainWindow.start_window("我的窗口", 800, 600, CguiWindowStyle::Default) == CguiStautsCode::SUCCESS) {
 * 
 *     // 主循环
 *     while (mainWindow.is_window_open() && !mainWindow.should_close()) {
 *         mainWindow.update();
 *         // 处理其他逻辑
 *     }
 *     // 关闭窗口
 *     mainWindow.close();
 * }
 * 
 * // 子窗口
 * Window childWindow(&mainWindow);
 * childWindow.start_window("子窗口", 400, 300, CguiWindowStyle::Dialog);
 * @endcode
 */
class Window {
private:
    WinRes res; // 私有资源实例

    // Pimpl模式，隐藏实现细节
    class Impl;
    Impl* pImpl;

    Window* parent; // 父窗口指针

public:
    /**
     * @brief Window构造函数 - 不带父窗口
     * @param parent 父窗口指针（可选）
     */
    Window(Window* parent = nullptr);
    /**
     * @brief Window构造函数 - 带父窗口引用
     * @param parent 父窗口引用
     */
    Window(Window& parent);
    /**
     * @brief Window析构函数
     * 会自动关闭窗口并释放资源
     */
    ~Window();

    /**
     * @brief start_window - 启动/创建窗口
     * @param title 窗口标题
     * @param width 窗口宽度
     * @param height 窗口高度
     * @param style 窗口样式（默认CguiWindowStyle::Default）
     * @return 操作状态码（CguiStautsCode）
     */
    CguiStautsCode start_window(const String& title = DEFAULT_WINDOW_TITLE, int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT, int style=CguiWindowStyle::Default);
    
    /**
     * @brief close - 关闭窗口
     */
    void close();
    
    /**
     * @brief update - 更新窗口（处理消息、重绘等）
     * 通常在主循环中调用
     */
    void update();
    
    /**
     * @brief is_window_open - 检查窗口是否打开
     * @return 如果窗口打开返回true，否则false
     */
    bool is_window_open() const;

    /**
     * @brief set_title - 设置窗口标题
     * @param title 新的窗口标题
     */
    void set_title(const String& title);
    
    /**
     * @brief get_title - 获取当前窗口标题
     * @return 当前窗口标题
     */
    String get_title() const;

    /**
     * @brief set_size - 设置窗口大小
     * @param width 新宽度
     * @param height 新高度
     */
    void set_size(int width, int height);
    
    /**
     * @brief get_width - 获取窗口宽度
     * @return 当前窗口宽度
     */
    int get_width() const;
    
    /**
     * @brief get_height - 获取窗口高度
     * @return 当前窗口高度
     */
    int get_height() const;

    /**
     * @brief should_close - 检查窗口是否应该关闭
     * @return 如果窗口应该关闭返回true，否则false
     */
    bool should_close() const;

    /**
     * @brief get_id - 获取窗口的唯一ID
     * @return 窗口唯一ID（字符串）
     */
    const String& get_id() const;

    /**
     * @brief set_open - 设置窗口打开状态（内部使用）
     * @param open 新的打开状态
     */
    void set_open(bool open);

    /**
     * @brief set_should_close - 设置窗口关闭标志（内部使用）
     * @param should_close 新的关闭标志
     */
    void set_should_close(bool should_close);

    /**
     * @brief get_handle - 获取原始窗口句柄（平台特定类型）
     * @return 窗口句柄（Windows上是HWND，其他平台是void*）
     * @note 这是一个高级功能，慎用
     */
    NativeWindowHandle get_handle() const;
};

} // namespace cgui

#endif // CGUI_WINDOW_H
