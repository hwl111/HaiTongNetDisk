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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpCLient
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *TcpCLient)
    {
        if (TcpCLient->objectName().isEmpty())
            TcpCLient->setObjectName("TcpCLient");
        TcpCLient->resize(602, 600);
        verticalLayout_2 = new QVBoxLayout(TcpCLient);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        textEdit = new QTextEdit(TcpCLient);
        textEdit->setObjectName("textEdit");
        QFont font;
        font.setPointSize(20);
        textEdit->setFont(font);

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        lineEdit = new QLineEdit(TcpCLient);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(TcpCLient);
        pushButton->setObjectName("pushButton");
        QFont font1;
        font1.setPointSize(24);
        pushButton->setFont(font1);

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(TcpCLient);

        QMetaObject::connectSlotsByName(TcpCLient);
    } // setupUi

    void retranslateUi(QWidget *TcpCLient)
    {
        TcpCLient->setWindowTitle(QCoreApplication::translate("TcpCLient", "TcpCLient", nullptr));
        pushButton->setText(QCoreApplication::translate("TcpCLient", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpCLient: public Ui_TcpCLient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
