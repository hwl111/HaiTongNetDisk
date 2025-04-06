#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include"protocol.h"
namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = nullptr);
    ~PrivateChat();

    static PrivateChat &getInstance();

    void setChatName(QString strName);   //设置聊天对象
    void updateMsg(const PDU *pdu);      //显示聊天信息

private slots:
    void on_sendMsg_pb_clicked();

private:
    Ui::PrivateChat *ui;
    QString m_strChatName;   //保存聊天对象名
    QString m_strLoginName;  //保存登录用户名
};

#endif // PRIVATECHAT_H
