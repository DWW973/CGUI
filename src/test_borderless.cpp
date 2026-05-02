#include "../include/window.h"
#include "../include/logger.hpp"

int main() {
    LOG_INFO("Testing BORDERLESS style...");
    
    try {
        cgui::Window window;
        
        // 测试BORDERLESS样式
        LOG_INFO("Creating borderless window...");
        cgui::CguiStautsCode status = window.start_window("Borderless Window", 800, 600, cgui::CguiWindowStyle::BORDERLESS);
        
        if (status == cgui::CguiStautsCode::SUCCESS) {
            LOG_INFO("Borderless window opened successfully");
            LOG_INFO_FMT("Window title: %s", window.get_title().c_str());
            LOG_INFO_FMT("Window size: %dx%d", window.get_width(), window.get_height());
            LOG_INFO_FMT("Window ID: %s", window.get_id().c_str());
            
            // 运行一段时间（5秒超时）
            LOG_INFO("Running for 5 seconds...");
            int timeout = 5000; // 5秒
            int start_time = GetTickCount();
            
            while (!window.should_close() && (GetTickCount() - start_time) < timeout) {
                window.update();
                Sleep(16); // ~60 FPS
            }
            
            LOG_INFO("Closing window...");
            window.close();
            LOG_INFO("Window closed");
        } else {
            LOG_ERROR_FMT("Failed to open borderless window, status code: %d", (int)status);
        }
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Exception caught: %s", e.what());
    } catch (...) {
        LOG_ERROR("Unknown exception caught");
    }
    
    LOG_INFO("Test completed!");
    return 0;
}