#include "../include/logger.hpp"
#include <iostream>

int main() {
    std::cout << "Testing logger initialization..." << std::endl;

    // 测试初始化全局日志器
    cgui::init_logger("test_app", cgui::LogLevel::DEBUG, "init_test.log");

    // 测试不同级别的日志
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");
    LOG_FATAL("This is a fatal message");

    std::cout << "Logger initialization test completed!" << std::endl;
    return 0;
}