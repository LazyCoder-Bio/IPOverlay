#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>
#include "ConfigHelper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

/**
 * @brief 参数配置对话框
 * @note 用于编辑公网IP查询URL列表，支持新增、删除、双击编辑URL；
 * setConfig加载外部配置到界面，getConfig从界面提取修改后的配置。
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief 设置对话框构造函数
     * @param parent 父窗口指针
     */
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

    /**
     * @brief 将外部传入的配置加载到UI控件
     * @param cfg 输入配置结构体
     */
    void setConfig(const AppConfig& cfg);

    /**
     * @brief 读取界面控件内容，组装并返回配置对象
     * @return 组装后的AppConfig配置实例
     */
    AppConfig getConfig() const;

private slots:
    /**
     * @brief 添加按钮槽函数：新增一条可编辑URL条目
     */
    void onBtnAddClicked();

    /**
     * @brief 删除按钮槽函数：删除列表选中的URL条目
     */
    void onBtnDelClicked();

    /**
     * @brief 保存按钮槽函数，执行accept，关闭对话框并标记确认保存
     */
    void onBtnSaveClicked();

    /**
     * @brief 取消按钮槽函数，执行reject，关闭对话框不保存修改
     */
    void onBtnCancelClicked();

private:
    Ui::SettingsDialog *ui; ///< UI界面对象指针
};

#endif // SETTINGSDIALOG_H
