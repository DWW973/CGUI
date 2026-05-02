#include "../include/logger.hpp"
#include <iostream>

int main() {
    std::cout << "Testing logger library..." << std::endl;

    // 测试不同级别的日志
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");
    LOG_FATAL("This is a fatal message");

    // 测试设置日志文件
    cgui::Logger& logger = cgui::get_logger();
    bool file_set = logger.set_file("test.log");
    if (file_set) {
        std::cout << "Log file set successfully" << std::endl;
        LOG_INFO("This message should be written to file");
    } else {
        std::cout << "Failed to set log file" << std::endl;
    }

    // 测试设置日志级别
    logger.set_level(cgui::LogLevel::WARNING);
    LOG_DEBUG("This debug message should not be shown");
    LOG_INFO("This info message should not be shown");
    LOG_WARNING("This warning message should be shown");

    std::cout << "Logger test completed!" << std::endl;
    return 0;
}