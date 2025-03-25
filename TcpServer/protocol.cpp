#include"protocol.h"


PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen;  //总的大小
    PDU *pdu = (PDU*)malloc(uiPDULen);
    if(NULL == pdu)
    {
        exit(EXIT_FAILURE);                 //申请失败结束程序
    }
    memset(pdu, 0, uiPDULen);                 //清空
    pdu->uiPDULen = uiPDULen;
    pdu->uiMSgLen = uiMsgLen;
    return pdu;
}
