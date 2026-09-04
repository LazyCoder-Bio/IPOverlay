#ifndef FLOATINGIPOVERLAYWIDGET_H
#define FLOATINGIPOVERLAYWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class FloatingIpoverlayWidget;
}
QT_END_NAMESPACE

class FloatingIpoverlayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FloatingIpoverlayWidget(QWidget *parent = nullptr);
    ~FloatingIpoverlayWidget() override;


    //对外暴露接口：由外部Widget调用，传入要显示的文本
    void setIpText(const QString& pubStr, const QString& lanStr);
    void safeShow();
    void setBackgroundAlpha(int alphaVal);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint m_dragPos;   // 保存拖拽偏移量




private:
    Ui::FloatingIpoverlayWidget *ui;
};
#endif // FLOATINGIPOVERLAYWIDGET_H
