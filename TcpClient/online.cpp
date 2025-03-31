#include "online.h"
#include "ui_online.h"
#include"tcpclient.h"

Online::Online(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Online)
{
    ui->setupUi(this);
}

Online::~Online()
{
    delete ui;
}

void Online::showUsr(PDU *pdu)
{
    if(NULL == pdu || 0 == pdu->uiMSgLen)
    {
        return;
    }

    // 1. 获取当前真正在线的用户列表
    QSet<QString> currentOnlineUsers;
    char caTmp[32];
    uint uiSize = pdu->uiMSgLen/32;
    for(uint i=0; i<uiSize; i++)
    {
        memcpy(caTmp, (char*)(pdu->caMsg)+i*32, 32);
        currentOnlineUsers.insert(QString(caTmp).trimmed());
    }

    // 2. 处理列表控件显示
    QListWidget* listWidget = ui->online_lw;

    // 先移除已下线的用户
    for(int i = listWidget->count()-1; i >= 0; --i)
    {
        QListWidgetItem* item = listWidget->item(i);
        if(!currentOnlineUsers.contains(item->text()))
        {
            delete listWidget->takeItem(i); // 移除不在线的用户
        }
    }

    // 再添加新上线的用户
    foreach(const QString& user, currentOnlineUsers)
    {
        bool found = false;
        for(int i = 0; i < listWidget->count(); ++i)
        {
            if(listWidget->item(i)->text() == user)
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            listWidget->addItem(user);
        }
    }
}

void Online::on_addFriend_pb_clicked()
{
    //加好友
    QListWidgetItem *pItem = ui->online_lw->currentItem();
    QString strPerUsrName = pItem->text();                 //获得对方用户名
    QString strLoginName =  TcpCLient::grtInstance().loginName();  //获得登录用户名
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType =ENUM_MSG_TYPE_ADD_FRIEND_RREQUEST;

    memcpy(pdu->caData, strPerUsrName.toStdString().c_str(), strPerUsrName.size());
    memcpy(pdu->caData+32, strLoginName.toStdString().c_str(), strLoginName.size());

    TcpCLient::grtInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen); //发送
    free(pdu);
    pdu = NULL;
}

