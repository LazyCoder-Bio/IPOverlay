#include "floatingipoverlaywidget.h"
#include "./ui_floatingipoverlaywidget.h"

#include <QMouseEvent>
#include <QApplication>
#include <QScreen>


FloatingIpoverlayWidget::FloatingIpoverlayWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FloatingIpoverlayWidget)
{
    ui->setupUi(this);

    // 1.窗口标记：无边框 + 置顶（总在最上层）
    setWindowFlags(
        Qt::Window                // 这是独立顶层窗口，不能丢
        | Qt::FramelessWindowHint // 去掉系统标题栏边框
        | Qt::WindowStaysOnTopHint // 窗口置顶，永远显示在其他窗口上方
        | Qt::WindowDoesNotAcceptFocus
        );

    // 2.开启窗口整体透明背景，允许绘制圆角/半透明
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating); //新增，show不抢焦点







}













void FloatingIpoverlayWidget:: safeShow(){
    show();
}




// setter接口：只负责把传入字符串设置给label
void FloatingIpoverlayWidget::setIpText(const QString &pubStr, const QString &lanStr)
{
    ui->m_PublicIp->setText(pubStr);
    ui->m_LocalIp->setText(lanStr);
}



//============自实现 拖拽，移动===================//


#include <QMouseEvent>

// 鼠标左键按下：记录偏移
void FloatingIpoverlayWidget::mousePressEvent(QMouseEvent *event)
{
    // 只处理鼠标左键
    if (event->button() == Qt::LeftButton)
    {
        // globalPosition()：屏幕全局鼠标坐标；frameGeometry().topLeft()：窗口左上角屏幕坐标
        m_dragPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept(); // 消费这个事件，不继续传递
    }
}

// 鼠标移动：拖动窗口
void FloatingIpoverlayWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPoint newWinPos = event->globalPosition().toPoint() - m_dragPos;

        // 获取屏幕可用区域（排除任务栏）
        QRect screenRect = QApplication::primaryScreen()->availableGeometry();
        QRect winRect(newWinPos, size());

        // 限制窗口不能跑出屏幕
        if (screenRect.contains(winRect))
        {
            move(newWinPos);
        }
        event->accept();
    }
}

//==========================================//




FloatingIpoverlayWidget::~FloatingIpoverlayWidget()
{
    delete ui;
}
