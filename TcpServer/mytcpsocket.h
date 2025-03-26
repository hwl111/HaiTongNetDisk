#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include"protocol.h"
#include"opedb.h"
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();

public slots:
    void recvMsg();
};

#endif // MYTCPSOCKET_H
