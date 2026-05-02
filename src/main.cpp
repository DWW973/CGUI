#include "../include/window.h"
#include "../include/logger.hpp"
#include "../include/computer_info.h"

int main() {
    LOG_INFO("Testing cgui library...");

    // Test get_platform
    cgui::String platform = cgui::ComputerInfo::get_platform();
    LOG_INFO_FMT("Current platform: %s", platform.c_str());
    
    // Test get_architecture
    short architecture = cgui::ComputerInfo::get_architecture();
    LOG_INFO_FMT("Current architecture: %d", architecture);
    
    // Test get_os_version
    cgui::String os_version = cgui::ComputerInfo::get_os_version();
    LOG_INFO_FMT("Current OS version: %s", os_version.c_str());
    
    // Test get_only_computer_id
    cgui::String computer_id = cgui::ComputerInfo::get_only_computer_id();
    LOG_INFO_FMT("Current computer ID: %s", computer_id.c_str());
    
    // Test get_screen_info
    cgui::Screen screen = cgui::ComputerInfo::get_screen_info();
    LOG_INFO_FMT("Current screen resolution: %dx%d", screen.width, screen.height);

    // Create a window
    cgui::Window window;

    // Start the window with custom title and size
    cgui::CguiStautsCode status = window.start_window("My Custom Window", 100, 100, cgui::CguiWindowStyle::Default);
    
    // Display open status
    switch (status) {
    case cgui::CguiStautsCode::SUCCESS:
        LOG_INFO("Window opened successfully");
        break;
    case cgui::CguiStautsCode::WINDOW_ALREADY_OPEN:
        LOG_WARNING("Window is already open");
        break;
    case cgui::CguiStautsCode::WINDOW_CLASS_REGISTER_FAILED:
        LOG_ERROR("Failed to register window class");
        break;
    case cgui::CguiStautsCode::WINDOW_CREATE_FAILED:
        LOG_ERROR("Failed to create window");
        break;
    default:
        LOG_ERROR("Unknown error when opening window");
        break;
    }
    
    // Check if window is open
    if (window.is_window_open()) {
        LOG_INFO("Window is open!");

        // Update the window
        window.update();

        // Change window title
        window.set_title("Updated Test Window");

        // Change window size (commented out for fullscreen mode)
        // window.set_size(1024, 768);

        // Get window properties
        LOG_INFO_FMT("Window title: %s", window.get_title().c_str());
        LOG_INFO_FMT("Window size: %dx%d", window.get_width(), window.get_height());
        LOG_INFO_FMT("Window ID: %s", window.get_id().c_str());

        // Main loop
        LOG_INFO("Starting main loop");
        while (!window.should_close()) {
            // Update window
            window.update();
            
            // Small delay to avoid high CPU usage
            Sleep(16); // ~60 FPS
        }
        LOG_INFO("Main loop ended");
        
        // Close the window
        window.close();
    }

    LOG_INFO("Test completed!");
    return 0;
}