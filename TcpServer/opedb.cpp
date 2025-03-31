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

void OpeDB::handleOffline(const char *name)
{
    if(NULL == name)
    {
        return;
    }
    QString data = QString("update usrInfo set online=0 where name=\'%1\'").arg(name);
    QSqlQuery query;
    query.exec(data);
}

QStringList OpeDB::handleALLOnline()
{
    QString data = QString("select name from usrInfo where online=1");

    QSqlQuery query;
    query.exec(data);
    QStringList result;
    result.clear();

    while(query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

int OpeDB::handleSearchUsr(const char *name)
{
    if(NULL == name)
    {
        return -1;  //不存在
    }
    QString data = QString("select online from usrInfo where name=\'%1\'").arg(name);
    QSqlQuery query;
    query.exec(data);
    if(query.next())
    {
        int ret = query.value(0).toInt();  //判断是否在线
        if(ret == 1)
        {
            return ret;  //在线
        }
        else if(ret == 0)
        {
            return ret;      //不在线
        }
    }
    else
    {
        return -1;  //不存在
    }

}

int OpeDB::handleAddFriend(const char *pername, const char *name)
{
    if(pername == NULL || name == NULL)
    {
        return -1;
    }
    QString data = QString("select * from friend where (id=(select id from usrInfo where name=\'%1\') and friendId = (select id from usrInfo where name=\'%2\'))"
                           " or (id=(select id from usrInfo where name=\'%3\') and friendId = (select id from usrInfo where name=\'%4\'))").arg(pername).arg(name).arg(name).arg(pername);

    QSqlQuery query;
    query.exec(data);
    if(query.next())
    {
        return 0;   //双方已是好友
    }
    else
    {
        QString data = QString("select online from usrInfo where name=\'%1\'").arg(name);
        QSqlQuery query;
        query.exec(data);
        if(query.next())
        {
            int ret = query.value(0).toInt();  //判断是否在线
            if(ret == 1)
            {
                return 1;  //在线
            }
            else if(ret == 0)
            {
                return 2;      //不在线
            }
        }
        else
        {
            return 3;  //不存在
        }

    }
}

void OpeDB::handleAddfriend(const char *friendName, const char *name)
{
    // 获取好友id
    QString strSql = QString("select id from userInfo where name = '%1'").arg(friendName);
    QSqlQuery query;
    query.exec(strSql);
    query.next();
    int friendId = query.value(0).toInt();
    // 获取登录用户id
    strSql = QString("select id from userInfo where name = '%1'").arg(name);
    query.exec(strSql);
    query.next();
    int loginId = query.value(0).toInt();
    // 新增好友
    strSql = QString("insert into friend (id,friendId) values ('%1','%2'), ('%2','%1')").arg(friendId).arg(loginId);
    query.exec(strSql);
}

int OpeDB::getIdByUserName(const char *name)
{
    if(NULL == name)
    {
        return -1;
    }
    QString data = QString("select id from userInfo where name=\'%1\'").arg(name);
    QSqlQuery query;
    query.exec(data);
    qDebug() << data;
    if(query.next())
    {
        return query.value(0).toInt();
    }
    else
    {
        return -1; // 不存在该用户
    }
}

void OpeDB::handleAddFriendAgree(const char *addedName, const char *sourceName)
{
    if(NULL == addedName || NULL == sourceName)
    {
        return;
    }
    int sourceUserId = -1;
    int addedUserId = -1;
    sourceUserId = getIdByUserName(sourceName);
    addedUserId = getIdByUserName(addedName);
    qDebug() << sourceUserId << " " << addedUserId;
    QString strQuery = QString("insert into friend values(%1, %2) ").arg(sourceUserId).arg(addedUserId);
    QSqlQuery query;
    query.exec(strQuery);
    return;

    //qDebug() << "handleAddFriendAgree " << strQuery;
}
