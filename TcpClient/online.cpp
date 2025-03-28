#include "online.h"
#include "ui_online.h"

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
