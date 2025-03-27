#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>                 //文件操作
#include<QTcpSocket>             //连接服务器和收发服务器数据
#include"protocol.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class TcpCLient;
}
QT_END_NAMESPACE

class TcpCLient : public QWidget
{
    Q_OBJECT

public:
    TcpCLient(QWidget *parent = nullptr);
    ~TcpCLient();
    void loadConfig();   //加载配置文件

public slots:
    void showConnect();
    void recvMsg();        //接收数据

private slots:
//    void on_pushButton_clicked();

    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_cancel_pb_clicked();

private:
    Ui::TcpCLient *ui;
    QString m_strIP;    //存放IP
    quint16 m_usPort;   //存放端口

    //连接服务器和服务器交互
    QTcpSocket m_tcpSocket;
};
#endif // TCPCLIENT_H
