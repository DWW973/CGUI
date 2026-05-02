/**
 * @file logger.hpp
 * @brief 日志库的定义和实现
 * @details 实现了一个简单的日志库，支持不同的日志级别和输出方式
 */
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "string.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <mutex>
#include <cstdarg>

// 前向声明GetOnlyFile函数
namespace cgui {
    String GetOnlyFile(const String& base_name, const String& extension);
}

namespace cgui {

/**
 * @enum LogLevel
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG = 0,    ///< 调试级别
    INFO,         ///< 信息级别
    WARNING,      ///< 警告级别
    ERROR,        ///< 错误级别
    FATAL         ///< 致命错误级别
};

/**
 * @class Logger
 * @brief 日志类
 * @details 实现了日志的记录功能，支持不同的日志级别和输出方式
 */
class Logger {
public:
    /**
     * @brief 构造函数
     * @param name 日志名称
     * @param level 日志级别
     */
    Logger(const String& name = "cgui", LogLevel level = LogLevel::INFO);

    /**
     * @brief 析构函数
     */
    ~Logger();

    /**
     * @brief 设置日志名称
     * @param name 日志名称
     */
    void set_name(const String& name);

    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    void set_level(LogLevel level);

    /**
     * @brief 设置日志文件
     * @param file_path 日志文件路径
     * @return 是否成功设置
     */
    bool set_file(const String& file_path);

    /**
     * @brief 关闭日志文件
     */
    void close_file();

    /**
     * @brief 输出调试级别日志
     * @param message 日志消息
     */
    void debug(const String& message) const;

    /**
     * @brief 输出信息级别日志
     * @param message 日志消息
     */
    void info(const String& message) const;

    /**
     * @brief 输出警告级别日志
     * @param message 日志消息
     */
    void warning(const String& message) const;

    /**
     * @brief 输出错误级别日志
     * @param message 日志消息
     */
    void error(const String& message) const;

    /**
     * @brief 输出致命错误级别日志
     * @param message 日志消息
     */
    void fatal(const String& message) const;

    /**
     * @brief 流式输出日志类
     */
    class LogStream {
    public:
        LogStream(Logger& logger, LogLevel level);
        
        ~LogStream();
        
        template<typename T>
        LogStream& operator<<(const T& value) {
            if (enabled_) {
                message_ += value;
            }
            return *this;
        }
        
        // 支持C风格字符串
        LogStream& operator<<(const char* value);
        
        // 支持字符
        LogStream& operator<<(char value);
        
        // 支持整数类型
        LogStream& operator<<(int value);
        
        LogStream& operator<<(long value);
        
        LogStream& operator<<(long long value);
        
        LogStream& operator<<(unsigned int value);
        
        LogStream& operator<<(unsigned long value);
        
        LogStream& operator<<(unsigned long long value);
        
        // 支持浮点类型
        LogStream& operator<<(float value);
        
        LogStream& operator<<(double value);
        
        // 支持布尔类型
        LogStream& operator<<(bool value);
        
    private:
        Logger& logger_;
        LogLevel level_;
        String message_;
        bool enabled_;
    };
    
    /**
     * @brief 获取调试级别流式输出
     * @return 流式输出对象
     */
    LogStream debug();
    
    /**
     * @brief 获取信息级别流式输出
     * @return 流式输出对象
     */
    LogStream info();
    
    /**
     * @brief 获取警告级别流式输出
     * @return 流式输出对象
     */
    LogStream warning();
    
    /**
     * @brief 获取错误级别流式输出
     * @return 流式输出对象
     */
    LogStream error();
    
    /**
     * @brief 获取致命错误级别流式输出
     * @return 流式输出对象
     */
    LogStream fatal();
    
    /**
     * @brief 格式化输出调试级别日志
     * @param format 格式化字符串
     * @param ... 可变参数
     */
    void debug(const char* format, ...) const;

    /**
     * @brief 格式化输出信息级别日志
     * @param format 格式化字符串
     * @param ... 可变参数
     */
    void info(const char* format, ...) const;

    /**
     * @brief 格式化输出警告级别日志
     * @param format 格式化字符串
     * @param ... 可变参数
     */
    void warning(const char* format, ...) const;

