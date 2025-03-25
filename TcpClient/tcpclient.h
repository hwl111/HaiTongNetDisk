#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class TcpCLient;
}
QT_END_NAMESPACE

class TcpCLient : public QWidget
{
    Q_OBJECT

public:
    TcpCLient(QWidget *parent = nullptr);
    ~TcpCLient();

private:
    Ui::TcpCLient *ui;
};
#endif // TCPCLIENT_H
