#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include<QList>
#include"mytcpsocket.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT          //支持信号槽的宏
public:
    MyTcpServer();

    static MyTcpServer &getInstance();
    void incomingConnection(qintptr socketDescriptor);

private:
    QList<MyTcpSocket*> m_tcpSocketList;   //用于保存所有的socket
};

#endif // MYTCPSERVER_H
