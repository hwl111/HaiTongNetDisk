#include "tcpclient.h"
#include "ui_tcpclient.h"
#include<QByteArray>
#include<QDebug>
#include<QMessageBox>
#include<QHostAddress>
#include"privatechat.h"

TcpCLient::TcpCLient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpCLient)
{
    ui->setupUi(this);
    loadConfig();
    connect(&m_tcpSocket, SIGNAL(connected())
            , this, SLOT(showConnect()));
    connect(&m_tcpSocket, SIGNAL(readyRead())
            , this, SLOT(recvMsg()));

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

TcpCLient &TcpCLient::getInstance()
{
    static TcpCLient instance;
    return instance;
}

QTcpSocket &TcpCLient::getTcpSocket()
{
    return m_tcpSocket;
}

QString TcpCLient::loginName()
{
    return m_strLoginName;
}

void TcpCLient::showConnect()
{
    QMessageBox::information(this, "连接服务器", "连接服务器成功");
}

void TcpCLient::recvMsg()
{
    qDebug()<<m_tcpSocket.bytesAvailable();
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen, sizeof(uint));   //获得总的大小
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);                   //用来接收剩余数据的PDU
    m_tcpSocket.read((char*)pdu + sizeof(uint), uiPDULen - sizeof(uint));
    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:           //注册请求
    {
        if(0 == strcmp(pdu->caData, REGIST_OK))
        {
            QMessageBox::information(this, "注册", REGIST_OK);
        }
        else if(0 == strcmp(pdu->caData, REGIST_FAILED))
        {
            QMessageBox::warning(this, "注册", REGIST_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:           //登录请求
    {
        if(0 == strcmp(pdu->caData, LOGIN_OK))
        {
            QMessageBox::information(this, "登录", LOGIN_OK);
            OpeWidget::getInstance().show();
            this->hide();   //隐藏登陆界面
        }
        else if(0 == strcmp(pdu->caData, LOGIN_FAILED))
        {
            QMessageBox::warning(this, "登录", LOGIN_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
    {
        OpeWidget::getInstance().getFriend()->showALLOnlineUsr(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USR_RESPOND:
    {
        if(strcmp(SEARCH_USR_NO, pdu->caData) == 0)
        {
            QMessageBox::information(this, "搜索", QString("%1: not exist").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        else if(strcmp(SEARCH_USR_ONLINE, pdu->caData) == 0)
        {
            QMessageBox::information(this, "搜索", QString("%1: online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        else if(strcmp(SEARCG_USR_OFFLINE, pdu->caData) == 0)
        {
            QMessageBox::information(this, "搜索", QString("%1: not online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
        }
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RREQUEST:
    {
        char caName[32] = {'\0'};//请求添加好友的名称
        strncpy(caName, pdu->caData + 32, 32);
        int ret = QMessageBox::information(this, "添加好友", QString("%1 want to add you as friend.").arg(caName), QMessageBox::Yes, QMessageBox::No);
        PDU *retPdu = mkPDU(0);
        memcpy(retPdu->caData, pdu->caData, 64);
        if(ret == QMessageBox::Yes)
        {
            retPdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
        }
        else
        {
            retPdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
        }
        m_tcpSocket.write((char*)retPdu, retPdu->uiPDULen);
        free(retPdu);
        retPdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
    {
        QMessageBox::information(this, "添加好友", pdu->caData);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:
    {
        QMessageBox::information(this, "添加好友", QString("%1 已同意您的好友申请！").arg(pdu->caData));
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:
    {
        QMessageBox::information(this, "添加好友", QString("%1 已拒绝您的好友申请！").arg(pdu->caData));
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:
    {
        OpeWidget::getInstance().getFriend()->updataFriendList(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
    {
        char caName[32] = {'\0'};
        memcpy(caName, pdu->caData, 32);
        QMessageBox::information(this, "删除好友", QString("%1 删除你作为他的好友").arg(caName));
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND:
    {
        QMessageBox::information(this, "删除好友", DELETE_FRIEND_OK);
        break;
    }
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
    {
        if(PrivateChat::getInstance().isHidden())
        {
            PrivateChat::getInstance().show();
        }
        char caSendName[32] = {'\0'};
        memcpy(caSendName, pdu->caData, 32);
        QString strSendName = caSendName;
        PrivateChat::getInstance().setChatName(strSendName);
        PrivateChat::getInstance().updateMsg(pdu);
        break;
    }
    default:
        break;
    }

    free(pdu);
    pdu = NULL;
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
    //获取输入的用户名和密码
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty() && !strPwd.isEmpty())
    {
        m_strLoginName = strName;    //保存登录用户名
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);  //拷贝用户名
        strncpy(pdu->caData+32, strPwd.toStdString().c_str(), 32);//拷贝密码
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);         //发送
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "登录", "登录失败:用户名与密码不能为空");
    }
}

//注册
void TcpCLient::on_regist_pb_clicked()
{
    //获取输入的用户名和密码
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty() && !strPwd.isEmpty())
    {
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);  //拷贝用户名
        strncpy(pdu->caData+32, strPwd.toStdString().c_str(), 32);//拷贝密码
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);         //发送
        free(pdu);
        pdu = NULL;
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

