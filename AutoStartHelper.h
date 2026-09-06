#ifndef AUTOSTARTHELPER_H
#define AUTOSTARTHELPER_H
#include <QString>

/**
 * @brief Windows开机自启辅助工具类
 * @note 全部为静态函数，操作HKEY_CURRENT_USER注册表Run项，普通用户权限，无需管理员；
 * 仅Windows平台生效，其他系统调用无实际效果。
 */
class AutoStartHelper
{
public:
    /**
     * @brief 设置Windows开机自启注册表项
     * @param enable true开启自启；false关闭自启
     * @param appName 注册表Run项下的键名称，作为程序唯一标识
     * @param exePath 应用程序exe完整路径
     * @return true注册表操作成功；false失败，如权限不足、组策略拦截
     * @note 路径带空格内部会自动包裹双引号，适配Windows Run注册表语法
     */
    static bool setAutoStart(bool enable, const QString& appName, const QString& exePath);

    /**
     * @brief 简单检测注册表中是否存在该自启键
     * @param appName 注册表Run项键名
     * @return true键存在；false键不存在
     * @warning 仅判断键是否存在，**不校验exe路径是否有效**，键指向其他程序也返回true
     */
    static bool isAutoStart(const QString& appName);

    /**
     * @brief 校验自启状态（严格版本）
     * @param appName 注册表Run项键名
     * @param currentExePath 当前运行程序exe路径
     * @return true注册表存在键并且存储的exe路径与当前程序路径一致；false未开启或者指向别的程序
     * @note 自动去除注册表值自带双引号，Windows路径大小写不敏感比对，解决程序移动后注册表残留旧项问题
     */
    static bool isAutoStart(const QString& appName, const QString& currentExePath);
};

#endif // AUTOSTARTHELPER_H
