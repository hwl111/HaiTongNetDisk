#include "mytcpsocket.h"
#include <QDebug>
MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead())
            , this, SLOT(recvMsg()));      //关联recvMsg()
    connect(this, SIGNAL(disconnected())
            ,this, SLOT(client_close()));  //只要断开连接就调用client_close()
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::recvMsg()
{
    qDebug()<<this->bytesAvailable();
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen, sizeof(uint));   //获得总的大小
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);                   //用来接收剩余数据的PDU
    this->read((char*)pdu + sizeof(uint), uiPDULen - sizeof(uint));
    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:           //注册请求
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        strncpy(caPwd, pdu->caData+32, 32);
        bool ret = OpeDB::getInstance().handleRegist(caName, caPwd);
        PDU *respdu = mkPDU(0);               //回复注册是否成功
        respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;    //注册回复
        if(ret)
        {
            strcpy(respdu->caData, REGIST_OK);
        }
        else
        {
            strcpy(respdu->caData, REGIST_FAILED);
        }
        write((char*)respdu, respdu->uiPDULen);         //发送
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:                //登录请求
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        strncpy(caPwd, pdu->caData+32, 32);
        bool ret = OpeDB::getInstance().handleLogin(caName, caPwd);
        PDU *respdu = mkPDU(0);               //回复登录是否成功
        respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;    //登录回复
        if(ret)
        {
            strcpy(respdu->caData, LOGIN_OK);
            m_strName = caName;                      //记录登录的名字
        }
        else
        {
            strcpy(respdu->caData, LOGIN_FAILED);
        }
        write((char*)respdu, respdu->uiPDULen);         //发送
        free(respdu);
        respdu = NULL;
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;
}

void MyTcpSocket::client_close()
{
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    emit offline(this);
}
