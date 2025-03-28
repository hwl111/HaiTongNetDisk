#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include<QSqlDatabase>   //连接数据库模块
#include<QSqlQuery>      //查询数据库
#include<QStringList>
class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    //定义成单例
    static OpeDB& getInstance();
    void init();                 //初始化时连接数据库
    ~OpeDB();

    bool handleRegist(const char *name, const char *pwd);         //处理注册
    bool handleLogin(const char *name, const char *pwd);          //处理登录
    void handleOffline(const char *name);                         //处理下线
    QStringList handleALLOnline();                                //处理在线用户
    int handleSearchUsr(const char * name);                       //查找用户

signals:

public slots:

private:
    QSqlDatabase m_db;    //连接数据库
};

#endif // OPEDB_H
