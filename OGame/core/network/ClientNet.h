//
//  ClientNet.h
//  RiddleGame
//
//  Created by quwei on 13-4-23.
//
//

#ifndef __RiddleGame__NetSession__
#define __RiddleGame__NetSession__
#include <string>
#include <queue>
#include "type_define.h"

typedef struct _tagMsgData
{
    DWORD   dwSize;
    Byte*   pBuff;
    
} tagMsgData;
using namespace std;
class ClientNet
{
public:
    ClientNet();
    ~ClientNet();
    
    bool Init(bool bNagle=true);
    void Destroy();
    
    int GetSocket() const {return m_Sock;}
    string& GetStrIP() {return m_strIP;}
    int GetPort() const {return m_nPort;}
    ////////////////
    //只用于相关线程调用
    void SetConnected(bool bConnected);
    void SetTerminateConnect(bool bTerminateConnect);
    void SetTerminateSend(bool bTerminateConnect);
    void SetTerminateReceive(bool bTerminateConnect);
    
    bool IsTerminateConnect();
    bool IsTerminateSend();
    bool IsTerminateReceive();
    
    void CreatSendRevsThread();
    LPBYTE GetSendMsg(DWORD& dwMsgSize);
    void AddRevMsg(LPBYTE pData, DWORD dwMsgSize);
    ////////////////
    void TryToConnect(string szIP, int nPort);
    bool IsTryingConnect();
    void StopTryingConnect();
    void Disconnect();
    bool IsConnected();
    
    bool Send(LPVOID pMsg, DWORD dwMsgSize);
    LPBYTE Recv(DWORD& dwMsgSize);
    
private:
    void Clear();
    bool GracefulCloseSocket(int& socket, int nRetryTimes);
private:
    char        m_szHostName[512];
    int         m_Sock;
    string      m_strIP;
    int         m_nPort;
    bool        m_bNagle;
    
    pthread_t					m_hConnect;
    pthread_t					m_hSend;
    pthread_t					m_hReceive;
    
    pthread_mutex_t         m_TerminateConnectMutex;
    volatile bool			m_bTerminateConnect;
    
    pthread_mutex_t         m_TerminateSendMutex;
    volatile bool			m_bTerminateSend;
    
    pthread_mutex_t         m_TerminateReceiveMutex;
    volatile bool			m_bTerminateReceive;
    
    pthread_mutex_t         m_ConnectedMutex;
    volatile bool           m_bConnected;
    
    queue<tagMsgData*>      m_pSendMsgQueue;
    queue<tagMsgData*>      m_pRecvMsgQueue;
    
    pthread_mutex_t         m_SendMutex;
    pthread_mutex_t         m_RecvMutex;
};

#endif /* defined(__RiddleGame__NetSession__) */
