#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include"mytcpserver.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class TcpServer;
}
QT_END_NAMESPACE

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
    void loadConfig();   //加载配置文件

private:
    Ui::TcpServer *ui;
    QString m_strIP;    //存放IP
    quint16 m_usPort;   //存放端口
};
#endif // TCPSERVER_H
