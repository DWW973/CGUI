#ifndef CGUI_COMPUTER_INFO_H
#define CGUI_COMPUTER_INFO_H

#include "string.hpp"

namespace cgui {

// 屏幕信息结构体
struct Screen {
    int width;   // 显示器当前宽度
    int height;  // 显示器当前高度
};

class ComputerInfo {
private:
    // 平台类型枚举
    enum class Platform {
        UNKNOWN,    // 未知平台
        WINDOWS,    // Windows
        LINUX,      // Linux
        MACOS,      // macOS
        ANDROID,    // Android
        IOS         // iOS
    };

public:
    // 构造函数和析构函数
    ComputerInfo();
    ~ComputerInfo();

    // 获取当前平台
    static String get_platform();
    
    // 获取电脑位数
    static short get_architecture();
    
    // 获取系统版本号
    static String get_os_version();
    
    // 获取唯一计算机ID
    static String get_only_computer_id();
    
    // 获取显示器当前分辨率
    static Screen get_screen_info();
};

} // namespace cgui

#endif // CGUI_COMPUTER_INFO_H
