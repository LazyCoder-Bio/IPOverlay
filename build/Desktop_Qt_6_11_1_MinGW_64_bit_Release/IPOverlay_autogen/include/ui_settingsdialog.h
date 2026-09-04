/********************************************************************************
** Form generated from reading UI file 'SettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listUrl;
    QHBoxLayout *horizontalLayout_btns;
    QPushButton *btnAdd;
    QPushButton *btnDel;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_okcancel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnSave;
    QPushButton *btnCancel;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName("SettingsDialog");
        SettingsDialog->resize(356, 195);
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
        SettingsDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SettingsDialog);
        verticalLayout->setObjectName("verticalLayout");
        listUrl = new QListWidget(SettingsDialog);
        listUrl->setObjectName("listUrl");

        verticalLayout->addWidget(listUrl);

        horizontalLayout_btns = new QHBoxLayout();
        horizontalLayout_btns->setObjectName("horizontalLayout_btns");
        btnAdd = new QPushButton(SettingsDialog);
        btnAdd->setObjectName("btnAdd");

        horizontalLayout_btns->addWidget(btnAdd);

        btnDel = new QPushButton(SettingsDialog);
        btnDel->setObjectName("btnDel");

        horizontalLayout_btns->addWidget(btnDel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_btns->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_btns);

        horizontalLayout_okcancel = new QHBoxLayout();
        horizontalLayout_okcancel->setObjectName("horizontalLayout_okcancel");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_okcancel->addItem(horizontalSpacer_2);

        btnSave = new QPushButton(SettingsDialog);
        btnSave->setObjectName("btnSave");

        horizontalLayout_okcancel->addWidget(btnSave);

        btnCancel = new QPushButton(SettingsDialog);
        btnCancel->setObjectName("btnCancel");

        horizontalLayout_okcancel->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout_okcancel);


        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "IPOverlay \350\256\276\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        listUrl->setToolTip(QCoreApplication::translate("SettingsDialog", "\345\205\254\347\275\221\346\237\245\350\257\242URL\345\210\227\350\241\250\357\274\214\347\250\213\345\272\217\344\273\205\344\275\277\347\224\250\347\254\254\344\270\200\346\235\241", nullptr));
#endif // QT_CONFIG(tooltip)
        btnAdd->setText(QCoreApplication::translate("SettingsDialog", "\346\267\273\345\212\240", nullptr));
        btnDel->setText(QCoreApplication::translate("SettingsDialog", "\345\210\240\351\231\244", nullptr));
        btnSave->setText(QCoreApplication::translate("SettingsDialog", "\344\277\235\345\255\230", nullptr));
        btnCancel->setText(QCoreApplication::translate("SettingsDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
