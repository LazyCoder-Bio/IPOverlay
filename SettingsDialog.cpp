#include "SettingsDialog.h"
#include "./ui_settingsdialog.h"
#include <QListWidgetItem>

/**
 * @brief 设置对话框构造函数
 * @param parent 父窗口指针
 * @note 用于管理公网IP获取接口URL列表，支持增删改URL
 */
SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle("IPOverlay 设置");

    // 绑定各个按钮点击信号到对应的槽函数
    connect(ui->btnAdd, &QPushButton::clicked, this, &SettingsDialog::onBtnAddClicked);
    connect(ui->btnDel, &QPushButton::clicked, this, &SettingsDialog::onBtnDelClicked);
    connect(ui->btnSave, &QPushButton::clicked, this, &SettingsDialog::onBtnSaveClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &SettingsDialog::onBtnCancelClicked);
}

/**
 * @brief 析构函数，释放UI资源
 */
SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/**
 * @brief 将传入的配置数据加载到对话框UI控件
 * @param cfg 外部传入的AppConfig配置对象
 * @note 把wanUrlList公网接口列表填充到QListWidget，每个条目支持双击编辑
 */
void SettingsDialog::setConfig(const AppConfig &cfg)
{
    ui->listUrl->clear();
    for(const QString& url : cfg.wanUrlList)
    {
        QListWidgetItem* item = new QListWidgetItem(url, ui->listUrl);
        item->setFlags(item->flags() | Qt::ItemIsEditable); //开启条目双击编辑能力
    }
}

/**
 * @brief 从UI界面提取用户修改后的配置，组装返回AppConfig
 * @return 组装完成的配置实例
 * @note 刷新间隔暂时硬编码固定，界面不提供编辑；过滤空字符串URL
 */
AppConfig SettingsDialog::getConfig() const
{
    AppConfig cfg = ConfigHelper::loadConfig(); //先读取原有完整配置
    cfg.wanUrlList.clear();

    //遍历列表控件，取出每一行非空URL
    for(int i = 0; i < ui->listUrl->count(); ++i)
    {
        QString text = ui->listUrl->item(i)->text().trimmed();
        if(!text.isEmpty())
        {
            cfg.wanUrlList.append(text);
        }
    }
    return cfg;
}

/**
 * @brief 添加按钮槽函数：新增一条可编辑的URL条目
 */
void SettingsDialog::onBtnAddClicked()
{
    QListWidgetItem* item = new QListWidgetItem("https://");
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listUrl->addItem(item);
    ui->listUrl->setCurrentItem(item);
    ui->listUrl->editItem(item); //自动进入编辑状态，方便用户直接输入地址
}

/**
 * @brief 删除按钮槽函数，删除当前选中列表项
 * @note 如果没有选中任何条目，则直接返回不做操作
 */
void SettingsDialog::onBtnDelClicked()
{
    QListWidgetItem* curItem = ui->listUrl->currentItem();
    if (!curItem)
    {
        //没有选中，直接返回，什么都不干
        return;
    }
    delete curItem; //QListWidget会自动移除被delete的item
}

/**
 * @brief 保存按钮：关闭对话框，返回accept接受状态
 */
void SettingsDialog::onBtnSaveClicked()
{
    accept();
}

/**
 * @brief 取消按钮：关闭对话框，返回reject拒绝状态，不保存修改
 */
void SettingsDialog::onBtnCancelClicked()
{
    reject();
}
