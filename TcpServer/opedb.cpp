#include "opedb.h"
#include<QMessageBox>
#include<QDebug>
OpeDB::OpeDB(QObject *parent): QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    m_db.setHostName("localhost");        //连接本地数据库
    m_db.setDatabaseName("C:\\Users\\43474\\Desktop\\QTproject\\TcpServer\\cloud.db");   //添加数据库
    if(m_db.open())
    {
        //查询数据库
        QSqlQuery query;
        query.exec("select * from usrInfo");
        while(query.next())
        {
            QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
            qDebug()<<data;
        }

    }
    else
    {
        QMessageBox::critical(NULL, "打开数据库", "打开数据库失败");
    }
}

OpeDB::~OpeDB()
{
    m_db.close();      //关闭数据库
}

bool OpeDB::handleRegist(const char *name, const char *pwd)
{
    if(NULL == name || NULL == pwd)
    {
        return false;
    }
    //插入数据库
    QString data = QString("insert into usrInfo(name, pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery query;
    return query.exec(data);
}

bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    if(NULL == name || NULL == pwd)
    {
        return false;
    }
    //查询数据库
    QString data = QString("select * from usrInfo where name=\'%1\' and pwd=\'%2\' and online=0").arg(name).arg(pwd);
    QSqlQuery query;
    query.exec(data);
    if(query.next())
    {
        //登陆成功,修改online的值为1
        QString data = QString("update usrInfo set online=1 where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
        QSqlQuery query;
        query.exec(data);
        return true;
    }
    else
    {
        return false;
    }
}
