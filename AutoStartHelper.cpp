#include "AutoStartHelper.h"
#include <QSettings>
#include <QFileInfo>

bool AutoStartHelper::setAutoStart(bool enable, const QString &appName, const QString &exePath)
{
    // HKEY_CURRENT_USER 当前用户，不需要管理员权限
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (enable)
    {
        QString absPath = QFileInfo(exePath).absoluteFilePath();
        // ✅修复：路径带空格必须包裹双引号，Windows Run注册表要求
        if(absPath.contains(" ")){
            absPath = QString("\"%1\"").arg(absPath);
        }
        reg.setValue(appName, absPath);
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

bool AutoStartHelper::isAutoStart(const QString &appName, const QString &currentExePath)
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(!reg.contains(appName))
        return false;

    QString regValue = reg.value(appName).toString();
    QString realExe = QFileInfo(currentExePath).absoluteFilePath();
    // 移除注册表值自带的双引号再对比路径
    regValue = regValue.remove('"');
    QString regExe = QFileInfo(regValue).absoluteFilePath();

    return regExe.compare(realExe, Qt::CaseInsensitive) == 0;
}