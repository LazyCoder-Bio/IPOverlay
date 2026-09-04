#include "floatingipoverlaywidget.h"
#include "Widget.h"



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

    a.setWindowIcon(QIcon(":/IPOverlay.png"));





   // FloatingIpoverlayWidget w;

    Widget w;
    w.hide();

    //w.show();
    return QApplication::exec();
}