    /**
     * @brief 格式化输出错误级别日志
     * @param format 格式化字符串
     * @param ... 可变参数
     */
    void error(const char* format, ...) const;

    /**
     * @brief 格式化输出致命错误级别日志
     * @param format 格式化字符串
     * @param ... 可变参数
     */
    void fatal(const char* format, ...) const;

private:
    /**
     * @brief 输出日志
     * @param level 日志级别
     * @param message 日志消息
     */
    void log(LogLevel level, const String& message) const;

    /**
     * @brief 获取当前时间字符串
     * @return 当前时间字符串
     */
    String get_current_time() const;

    /**
     * @brief 获取日志级别字符串
     * @param level 日志级别
     * @return 日志级别字符串
     */
    String get_level_string(LogLevel level) const;

private:
    // 颜色定义
    static constexpr const char* COLOR_DEBUG = "\x1b[34m";
    static constexpr const char* COLOR_INFO = "\x1b[32m";
    static constexpr const char* COLOR_WARNING = "\x1b[33m";
    static constexpr const char* COLOR_ERROR = "\x1b[31m";
    static constexpr const char* COLOR_FATAL = "\x1b[35m";
    static constexpr const char* COLOR_RESET = "\x1b[0m";
    
    String name_;                ///< 日志名称
    LogLevel level_;             ///< 日志级别
    mutable std::ofstream log_file_; ///< 日志文件流（mutable 允许在 const 成员函数中修改）
    mutable std::mutex mutex_;   ///< 互斥锁，保证线程安全（mutable 允许在 const 成员函数中修改）
};

/**
 * @brief 初始化全局日志器
 * @param name 日志名称
 * @param level 日志级别
 * @param file_path 日志文件路径（可选）
 */
void init_logger(const String& name, LogLevel level = LogLevel::INFO, const String& file_path = "");

/**
 * @brief 获取全局日志实例
 * @return 全局日志实例
 */
Logger& get_logger();

/**
 * @brief 调试级别日志宏
 */
#define LOG_DEBUG(message) cgui::get_logger().debug((message))

/**
 * @brief 信息级别日志宏
 */
#define LOG_INFO(message) cgui::get_logger().info((message))

/**
 * @brief 警告级别日志宏
 */
#define LOG_WARNING(message) cgui::get_logger().warning((message))

/**
 * @brief 错误级别日志宏
 */
#define LOG_ERROR(message) cgui::get_logger().error((message))

/**
 * @brief 致命错误级别日志宏
 */
#define LOG_FATAL(message) cgui::get_logger().fatal((message))

/**
 * @brief 流式输出调试级别日志宏
 */
#define LOG_DEBUG_STREAM cgui::get_logger().debug()

/**
 * @brief 流式输出信息级别日志宏
 */
#define LOG_INFO_STREAM cgui::get_logger().info()

/**
 * @brief 流式输出警告级别日志宏
 */
#define LOG_WARNING_STREAM cgui::get_logger().warning()

/**
 * @brief 流式输出错误级别日志宏
 */
#define LOG_ERROR_STREAM cgui::get_logger().error()

/**
 * @brief 流式输出致命错误级别日志宏
 */
#define LOG_FATAL_STREAM cgui::get_logger().fatal()

/**
 * @brief 格式化输出调试级别日志宏
 */
#define LOG_DEBUG_FMT(format, ...) cgui::get_logger().debug((format), ##__VA_ARGS__)

/**
 * @brief 格式化输出信息级别日志宏
 */
#define LOG_INFO_FMT(format, ...) cgui::get_logger().info((format), ##__VA_ARGS__)

/**
 * @brief 格式化输出警告级别日志宏
 */
#define LOG_WARNING_FMT(format, ...) cgui::get_logger().warning((format), ##__VA_ARGS__)

/**
 * @brief 格式化输出错误级别日志宏
 */
#define LOG_ERROR_FMT(format, ...) cgui::get_logger().error((format), ##__VA_ARGS__)

/**
 * @brief 格式化输出致命错误级别日志宏
 */
#define LOG_FATAL_FMT(format, ...) cgui::get_logger().fatal((format), ##__VA_ARGS__)

} // namespace cgui

#endif // LOGGER_HPP