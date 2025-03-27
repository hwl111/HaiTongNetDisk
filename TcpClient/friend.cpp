#include "friend.h"

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

    connect(m_pShowOnlineUsrPB, SIGNAL(clicked(bool))
            , this, SLOT(showOnline()));
}

void Friend::showOnline()
{
    if (m_pOnline->isHidden())
    {
        m_pOnline->show();   //点击在线用户显示
    }
    else
    {
        m_pOnline->hide();  //再次点击就隐藏
    }
}
