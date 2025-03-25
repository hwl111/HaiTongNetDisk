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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpCLient
{
public:

    void setupUi(QWidget *TcpCLient)
    {
        if (TcpCLient->objectName().isEmpty())
            TcpCLient->setObjectName("TcpCLient");
        TcpCLient->resize(800, 600);

        retranslateUi(TcpCLient);

        QMetaObject::connectSlotsByName(TcpCLient);
    } // setupUi

    void retranslateUi(QWidget *TcpCLient)
    {
        TcpCLient->setWindowTitle(QCoreApplication::translate("TcpCLient", "TcpCLient", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpCLient: public Ui_TcpCLient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
