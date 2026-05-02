#ifndef CGUI_ENUM_HPP
#define CGUI_ENUM_HPP

namespace cgui {

// 状态码枚举
/**
 * @brief CguiStautsCode - 操作状态码
 * 
 * | 状态码 | 值 | 说明 |
 * |--------|----|------|
 * | SUCCESS | 0 | 操作成功完成 |
 * | WINDOW_ALREADY_OPEN | 1 | 尝试打开一个已经打开的窗口 |
 * | WINDOW_CLASS_REGISTER_FAILED | 2 | 窗口类注册失败 |
 * | WINDOW_CREATE_FAILED | 3 | 窗口创建失败 |
 * | UNKNOWN_ERROR | 4 | 未知错误 |
 * 
 * @par 使用示例
 * @code
 * Window window;
 * auto result = window.start_window("测试", 400, 300);
 * 
 * if (result == CguiStautsCode::SUCCESS) {
 *     // 成功，继续操作
 *     window.update();
 * } else {
 *     // 处理错误
 *     std::cerr << "窗口创建失败" << std::endl;
 * }
 * @endcode
 */
enum class CguiStautsCode {
    SUCCESS = 0,           // 成功
    WINDOW_ALREADY_OPEN,    // 窗口已经打开
    WINDOW_CLASS_REGISTER_FAILED, // 窗口类注册失败
    WINDOW_CREATE_FAILED,   // 窗口创建失败
    UNKNOWN_ERROR          // 未知错误
};

// 窗口样式类（单个窗口专用）
/**
 * @brief CguiWindowStyle - 窗口样式枚举
 * 
 * | 样式名称 | 位值 | 十六进制值 | 说明 | Windows等效 |
 * |---------|------|------------|------|-------------|
 * | Default | 0 | 0x00000000 | 默认窗口样式（标题栏 + 边框 + 系统菜单 + 最小化/最大化按钮） | WS_OVERLAPPEDWINDOW |
 * | Borderless | 1 | 0x00000001 | 无边框窗口（没有边框和标题栏） | WS_POPUP |
 * | Resizable | 2 | 0x00000002 | 可调整大小的窗口（粗边框） | WS_THICKFRAME |
 * | FixedSize | 4 | 0x00000004 | 固定大小的窗口（细边框） | WS_BORDER |
 * | NoTitlebar | 8 | 0x00000008 | 无标题栏窗口 | WS_POPUP |
 * | AlwaysOnTop | 16 | 0x00000010 | 始终置顶窗口（需要额外调用SetWindowPos实现） | 需通过API设置 |
 * | Minimizable | 32 | 0x00000020 | 可最小化窗口（显示最小化按钮） | WS_MINIMIZEBOX |
 * | Maximizable | 64 | 0x00000040 | 可最大化窗口（显示最大化按钮） | WS_MAXIMIZEBOX |
 * | CloseButton | 128 | 0x00000080 | 显示关闭按钮 | WS_SYSMENU |
 * | Disabled | 256 | 0x00000100 | 禁用窗口（用户无法交互） | WS_DISABLED |
 * | Visible | 512 | 0x00000200 | 窗口创建后立即可见 | WS_VISIBLE |
 * | Popup | 1024 | 0x00000400 | 弹出窗口样式 | WS_POPUP |
 * | Dialog | 2048 | 0x00000800 | 对话框样式（标题栏 + 对话框边框） | WS_OVERLAPPED \| WS_CAPTION \| WS_DLGFRAME |
 * | HorizontalScroll | 4096 | 0x00001000 | 显示水平滚动条 | WS_HSCROLL |
 * | VerticalScroll | 8192 | 0x00002000 | 显示垂直滚动条 | WS_VSCROLL |
 * | SystemMenu | 16384 | 0x00004000 | 显示系统菜单（标题栏左侧） | WS_SYSMENU |
 * | ThickFrame | 32768 | 0x00008000 | 粗边框（可调整大小） | WS_THICKFRAME |
 * 
 * @par 样式组合指南
 * 可以使用位或运算（|）组合多个样式：
 * @code
 * // 可调整大小的对话框
 * int style = CguiWindowStyle::Dialog | CguiWindowStyle::Resizable;
 * 
 * // 无边框且置顶的窗口
 * int style = CguiWindowStyle::Borderless | CguiWindowStyle::AlwaysOnTop;
 * 
 * // 带滚动条的窗口
 * int style = CguiWindowStyle::Default | 
 *             CguiWindowStyle::HorizontalScroll | 
 *             CguiWindowStyle::VerticalScroll;
 * @endcode
 * 
 * @par 使用示例
 * @code
 * // 示例1：单个样式 - 无边框窗口
 * Window borderlessWindow;
 * borderlessWindow.start_window("无边框", 400, 300, CguiWindowStyle::Borderless);
 * 
 * // 示例2：组合样式 - 可调整大小的对话框
 * Window dialog;
 * dialog.start_window("对话框", 300, 200, 
 *     CguiWindowStyle::Dialog | CguiWindowStyle::Resizable | CguiWindowStyle::CloseButton);
 * 
 * // 示例3：默认样式 - 标准窗口
 * Window mainWindow;
 * mainWindow.start_window("主窗口", 640, 480, CguiWindowStyle::Default);
 * 
 * // 示例4：带滚动条的窗口
 * Window scrollWindow;
 * scrollWindow.start_window("滚动窗口", 800, 600, 
 *     CguiWindowStyle::Default | CguiWindowStyle::HorizontalScroll | CguiWindowStyle::VerticalScroll);
 * @endcode
 * 
 * @par 注意事项
 * 1. 某些样式（如AlwaysOnTop）可能需要在窗口创建后调用额外API实现
 * 2. 互斥的样式组合（如Borderless和Resizable同时使用）可能产生不可预期的效果
 * 3. Default样式已经包含了常用的窗口特性，适合大多数场景
 */
class CguiWindowStyle {
public:
    // 基础窗口样式（位掩码形式）
    static const int Default = 0;                  // 默认窗口样式（标题栏 + 边框）
    static const int Borderless = 1 << 0;         // 无边框窗口
    static const int Resizable = 1 << 1;          // 可调整大小
    static const int FixedSize = 1 << 2;         // 固定大小，不可调整
    static const int NoTitlebar = 1 << 3;        // 无标题栏
    static const int AlwaysOnTop = 1 << 4;      // 始终置顶
    static const int Minimizable = 1 << 5;        // 可最小化
    static const int Maximizable = 1 << 6;        // 可最大化
    static const int CloseButton = 1 << 7;       // 显示关闭按钮
    static const int Disabled = 1 << 8;           // 禁用窗口
    static const int Visible = 1 << 9;            // 初始可见
    static const int Popup = 1 << 10;             // 弹出窗口样式
    static const int Dialog = 1 << 11;            // 对话框样式
    static const int HorizontalScroll = 1 << 12; // 水平滚动条
    static const int VerticalScroll = 1 << 13;   // 垂直滚动条
    static const int SystemMenu = 1 << 14;       // 系统菜单
    static const int ThickFrame = 1 << 15;       // 粗边框（可调整大小）

private:
    CguiWindowStyle() = delete; // 防止实例化
};

}

#endif // CGUI_ENUM_HPP
