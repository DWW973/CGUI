#include "../include/window.h"

namespace cgui {

// 获取唯一ID的函数
template<typename T>
String GetOnlyId(const String& name) {
    static int counter = 0;
    time_t now = time(nullptr);
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "Enity(%s)-%ld-%d", name.c_str(), now, ++counter);
    return String(buffer);
};

// 显式实例化GetOnlyId模板
template String GetOnlyId<Window>(const String& name);

// 获取唯一文件名的函数
String GetOnlyFile(const String& base_name, const String& extension) {
    static int counter = 0;
    time_t now = time(nullptr);
    char buffer[256];
    // 确保Log目录存在
    #ifdef _WIN32
        CreateDirectoryA("Log", NULL);
    #else
        mkdir("Log", 0777);
    #endif
    if (extension.empty()) {
        snprintf(buffer, sizeof(buffer), "Log/%s_%ld_%d", base_name.c_str(), now, ++counter);
    } else {
        snprintf(buffer, sizeof(buffer), "Log/%s_%ld_%d.%s", base_name.c_str(), now, ++counter, extension.c_str());
    }
    return String(buffer);
};

} // namespace cgui