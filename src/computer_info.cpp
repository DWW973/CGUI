#include "../include/computer_info.h"
#include <cstdio>

// 条件包含Windows API头文件
#ifdef _WIN32
#include <windows.h>
#include <objbase.h>
#endif

namespace cgui {

// ComputerInfo类的默认构造函数
ComputerInfo::ComputerInfo() {
}

// ComputerInfo类的析构函数
ComputerInfo::~ComputerInfo() {
}

// 获取当前平台
String ComputerInfo::get_platform() {
#ifdef _WIN32
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__ANDROID__)
    return "Android";
#elif defined(__IOS__)
    return "iOS";
#else
    return "Unknown";
#endif
}

// 获取电脑位数
short ComputerInfo::get_architecture() {
#ifdef _WIN64
    return 64; // 64位
#elif defined(__x86_64__)
    return 64; // 64位
#elif defined(_WIN32)
    return 32; // 32位
#elif defined(__i386__)
    return 32; // 32位
#else
    return -1; // 未知
#endif
}

// 获取系统版本号
String ComputerInfo::get_os_version() {
#ifdef _WIN32
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    
    if (GetVersionEx(&osvi)) {
        char buffer[128];
        sprintf(buffer, "Windows %d.%d.%d", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
        return String(buffer);
    } else {
        return "Windows (Unknown Version)";
    }
#elif defined(__linux__)
    // Linux系统版本获取
    FILE* fp = popen("uname -r", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp)) {
            String version = "Linux ";
            version += buffer;
            // 移除换行符
            size_t pos = version.find('\n');
            if (pos != String::npos) {
                version = version.substr(0, pos);
            }
            pclose(fp);
            return version;
        }
        pclose(fp);
    }
    return "Linux (Unknown Version)";
#elif defined(__APPLE__)
    // macOS系统版本获取
    FILE* fp = popen("sw_vers -productVersion", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp)) {
            String version = "macOS ";
            version += buffer;
            // 移除换行符
            size_t pos = version.find('\n');
            if (pos != String::npos) {
                version = version.substr(0, pos);
            }
            pclose(fp);
            return version;
        }
        pclose(fp);
    }
    return "macOS (Unknown Version)";
#else
    return "Unknown OS Version";
#endif
}

// 获取唯一计算机ID
String ComputerInfo::get_only_computer_id() {
#ifdef _WIN32
    // Windows平台获取设备规格中的设备ID
    // 方法1: 尝试从注册表获取设备ID
    HKEY hKey;
    DWORD dwType = REG_SZ;
    char buffer[256] = {0};
    DWORD dwSize = sizeof(buffer);
    
    // 尝试读取设备ID（可能在不同位置）
    // 尝试1: 读取系统信息中的ComputerHardwareId
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\SystemInformation", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, "ComputerHardwareId", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return String(buffer);
        }
        RegCloseKey(hKey);
    }
    
    // 尝试2: 读取产品ID
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, "ProductId", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return String(buffer);
        }
        RegCloseKey(hKey);
    }
    
    // 尝试3: 读取系统BIOS信息
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, "SystemProductName", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return String(buffer);
        }
        RegCloseKey(hKey);
    }
    
    // 尝试4: 读取系统信息中的UUID
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, "MachineGuid", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return String(buffer);
        }
        RegCloseKey(hKey);
    }
    
    // 方法2: 获取系统盘卷序列号作为备选
    DWORD volumeSerialNumber = 0;
    if (GetVolumeInformation("C:\\", NULL, 0, &volumeSerialNumber, NULL, NULL, NULL, 0)) {
        char volumeBuffer[64];
        sprintf(volumeBuffer, "%08lx", volumeSerialNumber);
        return String(volumeBuffer);
    } else {
        // 方法3: 生成GUID作为备选
        GUID guid;
        if (CoCreateGuid(&guid) == S_OK) {
            char guidBuffer[64];
            sprintf(guidBuffer, "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                    guid.Data1, guid.Data2, guid.Data3,
                    guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                    guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
            return String(guidBuffer);
        } else {
            return "Unknown Computer ID";
        }
    }
#elif defined(__linux__)
    // Linux平台获取唯一计算机ID
    FILE* fp = popen("cat /etc/machine-id", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp)) {
            String computer_id = buffer;
            // 移除换行符
            size_t pos = computer_id.find('\n');
            if (pos != String::npos) {
                computer_id = computer_id.substr(0, pos);
            }
            pclose(fp);
            return computer_id;
        }
        pclose(fp);
    }
    // 尝试使用dmidecode命令
    fp = popen("dmidecode -s system-uuid", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp)) {
            String computer_id = buffer;
            // 移除换行符
            size_t pos = computer_id.find('\n');
            if (pos != String::npos) {
                computer_id = computer_id.substr(0, pos);
            }
            pclose(fp);
            return computer_id;
        }
        pclose(fp);
    }
    return "Unknown Computer ID";
#elif defined(__APPLE__)
    // macOS平台获取唯一计算机ID
    FILE* fp = popen("ioreg -rd1 -c IOPlatformExpertDevice | grep -E '(IOPlatformUUID)' | cut -c27-62", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp)) {
            String computer_id = buffer;
            // 移除换行符
            size_t pos = computer_id.find('\n');
            if (pos != String::npos) {
                computer_id = computer_id.substr(0, pos);
            }
            pclose(fp);
            return computer_id;
        }
        pclose(fp);
    }
    return "Unknown Computer ID";
#else
    return "Unknown Computer ID";
#endif
}

// 获取显示器当前分辨率
Screen ComputerInfo::get_screen_info() {
    Screen screen = {0, 0};
#ifdef _WIN32
    // Windows平台获取显示器当前分辨率
    // 使用EnumDisplaySettings获取真实分辨率，避免DPI缩放影响
    DEVMODE devMode;
    devMode.dmSize = sizeof(DEVMODE);
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
        screen.width = devMode.dmPelsWidth;
        screen.height = devMode.dmPelsHeight;
    }
#elif defined(__linux__)
    // Linux平台获取显示器当前分辨率
    FILE* fp = popen("xrandr | grep '*' | head -n 1", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp)) {
            sscanf(buffer, "%d x %d", &screen.width, &screen.height);
        }
        pclose(fp);
    }
#elif defined(__APPLE__)
    // macOS平台获取显示器当前分辨率
    FILE* fp = popen("system_profiler SPDisplaysDataType | grep Resolution", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp)) {
            sscanf(buffer, "%*s %*s %d x %d", &screen.width, &screen.height);
        }
        pclose(fp);
    }
#endif
    return screen;
}

} // namespace cgui
