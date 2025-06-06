#include "friend.h"
#include"protocol.h"
#include"tcpclient.h"
#include<QInputDialog>  //输入数据
#include<QDebug>
#include"privatechat.h"
#include<QMessageBox>

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
    connect(m_pFlushFriendPB, SIGNAL(clicked(bool))
            , this, SLOT(flushFriend()));
    connect(m_pDelFriendPB, SIGNAL(clicked(bool))
            , this, SLOT(delFriend()));
    connect(m_pPrivateChatPB, SIGNAL(clicked(bool))
            , this, SLOT(privateChat()));
    connect(m_pMsgSendPB, SIGNAL(clicked(bool))
            , this, SLOT(groupChat()));
}

void Friend::showALLOnlineUsr(PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    m_pOnline->showUsr(pdu);
}

void Friend::updataFriendList(PDU *pdu)
{
    if(pdu == NULL)
    {
        return;
    }
    uint uiSize = pdu->uiMSgLen/32;
    char caName[32] = {'\0'};
    for(uint i =0;i<uiSize; i++)
    {
        memcpy(caName, (char*)(pdu->caMsg)+i*32, 32);
        m_pFriendListWidgw->addItem(caName);
    }
}

void Friend::updateGroupMsg(PDU *pdu)
{
    QString strMsg = QString("%1 says: %2").arg(pdu->caData).arg((char*)(pdu->caMsg));
    m_pShowMsgTe->append(strMsg);
}

void Friend::showOnline()
{
    if (m_pOnline->isHidden())
    {
        m_pOnline->show();   //点击在线用户显示
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpCLient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen); //发送
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

        TcpCLient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen); //发送
        free(pdu);
        pdu = NULL;
    }
}

void Friend::flushFriend()
{
    //清空当前显示,避免重复显示的问题
    m_pFriendListWidgw->clear();
    QString strName = TcpCLient::getInstance().loginName();  //获得登录用户名
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData, strName.toStdString().c_str(), strName.size());

    TcpCLient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen); //发送
    free(pdu);
    pdu = NULL;
}

void Friend::delFriend()
{

    if(m_pFriendListWidgw->currentItem() != NULL)
    {
        QString strFriendName = m_pFriendListWidgw->currentItem()->text();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        QString strSelfName = TcpCLient::getInstance().loginName();
        memcpy(pdu->caData,strSelfName.toStdString().c_str(), strSelfName.size());
        memcpy(pdu->caData+32,strFriendName.toStdString().c_str(),strFriendName.size());
        TcpCLient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen); //发送
        free(pdu);
        pdu = NULL;
    }
}

void Friend::privateChat()
{
    if(m_pFriendListWidgw->currentItem() != NULL)
    {
        QString strChatName = m_pFriendListWidgw->currentItem()->text();
        PrivateChat::getInstance().setChatName(strChatName);
        if(PrivateChat::getInstance().isHidden())
        {
            PrivateChat::getInstance().show();
        }
    }
    else
    {
        QMessageBox::warning(this, "私聊", "请选择私聊对象");
    }
}

void Friend::groupChat()
{
    QString strMsg = m_pInputMsgLE->text();         //获得聊天数据
    m_pInputMsgLE->clear();                         //清除发送框的信息
    if(!strMsg.isEmpty())
    {
        PDU *pdu = mkPDU(strMsg.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
        QString strName = TcpCLient::getInstance().loginName();
        strncpy(pdu->caData, strName.toStdString().c_str(), strName.size());   //获得发送者名字
        strncpy((char*)pdu->caMsg, strMsg.toStdString().c_str(), strMsg.size());  //获得发送消息
        TcpCLient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);  //发送给服务器
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this, "群聊", "发送消息不能为空");
    }
}
