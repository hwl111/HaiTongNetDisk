#include "mytcpsocket.h"
#include <QDebug>
MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead())
            , this, SLOT(recvMsg()));      //关联recvMsg()
}

void MyTcpSocket::recvMsg()
{
    qDebug()<<this->bytesAvailable();
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen, sizeof(uint));   //获得总的大小
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);                   //用来接收剩余数据的PDU
    this->read((char*)pdu + sizeof(uint), uiPDULen - sizeof(uint));
    qDebug()<<pdu->uiMsgType << (char*)pdu->caMsg;
}
