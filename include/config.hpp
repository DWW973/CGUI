#ifndef CGUI_CONFIG_HPP
#define CGUI_CONFIG_HPP

#include "string.hpp"

namespace cgui {

// 版本信息
#define CGUI_VERSION_MAJOR 1
#define CGUI_VERSION_MINOR 0
#define CGUI_VERSION_PATCH 0

// 版本字符串
inline String get_version() {
    return String("1.0.0");
}

// 平台检测
#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX 2
#define PLATFORM_MACOS 3
#define PLATFORM_ANDROID 4
#define PLATFORM_IOS 5
#define PLATFORM_UNKNOWN 0

#ifdef _WIN32
    #define CGUI_PLATFORM PLATFORM_WINDOWS
#elif defined(__linux__)
    #define CGUI_PLATFORM PLATFORM_LINUX
#elif defined(__APPLE__)
    #define CGUI_PLATFORM PLATFORM_MACOS
#elif defined(__ANDROID__)
    #define CGUI_PLATFORM PLATFORM_ANDROID
#elif defined(__IOS__)
    #define CGUI_PLATFORM PLATFORM_IOS
#else
    #define CGUI_PLATFORM PLATFORM_UNKNOWN
#endif

// 架构检测
#define ARCH_32 1
#define ARCH_64 2
#define ARCH_UNKNOWN 0

#ifdef _WIN64
    #define CGUI_ARCH ARCH_64
#elif defined(__x86_64__)
    #define CGUI_ARCH ARCH_64
#elif defined(_WIN32)
    #define CGUI_ARCH ARCH_32
#elif defined(__i386__)
    #define CGUI_ARCH ARCH_32
#else
    #define CGUI_ARCH ARCH_UNKNOWN
#endif

// 最大值宏定义
#define FULLVALUE 1
#define FULLWIDTH FULLVALUE
#define FULLHEIGHT FULLVALUE

// 默认窗口配置
constexpr int DEFAULT_WINDOW_WIDTH = FULLWIDTH; // 使用最大宽度
constexpr int DEFAULT_WINDOW_HEIGHT = FULLHEIGHT; // 使用最大高度
constexpr const char* DEFAULT_WINDOW_TITLE = "CGUI Window";

// 日志配置
constexpr const char* LOG_DIR = "Log";
constexpr int MAX_LOG_FILE_SIZE = 1024 * 1024; // 1MB

} // namespace cgui

#endif // CGUI_CONFIG_HPP