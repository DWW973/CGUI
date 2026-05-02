cmake_minimum_required(VERSION 3.10)

project(cgui VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
    src/logger.cpp
    src/util.cpp
    src/windows/system.cpp
    src/computer_info.cpp
    src/logger.cpp
    src/windows/window.cpp
)

# 构建静态库
add_library(cgui_static STATIC ${SOURCES})
set_target_properties(cgui_static PROPERTIES OUTPUT_NAME "cgui")

# 构建动态库
add_library(cgui_shared SHARED ${SOURCES})
set_target_properties(cgui_shared PROPERTIES 
    OUTPUT_NAME "cgui"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# 安装库文件
install(TARGETS cgui_static cgui_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/
    DESTINATION include/cgui
)
