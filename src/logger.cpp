#include "../include/logger.hpp"
#include "../include/window.h"

// 处理Windows API的ERROR宏与LogLevel::ERROR的命名冲突
#ifdef _WIN32
#undef ERROR
#endif

namespace cgui {

// LogStream类的实现
Logger::LogStream::LogStream(Logger& logger, LogLevel level)
    : logger_(logger), level_(level), message_(), enabled_(logger.level_ <= level) {}

Logger::LogStream::~LogStream() {
    if (enabled_) {
        logger_.log(level_, message_);
    }
}

// 支持C风格字符串
Logger::LogStream& Logger::LogStream::operator<<(const char* value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

// 支持字符
Logger::LogStream& Logger::LogStream::operator<<(char value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

// 支持整数类型
Logger::LogStream& Logger::LogStream::operator<<(int value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

Logger::LogStream& Logger::LogStream::operator<<(long value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

Logger::LogStream& Logger::LogStream::operator<<(long long value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

Logger::LogStream& Logger::LogStream::operator<<(unsigned int value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

Logger::LogStream& Logger::LogStream::operator<<(unsigned long value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

Logger::LogStream& Logger::LogStream::operator<<(unsigned long long value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

// 支持浮点类型
Logger::LogStream& Logger::LogStream::operator<<(float value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

Logger::LogStream& Logger::LogStream::operator<<(double value) {
    if (enabled_) {
        message_ += value;
    }
    return *this;
}

// 支持布尔类型
Logger::LogStream& Logger::LogStream::operator<<(bool value) {
    if (enabled_) {
        message_ += value ? "true" : "false";
    }
    return *this;
}

// Logger类的实现
Logger::Logger(const String& name, LogLevel level)
    : name_(name), level_(level), log_file_() {
    // 生成唯一的日志文件名
    String log_file_name = GetOnlyFile(name, "log");
    set_file(log_file_name);
    // 避免递归调用日志宏，直接输出到控制台
    std::cout << "Logger initialized with log file: " << log_file_name.c_str() << std::endl;
}

Logger::~Logger() {
    close_file();
}

void Logger::set_level(LogLevel level) {
    level_ = level;
}

bool Logger::set_file(const String& file_path) {
    close_file();
    log_file_.open(file_path.c_str(), std::ios::app);
    return log_file_.is_open();
}

void Logger::close_file() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::debug(const String& message) const {
    if (level_ > LogLevel::DEBUG) {
        return;
    }
    log(LogLevel::DEBUG, message);
}

void Logger::info(const String& message) const {
    if (level_ > LogLevel::INFO) {
        return;
    }
    log(LogLevel::INFO, message);
}

void Logger::warning(const String& message) const {
    if (level_ > LogLevel::WARNING) {
        return;
    }
    log(LogLevel::WARNING, message);
}

void Logger::error(const String& message) const {
    if (level_ > LogLevel::ERROR) {
        return;
    }
    log(LogLevel::ERROR, message);
}

Logger::LogStream Logger::debug() {
    return LogStream(*this, LogLevel::DEBUG);
}

Logger::LogStream Logger::info() {
    return LogStream(*this, LogLevel::INFO);
}

Logger::LogStream Logger::warning() {
    return LogStream(*this, LogLevel::WARNING);
}

Logger::LogStream Logger::error() {
    return LogStream(*this, LogLevel::ERROR);
}

Logger::LogStream Logger::fatal() {
    return LogStream(*this, LogLevel::FATAL);
}

void Logger::debug(const char* format, ...) const {
    if (level_ > LogLevel::DEBUG) {
        return;
    }
    va_list args;
    va_start(args, format);
    int len = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if (len < 0) {
        return;
    }
    std::vector<char> buffer(len + 1);
    va_start(args, format);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    log(LogLevel::DEBUG, String(buffer.data()));
}

void Logger::info(const char* format, ...) const {
    if (level_ > LogLevel::INFO) {
        return;
    }
    va_list args;
    va_start(args, format);
    int len = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if (len < 0) {
        return;
    }
    std::vector<char> buffer(len + 1);
    va_start(args, format);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    log(LogLevel::INFO, String(buffer.data()));
}

void Logger::warning(const char* format, ...) const {
    if (level_ > LogLevel::WARNING) {
        return;
    }
    va_list args;
    va_start(args, format);
    int len = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if (len < 0) {
        return;
    }
    std::vector<char> buffer(len + 1);
    va_start(args, format);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    log(LogLevel::WARNING, String(buffer.data()));
}

void Logger::error(const char* format, ...) const {
    if (level_ > LogLevel::ERROR) {
        return;
    }
    va_list args;
    va_start(args, format);
    int len = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if (len < 0) {
        return;
    }
    std::vector<char> buffer(len + 1);
    va_start(args, format);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    log(LogLevel::ERROR, String(buffer.data()));
}

void Logger::fatal(const char* format, ...) const {
    if (level_ > LogLevel::FATAL) {
        return;
    }
    va_list args;
    va_start(args, format);
    int len = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if (len < 0) {
        return;
    }
    std::vector<char> buffer(len + 1);
    va_start(args, format);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    log(LogLevel::FATAL, String(buffer.data()));
}

void Logger::fatal(const String& message) const {
    if (level_ > LogLevel::FATAL) {
        return;
    }
    log(LogLevel::FATAL, message);
}

void Logger::log(LogLevel level, const String& message) const {
    std::lock_guard<std::mutex> lock(mutex_);

    String time_str = get_current_time();
    String level_str = get_level_string(level);
    String log_message = time_str + " [" + name_ + "] " + level_str + ": " + message;

    // 输出到控制台（带颜色）
    #if defined(_WIN32) || defined(_WIN64)
        // Windows可能需要特殊处理，暂时不使用颜色
        std::cout << log_message << std::endl;
    #else
        // 非Windows系统使用ANSI颜色
        const char* color = "";
        switch (level) {
            case LogLevel::DEBUG:
                color = COLOR_DEBUG;
                break;
            case LogLevel::INFO:
                color = COLOR_INFO;
                break;
            case LogLevel::WARNING:
                color = COLOR_WARNING;
                break;
            case LogLevel::ERROR:
                color = COLOR_ERROR;
                break;
            case LogLevel::FATAL:
                color = COLOR_FATAL;
                break;
            default:
                color = "";
        }
        std::cout << color << log_message << COLOR_RESET << std::endl;
    #endif

    // 输出到文件（不带颜色）
    if (log_file_.is_open()) {
        try {
            log_file_ << log_message << std::endl;
            log_file_.flush();
        } catch (const std::exception& e) {
            // 忽略文件写入异常，确保程序不会崩溃
            std::cerr << "Log file write error: " << e.what() << std::endl;
        }
    }
}

String Logger::get_current_time() const {
    time_t now = time(nullptr);
    struct tm local_time;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&local_time, &now); // Windows线程安全版本
    #else
        localtime_r(&now, &local_time); // POSIX线程安全版本
    #endif
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_time);
    return String(buffer);
}

String Logger::get_level_string(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

void Logger::set_name(const String& name) {
    name_ = name;
}

Logger& get_logger() {
    static Logger logger;
    return logger;
}

void init_logger(const String& name, LogLevel level, const String& file_path) {
    Logger& logger = get_logger();
    logger.set_name(name);
    logger.set_level(level);
    if (!file_path.empty()) {
        logger.set_file(file_path);
    } else {
        // 生成唯一的日志文件名
        String log_file_name = GetOnlyFile(name, "log");
        logger.set_file(log_file_name);
        // 避免递归调用日志宏，直接输出到控制台
        std::cout << "Logger initialized with log file: " << log_file_name.c_str() << std::endl;
    }
}

} // namespace cgui