#include "friend.h"
#include"protocol.h"
#include"tcpclient.h"
#include<QInputDialog>  //输入数据
#include<QDebug>

Friend::Friend(QWidget *parent)
    : QWidget{parent}
{

    m_pShowMsgTe = new QTextEdit;                 //显示信息
    m_pFriendListWidgw = new QListWidget;         //显示好友列表
    m_pInputMsgLE = new QLineEdit;                //信息输入框

    m_pDelFriendPB = new QPushButton("删除好友");             //删除好友按钮
    m_pFlushFriendPB = new QPushButton("刷新好友");           //刷新在线好友列表
    m_pShowOnlineUsrPB = new QPushButton("显示在线用户");      //查看在线用户
    m_pSearchUsrPB = new QPushButton("查找用户");             //查找用户
    m_pMsgSendPB = new QPushButton("发送信息");               //发送信息
    m_pPrivateChatPB = new QPushButton("私聊");              //私聊

    //按钮垂直布局
    QVBoxLayout *pRightPBVBL = new QVBoxLayout;
    pRightPBVBL->addWidget(m_pDelFriendPB);
    pRightPBVBL->addWidget(m_pFlushFriendPB);
    pRightPBVBL->addWidget(m_pShowOnlineUsrPB);
    pRightPBVBL->addWidget(m_pSearchUsrPB);
    pRightPBVBL->addWidget(m_pPrivateChatPB);

    QHBoxLayout *pTopHBL = new QHBoxLayout;
    pTopHBL->addWidget(m_pShowMsgTe);
    pTopHBL->addWidget(m_pFriendListWidgw);
    pTopHBL->addLayout(pRightPBVBL);

    QHBoxLayout *pMsgHBL = new QHBoxLayout;
    pMsgHBL->addWidget(m_pInputMsgLE);
    pMsgHBL->addWidget(m_pMsgSendPB);

    m_pOnline = new Online;

    QVBoxLayout *pMain = new QVBoxLayout;
    pMain->addLayout(pTopHBL);
    pMain->addLayout(pMsgHBL);
    pMain->addWidget(m_pOnline);
    m_pOnline->hide();


    setLayout(pMain);

    //关联信号槽
    connect(m_pShowOnlineUsrPB, SIGNAL(clicked(bool))
            , this, SLOT(showOnline()));
    connect(m_pSearchUsrPB, SIGNAL(clicked(bool))
            , this, SLOT(searchUsr()));
}

void Friend::showALLOnlineUsr(PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    m_pOnline->showUsr(pdu);
}

void Friend::showOnline()
{
    if (m_pOnline->isHidden())
    {
        m_pOnline->show();   //点击在线用户显示
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpCLient::grtInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen); //发送
        free(pdu);
    }
    else
    {
        m_pOnline->hide();  //再次点击就隐藏
    }
}

void Friend::searchUsr()
{
    m_strSearchName = QInputDialog::getText(this, "搜索", "用户名:");
    if(!m_strSearchName.isEmpty())
    {
        qDebug()<<m_strSearchName;   //打印测试
        PDU *pdu = mkPDU(0);
        memcpy(pdu->caData, m_strSearchName.toStdString().c_str(), m_strSearchName.size());
        pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_REQUEST;  //查找用户请求

        TcpCLient::grtInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen); //发送
        free(pdu);
        pdu = NULL;
    }
}
