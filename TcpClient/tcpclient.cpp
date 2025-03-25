#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpCLient::TcpCLient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpCLient)
{
    ui->setupUi(this);
}

TcpCLient::~TcpCLient()
{
    delete ui;
}
