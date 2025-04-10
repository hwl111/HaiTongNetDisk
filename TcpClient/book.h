#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include<QListWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"protocol.h"
#include<QTimer>

class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);
    void clearEnterDir();                         //清空保存的文件路径
    QString enterDir();                           //获得进入的文件夹路径
    void setDownloadStatus(bool status);          //设置download状态
    bool getDownloadStatus();                     //获得download状态
    QString getSaveFilePath();                    //获得保存文件路径

    qint64 m_iTotal = 0;                          //下载文件大小
    qint64 m_iRecved = 0;                         //已经收到多少

signals:

private:
    QListWidget *m_pBookListW;       //显示文件列表
    QPushButton *m_pReturnPB;        //返回
    QPushButton *m_pCreateDirPB;     //创建文件夹
    QPushButton *m_pDelDirPB;        //删除文件夹
    QPushButton *m_pRenamePB;        //重命名文件夹
    QPushButton *m_pFlushFilePB;     //刷新文件夹
    QPushButton *m_pUploadPB;        //上传文件
    QPushButton *m_pDownLoadPB;      //下载文件
    QPushButton *m_pDelFilePB;       //删除文件
    QPushButton *m_pShareFilePB;     //分享文件

    QString m_strEnterDir;           //保存进入的文件夹
    QString m_strUploadFilePath;     //保存上传文件的路径

    QTimer *m_pTimer;                //上传文件定时器

    QString m_strSaveFilePath;       //保存下载文件的路径
    bool m_bDownload;                //是否处于下载文件状态

public slots:
    void createDir();                //创建文件夹
    void flushFile();                //刷新文件
    void updateFileList(const PDU * pdu);         //刷新文件列表
    void delDir();                                //删除文件夹
    void renameFile();                            //重命名文件
    void enterDir(const  QModelIndex &index);     //双击进入文件夹
    void returnPre();                             //返回上级目录
    void delRegFile();                            //删除常规文件
    void uploadFile();                            //上传文件

    void uploadFileData();                        //上传文件数据
    void downloadFile();                          //下载文件



};

#endif // BOOK_H
