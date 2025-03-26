#include "tcpclient.h"
#include "ui_tcpclient.h"
#include<QByteArray>
#include<QDebug>
#include<QMessageBox>
#include<QHostAddress>

TcpCLient::TcpCLient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpCLient)
{
    ui->setupUi(this);
    loadConfig();
    connect(&m_tcpSocket, SIGNAL(connected())
            , this, SLOT(showConnect()));
    //连接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP), m_usPort);
}

TcpCLient::~TcpCLient()
{
    delete ui;
}

void TcpCLient::loadConfig()
{
    QFile file(":/client.config");
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

void TcpCLient::showConnect()
{
    QMessageBox::information(this, "连接服务器", "连接服务器成功");
}

#if 0
void TcpCLient::on_pushButton_clicked()
{
    //发送的信号槽
    QString strMsg = ui->lineEdit->text();   // 获得输入框数据
    if (!strMsg.isEmpty())
    {
        PDU *pdu = mkPDU(strMsg.size()+1);
        pdu->uiMsgType = 8888;
        memcpy(pdu->caMsg, strMsg.toStdString().c_str(), strMsg.size());
        qDebug()<<pdu->caMsg;
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);         //发送
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this, "信息发送", "发送信息不能为空");
    }
}
#endif

//登录
void TcpCLient::on_login_pb_clicked()
{

}

//注册
void TcpCLient::on_regist_pb_clicked()
{
    //获取输入的用户名和密码
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty() && !strPwd.isEmpty())
    {

    }
    else
    {
        QMessageBox::critical(this, "注册", "注册失败:用户名与密码不能为空");
    }
}

//注销
void TcpCLient::on_cancel_pb_clicked()
{

}

