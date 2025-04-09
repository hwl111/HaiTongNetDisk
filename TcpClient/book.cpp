#include "book.h"
#include"tcpclient.h"
#include<QInputDialog>
#include<QMessageBox>
Book::Book(QWidget *parent)
    : QWidget{parent}
{
    m_strEnterDir.clear();             //清空保存进入的文件夹

    m_pBookListW = new QListWidget;
    m_pReturnPB = new QPushButton("返回");
    m_pCreateDirPB = new QPushButton("创建文件夹");
    m_pDelDirPB = new QPushButton("删除文件夹");
    m_pRenamePB = new QPushButton("重命名文件");
    m_pFlushFilePB = new QPushButton("刷新文件");

    QVBoxLayout *pDirVBL = new QVBoxLayout;   //垂直布局
    pDirVBL->addWidget(m_pReturnPB);
    pDirVBL->addWidget(m_pCreateDirPB);
    pDirVBL->addWidget(m_pDelDirPB);
    pDirVBL->addWidget(m_pRenamePB);
    pDirVBL->addWidget(m_pFlushFilePB);

    m_pUploadPB = new QPushButton("上传文件");
    m_pDownLoadPB = new QPushButton("下载文件");
    m_pDelFilePB = new QPushButton("删除文件");
    m_pShareFilePB = new QPushButton("共享文件");

    QVBoxLayout *pFileVBL = new QVBoxLayout;
    pFileVBL->addWidget(m_pUploadPB);
    pFileVBL->addWidget(m_pDownLoadPB);
    pFileVBL->addWidget(m_pDelFilePB);
    pFileVBL->addWidget(m_pShareFilePB);

    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pBookListW);
    pMain->addLayout(pDirVBL);
    pMain->addLayout(pFileVBL);

    setLayout(pMain);

    connect(m_pCreateDirPB, SIGNAL(clicked(bool))
            , this, SLOT(createDir()));
    connect(m_pFlushFilePB, SIGNAL(clicked(bool))
            , this, SLOT(flushFile()));
    connect(m_pDelDirPB, SIGNAL(clicked(bool))
            , this, SLOT(delDir()));
    connect(m_pRenamePB, SIGNAL(clicked(bool))
            , this, SLOT(renameFile()));
    connect(m_pBookListW, SIGNAL(doubleClicked(QModelIndex))
            ,this, SLOT(enterDir(QModelIndex)));
}

void Book::createDir()   //创建文件夹
{
    QString strNewDir = QInputDialog::getText(this, "新建文件夹", "新文件夹名字");
    if(!strNewDir.isEmpty())
    {
        if(strNewDir.size()>32)
        {
            QMessageBox::warning(this, "新建文件夹", "文件夹名字不能超过32个字符");
        }
        else
        {
            QString strName = TcpCLient::getInstance().loginName();   //获得登录用户名
            QString strCurPath = TcpCLient::getInstance().curPath();  //获得当前路径
            PDU *pdu = mkPDU(strCurPath.size()+1);
            pdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
            strncpy(pdu->caData, strName.toStdString().c_str(), strName.size());           //拷贝用户名
            strncpy(pdu->caData+32, strNewDir.toStdString().c_str(), strNewDir.size());  //拷贝文件路径
            memcpy(pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());

            TcpCLient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);      //发送给服务器
            free(pdu);
            pdu = NULL;
        }
    }
    else
    {
        QMessageBox::warning(this,"新建文件夹", "文件夹名字不能为空");
    }
}

void Book::flushFile()
{
    QString strCurPath = TcpCLient::getInstance().curPath();  //获得当前路径
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    strncpy((char*)pdu->caMsg,strCurPath.toStdString().c_str(), strCurPath.size());
    TcpCLient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);      //发送给服务器
    free(pdu);
    pdu = NULL;
}

void Book::updateFileList(const PDU *pdu)
{
    if(pdu == NULL)
    {
        return;
    }

    m_pBookListW->clear();  //防止重复显示

    FileInfo *pFileInfo = NULL;
    int iCount = pdu->uiMSgLen/sizeof(FileInfo);
    for(int i=0;i<iCount;i++)
    {
        pFileInfo = (FileInfo*)(pdu->caMsg) + i;
        //if(pFileInfo->caFileName != QString(".") && pFileInfo->caFileName != QString(".."))
        QListWidgetItem *pItem = new QListWidgetItem;
        if(pFileInfo->iFileType == 0)
        {
            pItem->setIcon(QIcon(QPixmap(":/map/dir.jpg")));  //设置图标
        }
        else if(pFileInfo->iFileType == 1)
        {
            pItem->setIcon(QIcon(QPixmap(":/map/reg.jpg")));
        }
        pItem->setText(pFileInfo->caFileName);   //设置名字
        m_pBookListW->addItem(pItem);
    }
}

void Book::delDir()
{
    QString strCurPath = TcpCLient::getInstance().curPath();  //获得当前路径
    QListWidgetItem *pItem =  m_pBookListW->currentItem();
    if(pItem == NULL)
    {
        QMessageBox::warning(this, "删除文件","请选择要删除的文件");
    }
    else
    {
        QString strDelName = pItem->text();   //获得文件名
        PDU *pdu = mkPDU(strCurPath.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_REQUEST;
        strncpy(pdu->caData, strDelName.toStdString().c_str(), strDelName.size());   //拷贝要删除的文件夹名
        memcpy((char*)pdu->caMsg,strCurPath.toStdString().c_str(), strCurPath.size()); //拷贝路径
        TcpCLient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);      //发送给服务器
        free(pdu);
        pdu = NULL;
    }
}

void Book::renameFile()
{
    QString strCurPath = TcpCLient::getInstance().curPath();  //获得当前路径
    QListWidgetItem *pItem =  m_pBookListW->currentItem();
    if(pItem == NULL)
    {
        QMessageBox::warning(this, "重命名文件","请选择要重命名的文件");
    }
    else
    {
        QString strOldName = pItem->text();   //获得旧的文件名
        QString strNewName = QInputDialog::getText(this, "重命名文件", "请输入新的文件名");
        if(!strNewName.isEmpty())
        {
            PDU *pdu = mkPDU(strCurPath.size()+1);
            pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
            strncpy(pdu->caData, strOldName.toStdString().c_str(), strOldName.size());  //拷贝旧文件名
            strncpy(pdu->caData+32, strNewName.toStdString().c_str(), strNewName.size());  //拷贝新文件名
            memcpy((char*)pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size()); //拷贝路径
            TcpCLient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);      //发送给服务器
            free(pdu);
            pdu = NULL;
        }
        else
        {
            QMessageBox::warning(this,"重命名文件", "新文件名不能为空");
        }
    }
}

void Book::enterDir(const QModelIndex &index)
{
    QString strDirName = index.data().toString();  //获得双击的文件名
    //qDebug()<<strDirName;
    m_strEnterDir = strDirName;
    QString strCurPath = TcpCLient::getInstance().curPath();  //当前所在目录
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_ENTER_DIR_REQUEST;
    strncpy(pdu->caData, strDirName.toStdString().c_str(), strDirName.size());
    memcpy(pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());

    TcpCLient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);      //发送给服务器
    free(pdu);
    pdu = NULL;
}

void Book::clearEnterDir()
{
    m_strEnterDir.clear();
}

QString Book::enterDir()
{
    return m_strEnterDir;
}
