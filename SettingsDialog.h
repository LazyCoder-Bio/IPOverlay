#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ConfigHelper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

    void setConfig(const AppConfig& cfg);
    AppConfig getConfig() const;

private slots:
    void onBtnAddClicked();
    void onBtnDelClicked();
    void onBtnSaveClicked();
    void onBtnCancelClicked();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
