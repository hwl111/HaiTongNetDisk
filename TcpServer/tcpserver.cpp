#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"
#include<QByteArray>
#include<QDebug>
#include<QMessageBox>
#include<QHostAddress>
#include<QFile>


TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();                          //加载配置文件
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP), m_usPort);   //监听
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly))                   //只读打开文件
    {
        QByteArray baData = file.readAll();              //读取数据
        QString strData = baData.toStdString().c_str();  //字节转换成字符串
        //qDebug() <<strData;                            //打印
        file.close();                                    //关闭文件

        strData.replace("\r\n", " ");                    //用空格替换\r\n
        //qDebug() <<strData;
        QStringList strList =  strData.split(" ");       //按空格切分
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toShort();              //字符串转化成无符号整型

    }
    else
    {
        QMessageBox::critical(this, "open config ", "open config failed");
    }
}
