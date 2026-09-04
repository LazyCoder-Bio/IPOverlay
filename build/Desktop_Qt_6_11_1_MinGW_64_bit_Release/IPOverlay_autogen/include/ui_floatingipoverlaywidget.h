/********************************************************************************
** Form generated from reading UI file 'floatingipoverlaywidget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLOATINGIPOVERLAYWIDGET_H
#define UI_FLOATINGIPOVERLAYWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FloatingIpoverlayWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *m_PublicIp_Label;
    QLabel *m_PublicIp;
    QHBoxLayout *horizontalLayout;
    QLabel *m_LocalIp_Label;
    QLabel *m_LocalIp;

    void setupUi(QWidget *FloatingIpoverlayWidget)
    {
        if (FloatingIpoverlayWidget->objectName().isEmpty())
            FloatingIpoverlayWidget->setObjectName("FloatingIpoverlayWidget");
        FloatingIpoverlayWidget->resize(184, 82);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/IPOverlay.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        FloatingIpoverlayWidget->setWindowIcon(icon);
        FloatingIpoverlayWidget->setStyleSheet(QString::fromUtf8("background-color:rgba(0,0,0,140); /*\345\215\212\351\200\217\351\273\221\350\211\262\345\272\225\346\235\277\357\274\214\346\234\200\345\220\216\346\225\260\345\255\2270\342\200\221255\346\216\247\345\210\266\351\200\217\346\230\216\345\272\246*/\n"
"    border-radius:12px;"));
        verticalLayout = new QVBoxLayout(FloatingIpoverlayWidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        m_PublicIp_Label = new QLabel(FloatingIpoverlayWidget);
        m_PublicIp_Label->setObjectName("m_PublicIp_Label");
        QFont font;
        font.setPointSize(13);
        m_PublicIp_Label->setFont(font);
        m_PublicIp_Label->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);\n"
"background-color: transparent;"));

        horizontalLayout_2->addWidget(m_PublicIp_Label);

        m_PublicIp = new QLabel(FloatingIpoverlayWidget);
        m_PublicIp->setObjectName("m_PublicIp");
        m_PublicIp->setFont(font);
        m_PublicIp->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 0);\n"
"background-color: transparent;"));

        horizontalLayout_2->addWidget(m_PublicIp);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        m_LocalIp_Label = new QLabel(FloatingIpoverlayWidget);
        m_LocalIp_Label->setObjectName("m_LocalIp_Label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_LocalIp_Label->sizePolicy().hasHeightForWidth());
        m_LocalIp_Label->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(9);
        m_LocalIp_Label->setFont(font1);

        horizontalLayout->addWidget(m_LocalIp_Label);

        m_LocalIp = new QLabel(FloatingIpoverlayWidget);
        m_LocalIp->setObjectName("m_LocalIp");

        horizontalLayout->addWidget(m_LocalIp);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(FloatingIpoverlayWidget);

        QMetaObject::connectSlotsByName(FloatingIpoverlayWidget);
    } // setupUi

    void retranslateUi(QWidget *FloatingIpoverlayWidget)
    {
        FloatingIpoverlayWidget->setWindowTitle(QCoreApplication::translate("FloatingIpoverlayWidget", "FloatingIpoverlayWidget", nullptr));
        m_PublicIp_Label->setText(QCoreApplication::translate("FloatingIpoverlayWidget", "\345\205\254\347\275\221IP\357\274\232", nullptr));
        m_PublicIp->setText(QString());
        m_LocalIp_Label->setText(QCoreApplication::translate("FloatingIpoverlayWidget", "\345\206\205\347\275\221IP\357\274\232", nullptr));
        m_LocalIp->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FloatingIpoverlayWidget: public Ui_FloatingIpoverlayWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLOATINGIPOVERLAYWIDGET_H
