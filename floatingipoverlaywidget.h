#ifndef FLOATINGIPOVERLAYWIDGET_H
#define FLOATINGIPOVERLAYWIDGET_H
#include <QWidget>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui {
class FloatingIpoverlayWidget;
}
QT_END_NAMESPACE

/**
 * @brief IP悬浮置顶窗口
 * @note 独立顶层无边框置顶窗口，支持鼠标拖拽移动，透明背景；
 * 对外提供setIpText接口更新公网/内网IP显示文本；
 * safeShow用于安全显示窗口，避免抢焦点。
 */
class FloatingIpoverlayWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父控件指针
     */
    explicit FloatingIpoverlayWidget(QWidget *parent = nullptr);
    ~FloatingIpoverlayWidget() override;

    /**
     * @brief 设置显示的公网IP与内网IP文本
     * @param pubStr 公网IP字符串
     * @param lanStr 内网IP字符串
     * @note 由外部Widget调用更新界面label内容
     */
    void setIpText(const QString& pubStr, const QString& lanStr);

    /**
     * @brief 安全显示悬浮窗口，show不抢夺输入焦点
     */
    void safeShow();

    /**
     * @brief 设置背景透明度（预留接口，可后续实现半透明效果）
     * @param alphaVal 透明度数值 0‑255
     */
    void setBackgroundAlpha(int alphaVal);

protected:
    /**
     * @brief 鼠标按下事件，记录拖拽起始偏移
     * @param event 鼠标事件对象
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief 鼠标移动事件，实现窗口拖拽，附带屏幕边界限制
     * @param event 鼠标事件对象
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPoint m_dragPos;   ///< 窗口拖拽时，鼠标相对窗口左上角的偏移量

private:
    Ui::FloatingIpoverlayWidget *ui; ///< UI界面对象指针
};

#endif // FLOATINGIPOVERLAYWIDGET_H
