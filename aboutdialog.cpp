#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QtGlobal>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    // 移除问号帮助按钮，保留右上角关闭
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QString html = R"(
<h3>IP悬浮显示工具</h3>
<p>版本：V1.0.0</p>
<p>编译环境：%QT_VER%</p>
<br>
<p>作者：LazyCoder-Bio</p>
<p>GitHub：<a href="https://github.com/LazyCoder-Bio/xxx">https://github.com/LazyCoder-Bio/xxx</a></p>
<br>
<p>公网IP获取来源：</p>
<p>ipinfo.io、api‑ipv4.ip.sb、api.ipify.org</p>
<p><i>提示：第三方免费接口，网络不同结果会有差异</i></p>
<br>
<p>Copyright © 2026</p>
    )";

    html = html.replace("%QT_VER%", QString("Qt %1").arg(QT_VERSION_STR));
    ui->lblContent->setText(html);

    // 开启标签超链接点击打开浏览器
    ui->lblContent->setOpenExternalLinks(true);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
