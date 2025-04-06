#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include<QTextEdit>
#include<QListWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include"online.h"

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);
    void showALLOnlineUsr(PDU *pdu);              //显示在线用户
    QString m_strSearchName;             //保存查找用户名字
    void updataFriendList(PDU *pdu);     //刷新好友列表

private:
    QTextEdit *m_pShowMsgTe;             //显示信息
    QListWidget *m_pFriendListWidgw;     //显示好友列表
    QLineEdit *m_pInputMsgLE;            //信息输入框

    QPushButton *m_pDelFriendPB;         //删除好友按钮
    QPushButton *m_pFlushFriendPB;       //刷新在线好友列表
    QPushButton *m_pShowOnlineUsrPB;     //查看在线用户
    QPushButton *m_pSearchUsrPB;         //查找用户

    QPushButton *m_pMsgSendPB;           //发送信息
    QPushButton *m_pPrivateChatPB;       //私聊

    Online *m_pOnline;


signals:

public slots:
    void showOnline();                    //用于显示在线用户的信号槽
    void searchUsr();                     //查找用户信号槽
    void flushFriend();                   //刷新好友列表
    void delFriend();                     //删除好友
};

#endif // FRIEND_H
