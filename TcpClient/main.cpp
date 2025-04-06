#include "tcpclient.h"

#include <QApplication>

//#include<online.h>
// #include"opewidget.h"
//#include"friend.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Online w;
    // w.show();

    // TcpCLient w;
    // w.show();
    TcpCLient::getInstance().show();   //用单例产生
    // Friend w;
    // w.show();

    // OpeWidget w;
    // w.show();

    return a.exec();
}
