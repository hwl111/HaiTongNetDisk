#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include"protocol.h"
#include"opedb.h"
#include<QDir>
#include<QFile>
#include<QTimer>
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
    void sendFileToClient();            //发送文件给客户端

private:
    QString m_strName;
    QFile m_file;
    qint64 m_iTotal = 0;                      //文件总大小
    qint64 m_iRecved = 0;                     //文件接收多少
    bool m_bUpload;                           //是否处于上传文件状态
    QTimer *m_pTimer;                         //定时发送文件
};

#endif // MYTCPSOCKET_H
