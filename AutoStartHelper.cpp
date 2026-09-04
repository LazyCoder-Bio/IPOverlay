#include "AutoStartHelper.h"
#include <QSettings>
#include <QFileInfo>

bool AutoStartHelper::setAutoStart(bool enable, const QString &appName, const QString &exePath)
{
    // HKEY_CURRENT_USER 当前用户，不需要管理员权限
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (enable)
    {
        // absoluteFilePath 获取完整磁盘绝对路径，不能只用filePath()
        reg.setValue(appName, QFileInfo(exePath).absoluteFilePath());
    }
    else
    {
        reg.remove(appName);
    }
    return reg.status() == QSettings::NoError;
}

bool AutoStartHelper::isAutoStart(const QString &appName)
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    return reg.contains(appName);
}
