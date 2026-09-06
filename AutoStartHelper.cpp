#include "AutoStartHelper.h"
#include <QSettings>
#include <QFileInfo>

/**
 * @brief 设置Windows开机自启注册表项（HKEY_CURRENT_USER，无需管理员权限）
 * @param enable true开启开机自启；false关闭开机自启
 * @param appName 注册表Run项内程序键名，作为本程序唯一标识
 * @param exePath 应用程序exe完整路径
 * @return true操作注册表成功；false失败（权限不足等）
 * @note 路径包含空格时必须用双引号包裹，Windows Run注册表语法要求
 */
bool AutoStartHelper::setAutoStart(bool enable, const QString &appName, const QString &exePath)
{
    // 当前用户Run注册表项，普通用户权限即可读写，不需要管理员
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
        // 删除对应注册表键，取消开机自启
        reg.remove(appName);
    }
    // 判断注册表读写状态，返回操作结果
    return reg.status() == QSettings::NoError;
}

/**
 * @brief 简单判断注册表是否存在该appName键
 * @param appName 注册表Run项键名
 * @return true注册表存在该键；false不存在
 * @note 仅判断键是否存在，**不校验exe路径是否匹配**，存在键但指向别的程序也返回true
 */
bool AutoStartHelper::isAutoStart(const QString &appName)
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    return reg.contains(appName);
}

/**
 * @brief 校验开机自启：注册表存在键，并且注册表存储的exe路径与当前程序路径完全一致
 * @param appName 注册表Run项键名
 * @param currentExePath 当前运行程序exe路径
 * @return true开启自启且路径匹配；false未开启/注册表指向别的exe
 * @note 自动去除注册表值两端双引号，文件路径大小写不敏感比对（Windows文件系统大小写无关）
 */
bool AutoStartHelper::isAutoStart(const QString &appName, const QString &currentExePath)
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(!reg.contains(appName))
        return false;

    QString regValue = reg.value(appName).toString();
    QString realExe = QFileInfo(currentExePath).absoluteFilePath();

    // 移除注册表值自带的双引号，再做路径解析
    regValue = regValue.remove('"');
    QString regExe = QFileInfo(regValue).absoluteFilePath();

    // Windows路径大小写无关，使用不区分大小写比对
    return regExe.compare(realExe, Qt::CaseInsensitive) == 0;
}
