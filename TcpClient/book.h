#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include<QListWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"protocol.h"

class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);

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

public slots:
    void createDir();                //创建文件夹
    void flushFile();                //刷新文件
    void updateFileList(const PDU * pdu);         //刷新文件列表


};

#endif // BOOK_H
