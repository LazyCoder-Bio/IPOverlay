#include "LogHelper.h"
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>

void LogHelper::init()
{
    rotateLogIfNeeded();
}

void LogHelper::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 1. 组装日志时间 + 等级标签
    QString timeStr = QDateTime::currentDateTime().toString("yyyy‑MM‑dd HH:mm:ss");
    QString level = levelToString(type);

    // 日志文本：[时间] [等级] 内容 | 文件:行号（release下context信息会被Qt移除）
    QString logLine = QString("[%1] [%2] %3").arg(timeStr, level, msg);
    if (context.file != nullptr)
    {
        logLine += QString(" | %1:%2").arg(context.file).arg(context.line);
    }

    // 控制台输出
    fprintf(stdout, "%s\n", logLine.toUtf8().constData());

    // 写入日志文件
    QFile outFile(logFilePath());
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream ts(&outFile);
        ts << logLine << Qt::endl;
        outFile.flush();
        outFile.close();
    }
}

void LogHelper::rotateLogIfNeeded()
{
    QFileInfo logInfo(logFilePath());
    if (!logInfo.exists())
        return;

    if (logInfo.size() < MAX_LOG_FILE_SIZE)
        return;

    // 文件超限：删除旧备份，当前日志重命名为old
    QFile oldFile(oldLogFilePath());
    if (oldFile.exists())
    {
        oldFile.remove();
    }
    QFile curLog(logFilePath());
    curLog.rename(oldLogFilePath());
}

QString LogHelper::logFilePath()
{
    // 日志放在exe同级目录
    QDir appDir(QCoreApplication::applicationDirPath());
    return appDir.filePath("ipoverlay.log");
}

QString LogHelper::oldLogFilePath()
{
    QDir appDir(QCoreApplication::applicationDirPath());
    return appDir.filePath("ipoverlay.old.log");
}

QString LogHelper::levelToString(QtMsgType type)
{
    switch (type)
    {
    case QtDebugMsg:    return "DEBUG";
    case QtInfoMsg:     return "INFO";
    case QtWarningMsg:  return "WARN";
    case QtCriticalMsg: return "ERROR";
    case QtFatalMsg:    return "FATAL";
    default: return "UNKNOWN";
    }
}
