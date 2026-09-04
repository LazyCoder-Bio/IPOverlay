#include "SettingsDialog.h"
#include "./ui_settingsdialog.h"
#include <QListWidgetItem>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle("IPOverlay 设置");

    //绑定按钮信号
    connect(ui->btnAdd, &QPushButton::clicked, this, &SettingsDialog::onBtnAddClicked);
    connect(ui->btnDel, &QPushButton::clicked, this, &SettingsDialog::onBtnDelClicked);
    connect(ui->btnSave, &QPushButton::clicked, this, &SettingsDialog::onBtnSaveClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &SettingsDialog::onBtnCancelClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setConfig(const AppConfig &cfg)
{
    ui->listUrl->clear();
    for(const QString& url : cfg.wanUrlList)
    {
        QListWidgetItem* item = new QListWidgetItem(url, ui->listUrl);
        item->setFlags(item->flags() | Qt::ItemIsEditable); //双击编辑
    }
}

AppConfig SettingsDialog::getConfig() const
{
    AppConfig cfg;
    //⚠️ 刷新间隔保留，从配置读取，界面不再展示编辑
    cfg.refreshIntervalMs = 10 * 60 * 1000;
    cfg.wanUrlList.clear();

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

//添加URL
void SettingsDialog::onBtnAddClicked()
{
    QListWidgetItem* item = new QListWidgetItem("https://");
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listUrl->addItem(item);
    ui->listUrl->setCurrentItem(item);
    ui->listUrl->editItem(item);
}

//删除：必须有选中条目才执行删除
void SettingsDialog::onBtnDelClicked()
{
    QListWidgetItem* curItem = ui->listUrl->currentItem();
    if (!curItem)
    {
        //没有选中，直接返回，什么都不干
        return;
    }
    delete curItem;
}

void SettingsDialog::onBtnSaveClicked()
{
    accept();
}

void SettingsDialog::onBtnCancelClicked()
{
    reject();
}
