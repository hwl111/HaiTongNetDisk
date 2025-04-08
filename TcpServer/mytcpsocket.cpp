#include "mytcpsocket.h"
#include <QDebug>
#include"mytcpserver.h"
#include<QDir>
#include<QFileInfoList>
MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead())
            , this, SLOT(recvMsg()));      //关联recvMsg()
    connect(this, SIGNAL(disconnected())
            ,this, SLOT(client_close()));  //只要断开连接就调用client_close()
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::recvMsg()  //接收数据
{
    qDebug()<<this->bytesAvailable();
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen, sizeof(uint));   //获得总的大小
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);                   //用来接收剩余数据的PDU
    this->read((char*)pdu + sizeof(uint), uiPDULen - sizeof(uint));
    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:           //注册请求
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        strncpy(caPwd, pdu->caData+32, 32);
        bool ret = OpeDB::getInstance().handleRegist(caName, caPwd);
        PDU *respdu = mkPDU(0);               //回复注册是否成功
        respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;    //注册回复
        if(ret)
        {
            strcpy(respdu->caData, REGIST_OK);
            QDir dir;
            dir.mkdir(QString("./%1").arg(caName));     //注册成功时用注册的用户名创建一个文件夹
        }
        else
        {
            strcpy(respdu->caData, REGIST_FAILED);
        }
        write((char*)respdu, respdu->uiPDULen);         //发送
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:                //登录请求
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        strncpy(caPwd, pdu->caData+32, 32);
        bool ret = OpeDB::getInstance().handleLogin(caName, caPwd);
        PDU *respdu = mkPDU(0);               //回复登录是否成功
        respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;    //登录回复
        if(ret)
        {
            strcpy(respdu->caData, LOGIN_OK);
            m_strName = caName;                      //记录登录的名字
        }
        else
        {
            strcpy(respdu->caData, LOGIN_FAILED);
        }
        write((char*)respdu, respdu->uiPDULen);         //发送
        free(respdu);
        respdu = NULL;
        break;
    }

    case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST:
    {
        QStringList ret = OpeDB::getInstance().handleALLOnline();

        uint uiMsgLen = ret.size()*32;  //消息的长度
        PDU *respdu = mkPDU(uiMsgLen);
        respdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;
        for(int i=0;i<ret.size();i++)
        {
            //拷贝用户名
            memcpy((char*)(respdu->caMsg)+i*32, ret.at(i).toStdString().c_str(),ret.at(i).size());
        }
        //发送pdu给客户端
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;

        break;
    }

    case ENUM_MSG_TYPE_SEARCH_USR_REQUEST:
    {
        int ret = OpeDB::getInstance().handleSearchUsr(pdu->caData);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_RESPOND;
        if(ret == -1)
        {
            strcpy(respdu->caData, SEARCH_USR_NO);
        }
        else if(ret == 1)
        {
            strcpy(respdu->caData, SEARCH_USR_ONLINE);
        }
        else if(ret == 0)
        {
            strcpy(respdu->caData, SEARCG_USR_OFFLINE);
        }
        //发送pdu给客户端
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RREQUEST:
    {
        char caPerName[32] = {'\0'};
        char caName[32] = {'\0'};
        strncpy(caPerName, pdu->caData, 32);
        strncpy(caName, pdu->caData+32, 32);
        int ret = OpeDB::getInstance().handleAddFriend(caPerName, caName);
        PDU *respdu = NULL;
        if(ret == -1)
        {
            respdu  = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData, UNKNOW_ERROR);
        }
        else if(ret == 0)
        {
            respdu  = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData, EXISTED_FRIEND);
        }
        else if(ret == 1)
        {
            MyTcpServer::getInstance().resend(caPerName, pdu);
            respdu = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData, ADD_FRIEND_OK); // 表示加好友请求已发送
        }
        else if(ret == 2)
        {
            respdu  = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData, ADD_FRIEND_OFFLINE);
        }
        else if(ret == 3)
        {
            respdu  = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData, ADD_FRIEND_NOEXIST);
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:
    {
        char addedName[32] = {'\0'};
        char sourceName[32] = {'\0'};
        // 拷贝读取的信息
        strncpy(addedName, pdu->caData, 32);
        strncpy(sourceName, pdu->caData + 32, 32);
        PDU *respdu = mkPDU(0);
        OpeDB::getInstance().handleAddFriendAgree(addedName, sourceName);
        respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
        // 将新的好友关系信息写入数据库

        // 服务器需要转发给发送好友请求方其被同意的消息
        MyTcpServer::getInstance().resend(sourceName, pdu);
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:
    {
        char sourceName[32] = {'\0'};
            // 拷贝读取的信息
        strncpy(sourceName, pdu -> caData + 32, 32);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
        // 服务器需要转发给发送好友请求方其被拒绝的消息
        MyTcpServer::getInstance().resend(sourceName, pdu);
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:
    {
        char caName[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        QStringList ret = OpeDB::getInstance().handleFlushFriend(caName);
        uint uiMsgLen = ret.size()*32;
        PDU *respdu = mkPDU(uiMsgLen);
        respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
        for(int i=0; i<ret.size();i++)
        {
            memcpy((char*)(respdu->caMsg)+i*32,ret.at(i).toStdString().c_str(), ret.at(i).size());
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
    {
        char caSelfName[32] = {'\0'};
        char caFriendName[32] = {'\0'};
        strncpy(caSelfName, pdu->caData, 32);
        strncpy(caFriendName, pdu->caData+32, 32);
        OpeDB::getInstance().handleDelFriend(caSelfName, caFriendName);

        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
        strcpy(respdu->caData,DELETE_FRIEND_OK);
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;

        MyTcpServer::getInstance().resend(caFriendName, pdu);

        break;
    }
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
    {
        char caPerName[32] = {'\0'};
        memcpy(caPerName, pdu->caData+32, 32);
        MyTcpServer::getInstance().resend(caPerName, pdu);

        break;
    }
    case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
    {
        char caName[32] = {'\0'};
        strncpy(caName, pdu->caData, 32);
        QStringList onlineFriend = OpeDB::getInstance().handleFlushFriend(caName);  //获得在线好友
        QString tmp;
        for(int i=0;i<onlineFriend.size();i++)
        {
            tmp = onlineFriend.at(i);
            MyTcpServer::getInstance().resend(tmp.toStdString().c_str(), pdu);
        }
        break;
    }
    case ENUM_MSG_TYPE_CREATE_DIR_REQUEST:   //创建文件夹
    {
        QDir dir;
        QString strCurPath = QString("%1").arg((char*)(pdu->caMsg));
        bool ret = dir.exists(QString(strCurPath));
        PDU *respdu = NULL;
        if(ret)   //当前目录存在
        {
            char caNewDir[32] = {'\0'};
            memcpy(caNewDir, pdu->caData+32, 32);
            QString strNewPath = strCurPath + "/" + caNewDir;  //拼接新路径
            ret = dir.exists(strNewPath);
            if(ret)   //创建的文件名已存在
            {
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                strcpy(respdu->caData, FILE_NAME_EXIST);
            }
            else
            {
                dir.mkdir(strNewPath);  //创建文件夹
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                strcpy(respdu->caData, CREATE_DIR_OK);
            }
        }
        else
        {
            respdu = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
            strcpy(respdu->caData, DIR_NO_EXIST);
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
    {
        char *pCurPath = new char[pdu->uiMSgLen];
        memcpy(pCurPath, pdu->caMsg, pdu->uiMSgLen);
        QDir dir(pCurPath);
        QFileInfoList fileInfoList = dir.entryInfoList();  //获得文件信息
        int iFileCount = fileInfoList.size();              //获得文件个数
        PDU *respdu = mkPDU(sizeof(FileInfo)*iFileCount);   //防止显示.和..文件夹
        respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
        FileInfo *pFileInfo = NULL;
        QString strFileName;                                //保存文件名
        for(int i=0;i<iFileCount;i++)
        {
            // if(QString(".") == fileInfoList[i].fileName() || QString("..") == fileInfoList[i].fileName())
            // {
            //     continue;
            // }
            pFileInfo = (FileInfo*)(respdu->caMsg) + i;
            strFileName = fileInfoList[i].fileName();        //获得文件名

            memcpy(pFileInfo->caFileName, strFileName.toStdString().c_str(),strFileName.size());
            if(fileInfoList[i].isDir())
            {
                //如果是文件夹
                pFileInfo->iFileType = 0;    //0表示是一个文件夹

            }
            else if(fileInfoList[i].isFile())
            {
                //是一个常规文件
                pFileInfo->iFileType = 1;
            }
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_DEL_DIR_REQUEST:
    {
        char caName[32] = {'\0'};
        strcpy(caName, pdu->caData);
        char *pPath = new char[pdu->uiMSgLen];   //路径长度
        memcpy(pPath, pdu->caMsg, pdu->uiMSgLen);
        QString strPath = QString("%1/%2").arg(pPath).arg(caName);

        QFileInfo fileInfo(strPath);
        bool ret = false;
        if(fileInfo.isDir())  //如果是文件夹
        {
            QDir dir;
            dir.setPath(strPath);
            ret = dir.removeRecursively();  //删除文件夹,会删除文件夹所有内容
        }
        else if(fileInfo.isFile())  //如果是常规文件
        {
            ret = false;
        }
        PDU *respdu = NULL;
        if(ret)
        {
            //删除成功
            respdu = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_RESPOND;
            memcpy(respdu->caData, DEL_DIR_OK, strlen(DEL_DIR_OK));
        }
        else
        {
            respdu = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_RESPOND;
            memcpy(respdu->caData, DEL_DIR_FAILURED, strlen(DEL_DIR_FAILURED));
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }
    case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:
    {
        char caOldName[32] = {'\0'};
        char caNewName[32] = {'\0'};
        strncpy(caOldName, pdu->caData, 32);
        strncpy(caNewName, pdu->caData+32, 32);

        char *pPath = new char[pdu->uiMSgLen];
        memcpy(pPath, pdu->caMsg, pdu->uiMSgLen);

        QString strOldPath = QString("%1/%2").arg(pPath).arg(caOldName);
        QString strNewPath = QString("%1/%2").arg(pPath).arg(caNewName);

        QDir dir;
        bool ret = dir.rename(strOldPath, strNewPath);    //重命名
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType =ENUM_MSG_TYPE_RENAME_FILE_RESPOND;
        if(ret)
        {
            strcpy(respdu->caData, RENAME_FILE_OK);
        }
        else
        {
            strcpy(respdu->caData, RENAME_FILE_FAILURED);
        }
        write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
        break;
    }

    default:
        break;
    }
    free(pdu);
    pdu = NULL;
}

void MyTcpSocket::client_close()
{
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    emit offline(this);
}
