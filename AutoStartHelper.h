#ifndef AUTOSTARTHELPER_H
#define AUTOSTARTHELPER_H

#include <QString>

// 纯工具类，静态函数，只负责Windows注册表开机自启读写
class AutoStartHelper
{
public:
    // enable=true开启开机自启；appName注册表键名；exePath程序完整绝对路径
    static bool setAutoStart(bool enable, const QString& appName, const QString& exePath);
    // 查询该键名是否已经开启开机自启
    static bool isAutoStart(const QString& appName);
    // 重载：校验注册表存储路径和当前exe路径匹配，解决程序移动位置残留旧注册表
    static bool isAutoStart(const QString& appName, const QString& currentExePath);

};

#endif // AUTOSTARTHELPER_H
