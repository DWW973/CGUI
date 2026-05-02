#include "../include/logger.hpp"
#include <iostream>

int main() {
    std::cout << "Testing logger new features..." << std::endl;

    // 初始化日志器
    cgui::init_logger("feature_test", cgui::LogLevel::DEBUG, "feature_test.log");

    // 测试传统字符串日志
    std::cout << "\n1. Testing traditional string logs:" << std::endl;
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");
    LOG_FATAL("This is a fatal message");

    // 测试流式输出
    std::cout << "\n2. Testing stream output:" << std::endl;
    int value = 42;
    double pi = 3.14159;
    bool flag = true;
    
    LOG_DEBUG_STREAM << "Stream debug: value = " << value << ", pi = " << pi << ", flag = " << flag;
    LOG_INFO_STREAM << "Stream info: value = " << value << ", pi = " << pi << ", flag = " << flag;
    LOG_WARNING_STREAM << "Stream warning: value = " << value << ", pi = " << pi << ", flag = " << flag;
    LOG_ERROR_STREAM << "Stream error: value = " << value << ", pi = " << pi << ", flag = " << flag;
    LOG_FATAL_STREAM << "Stream fatal: value = " << value << ", pi = " << pi << ", flag = " << flag;

    // 测试格式化输出
    std::cout << "\n3. Testing formatted output:" << std::endl;
    LOG_DEBUG_FMT("Formatted debug: value = %d, pi = %.2f, flag = %s", value, pi, flag ? "true" : "false");
    LOG_INFO_FMT("Formatted info: value = %d, pi = %.2f, flag = %s", value, pi, flag ? "true" : "false");
    LOG_WARNING_FMT("Formatted warning: value = %d, pi = %.2f, flag = %s", value, pi, flag ? "true" : "false");
    LOG_ERROR_FMT("Formatted error: value = %d, pi = %.2f, flag = %s", value, pi, flag ? "true" : "false");
    LOG_FATAL_FMT("Formatted fatal: value = %d, pi = %.2f, flag = %s", value, pi, flag ? "true" : "false");

    std::cout << "\nLogger features test completed!" << std::endl;
    return 0;
}