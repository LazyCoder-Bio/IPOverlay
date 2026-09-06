#include "AutoStartHelper.h"
#include <QSettings>
#include <QFileInfo>
#include <QDebug>

/**
 * @brief 设置Windows开机自启注册表项（HKEY_CURRENT_USER，无需管理员权限）
 * @param enable true开启开机自启；false关闭开机自启
 * @param appName 注册表Run项内程序键名，作为本程序唯一标识
 * @param exePath 应用程序exe完整路径
 * @return true操作注册表成功；false失败（权限不足等）
 * @note 无条件给exe路径包裹英文双引号，Windows Run注册表语法要求
 */
bool AutoStartHelper::setAutoStart(bool enable, const QString &appName, const QString &exePath)
{
    // 当前用户Run注册表项，普通用户权限即可读写，不需要管理员
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (enable)
    {
        QString absPath = QFileInfo(exePath).absoluteFilePath();
        // 无条件包裹英文双引号，规避空格、括号、特殊字符解析问题
        absPath = QString("\"%1\"").arg(absPath);
        reg.setValue(appName, absPath);
        qDebug() << "[AutoStartHelper] setAutoStart enable, appName:" << appName << "path:" << absPath;
    }
    else
    {
        reg.remove(appName);
        qDebug() << "[AutoStartHelper] setAutoStart disable, remove key:" << appName;
    }
    reg.sync(); //强制刷入注册表
    // 判断注册表读写状态，返回操作结果
    if (reg.status() != QSettings::NoError)
    {
        qWarning() << "[AutoStartHelper] 注册表操作失败! status=" << reg.status();
        return false;
    }
    return true;
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
    if (reg.status() != QSettings::NoError)
    {
        qWarning() << "[AutoStartHelper] 查询注册表失败 status=" << reg.status();
        return false;
    }
    bool exist = reg.contains(appName);
    qDebug() << "[AutoStartHelper] isAutoStart(simple) key:" << appName << "exist:" << exist;
    return exist;
}

/**
 * @brief 校验开机自启：注册表存在键，并且注册表存储的exe路径与当前程序路径完全一致
 * @param appName 注册表Run项键名
 * @param currentExePath 当前运行程序exe路径
 * @return true开启自启且路径匹配；false未开启/注册表指向别的exe
 * @note 只去除注册表值首尾一对双引号，文件路径大小写不敏感比对（Windows文件系统大小写无关）
 */
bool AutoStartHelper::isAutoStart(const QString &appName, const QString &currentExePath)
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (reg.status() != QSettings::NoError)
    {
        qWarning() << "[AutoStartHelper] 查询注册表失败 status=" << reg.status();
        return false;
    }

    if(!reg.contains(appName))
    {
        qDebug() << "[AutoStartHelper] isAutoStart(checkPath) key not exist:" << appName;
        return false;
    }

    QString regValue = reg.value(appName).toString();
    QString realExe = QFileInfo(currentExePath).absoluteFilePath();

    // ✅只剥离首尾一对双引号，不要删除全部引号，防御异常注册表内容
    if (regValue.startsWith('"') && regValue.endsWith('"'))
    {
        regValue = regValue.mid(1, regValue.length() - 2);
    }

    QString regExe = QFileInfo(regValue).absoluteFilePath();
    bool ok = regExe.compare(realExe, Qt::CaseInsensitive) == 0;
    qDebug() << "[AutoStartHelper] path check regExe:"<<regExe<<" realExe:"<<realExe<<" match:"<<ok;
    return ok;
}
