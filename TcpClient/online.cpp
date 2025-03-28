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
    if(NULL == pdu)
    {
        return;
    }
    char caTmp[32];   //临时数组用于存储信息
    uint uiSize = pdu->uiMSgLen/32;  //消息个数
    for(uint i=0;i<uiSize;i++)
    {
        memcpy(caTmp, (char*)(pdu->caMsg)+i*32, 32);  //拷贝数据
        ui->online_lw->addItem(caTmp);                //显示数据到界面上
    }
}
