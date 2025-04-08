#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include"protocol.h"
#include"opedb.h"
#include<QDir>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString getName();

signals:
    void offline(MyTcpSocket *mysocket);           //下线的信号

public slots:
    void recvMsg();
    void client_close();                //用于处理客户端下线

private:
    QString m_strName;
};

#endif // MYTCPSOCKET_H
