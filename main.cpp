#include "floatingipoverlaywidget.h"
#include "Widget.h"
#include "LogHelper.h"


#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;

    //语言支持
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "IPOverlay_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }


    a.setQuitOnLastWindowClosed(false); // ⭐新增，后台驻留必备

    // ✅初始化日志系统，执行日志滚动
    LogHelper::init();
    // ✅全局接管Qt日志输出，所有qDebug/qWarning/qCritical全部进入日志文件
    qInstallMessageHandler(LogHelper::messageHandler);





   // FloatingIpoverlayWidget w;

    Widget w;
    w.hide();

    //w.show();
    return QApplication::exec();
}
