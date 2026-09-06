#ifndef LOGHELPER_H
#define LOGHELPER_H

#include <QString>
// ✅补上这个头文件，QtMsgType就在这里
#include <QtCore/qlogging.h>

/**
 * @brief 简易日志辅助工具类
 * @details
 * - 同时输出控制台与本地日志文件 ipoverlay.log
 * - 文件大小超过2MB，旧日志备份为 ipoverlay.old.log
 * - 配合 qInstallMessageHandler 接管Qt全局qDebug/qInfo/qWarning/qCritical
 */
class LogHelper
{
public:
    /**
     * @brief 初始化日志系统，执行日志滚动检查
     */
    static void init();

    /**
     * @brief Qt消息回调处理函数，对接qInstallMessageHandler
     * @param type 消息等级 Debug/Info/Warning/Critical
     * @param context 源码上下文（文件、行号、函数）
     * @param msg 日志文本内容
     */
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    /// 单日志文件最大字节：2MB
    inline static constexpr qint64 MAX_LOG_FILE_SIZE = 2 * 1024 * 1024;

    /**
     * @brief 检查日志文件大小，超过阈值执行滚动备份
     */
    static void rotateLogIfNeeded();

    /**
     * @brief 获取当前日志文件路径 ipoverlay.log
     * @return 日志绝对路径
     */
    static QString logFilePath();

    /**
     * @brief 获取备份旧日志路径 ipoverlay.old.log
     * @return 备份日志绝对路径
     */
    static QString oldLogFilePath();

    /**
     * @brief 将QtMsgType转为可读字符串标签 [DEBUG] [INFO] [WARN] [ERROR]
     * @param type Qt消息类型
     * @return 等级标签字符串
     */
    static QString levelToString(QtMsgType type);
};

#endif // LOGHELPER_H
