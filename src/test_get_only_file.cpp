#include "../include/window.h"
#include "../include/logger.hpp"

using namespace cgui;

int main() {
    LOG_INFO("Testing GetOnlyFile function...");
    
    // Test with just base name
    String file1 = GetOnlyFile("test");
    LOG_INFO_FMT("File 1: %s", file1.c_str());
    
    // Test with base name and extension
    String file2 = GetOnlyFile("test", "txt");
    LOG_INFO_FMT("File 2: %s", file2.c_str());
    
    // Test with different base name
    String file3 = GetOnlyFile("log");
    LOG_INFO_FMT("File 3: %s", file3.c_str());
    
    // Test with different extension
    String file4 = GetOnlyFile("image", "png");
    LOG_INFO_FMT("File 4: %s", file4.c_str());
    
    LOG_INFO("Test completed!");
    return 0;
}