/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpCLient
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *name_le;
    QLineEdit *pwd_le;
    QLabel *pwd_lan;
    QLabel *name_lan;
    QHBoxLayout *horizontalLayout;
    QPushButton *regist_pb;
    QPushButton *login_pb;
    QPushButton *cancel_pb;

    void setupUi(QWidget *TcpCLient)
    {
        if (TcpCLient->objectName().isEmpty())
            TcpCLient->setObjectName("TcpCLient");
        TcpCLient->resize(598, 656);
        horizontalLayout_2 = new QHBoxLayout(TcpCLient);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        name_le = new QLineEdit(TcpCLient);
        name_le->setObjectName("name_le");
        QFont font;
        font.setPointSize(16);
        name_le->setFont(font);

        gridLayout->addWidget(name_le, 0, 1, 1, 1);

        pwd_le = new QLineEdit(TcpCLient);
        pwd_le->setObjectName("pwd_le");
        pwd_le->setFont(font);
        pwd_le->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(pwd_le, 1, 1, 1, 1);

        pwd_lan = new QLabel(TcpCLient);
        pwd_lan->setObjectName("pwd_lan");
        pwd_lan->setFont(font);

        gridLayout->addWidget(pwd_lan, 1, 0, 1, 1);

        name_lan = new QLabel(TcpCLient);
        name_lan->setObjectName("name_lan");
        name_lan->setFont(font);

        gridLayout->addWidget(name_lan, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        regist_pb = new QPushButton(TcpCLient);
        regist_pb->setObjectName("regist_pb");
        QFont font1;
        font1.setPointSize(14);
        regist_pb->setFont(font1);

        horizontalLayout->addWidget(regist_pb);

        login_pb = new QPushButton(TcpCLient);
        login_pb->setObjectName("login_pb");
        login_pb->setFont(font1);

        horizontalLayout->addWidget(login_pb);

        cancel_pb = new QPushButton(TcpCLient);
        cancel_pb->setObjectName("cancel_pb");
        cancel_pb->setFont(font1);

        horizontalLayout->addWidget(cancel_pb);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(TcpCLient);

        QMetaObject::connectSlotsByName(TcpCLient);
    } // setupUi

    void retranslateUi(QWidget *TcpCLient)
    {
        TcpCLient->setWindowTitle(QCoreApplication::translate("TcpCLient", "TcpCLient", nullptr));
        pwd_lan->setText(QCoreApplication::translate("TcpCLient", "\345\257\206\347\240\201:", nullptr));
        name_lan->setText(QCoreApplication::translate("TcpCLient", "\347\224\250\346\210\267\345\220\215:", nullptr));
        regist_pb->setText(QCoreApplication::translate("TcpCLient", "\346\263\250\345\206\214", nullptr));
        login_pb->setText(QCoreApplication::translate("TcpCLient", "\347\231\273\345\275\225", nullptr));
        cancel_pb->setText(QCoreApplication::translate("TcpCLient", "\346\263\250\351\224\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpCLient: public Ui_TcpCLient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
