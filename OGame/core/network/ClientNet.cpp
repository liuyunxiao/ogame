//
//  ClientNet.cpp
//  RiddleGame
//
//  Created by quwei on 13-4-23.
//
//

#include "ClientNet.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

#include "type_define.h"
#include "net_define.h"
#include "filter_define.h"
#include "MINILZO.H"


static void* ThreadConnect(void* data)
{
    ClientNet* pNet = (ClientNet*)data;
    sockaddr_in server_addr;
    server_addr.sin_len = sizeof(struct sockaddr_in);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(pNet->GetPort());
    server_addr.sin_addr.s_addr = inet_addr(pNet->GetStrIP().c_str());
    
    bzero(&(server_addr.sin_zero), 8);
    
    fd_set fdWrite;
    timeval block_time;
    block_time.tv_sec = 0;
    block_time.tv_usec = GT_NET_BLOCK_TIME;
    
    int conn = connect(pNet->GetSocket(), (struct sockaddr*)&server_addr, sizeof(sockaddr_in));
    if(SOCKET_ERROR == conn)
    {
        printf("errno is %ld\n", errno);
        //if(EWOULDBLOCK == errno)
        if(EINPROGRESS == errno)
        {
            for(int n = 0; n < 64; ++n)
            {
                FD_ZERO(&fdWrite);
                FD_SET(pNet->GetSocket(), &fdWrite);
            
                if(1 == select(pNet->GetSocket() + 1, NULL, &fdWrite, NULL, &block_time))
                    goto __connected;
            
                if(pNet->IsTerminateConnect())
                    goto __connected_terminated;
            }
        }
        
        goto __connected_terminated;
    }
    
__connected:
    pNet->CreatSendRevsThread();
    pNet->SetConnected(true);
    pthread_exit(NULL);
    return NULL;
    
__connected_terminated:
    pthread_exit(NULL);
    return NULL;
}


static void* ThreadSend(void* data)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    ClientNet* pNet = (ClientNet*)data;
    
    fd_set fdwrite;
    timeval block_time;
    block_time.tv_sec = 0;
    block_time.tv_usec = GT_NET_BLOCK_TIME;
    
    LPBYTE pMsg = NULL, pTempPtr = NULL;
    DWORD dwMsgSize = 0;
    int nReturn = 0;
    
    while (!pNet->IsTerminateSend())
    {
        for(;;)
        {
            pTempPtr = pMsg = pNet->GetSendMsg(dwMsgSize);
        
            if(NULL == pMsg)
                break;
            
            msgBase* pp = (msgBase*)(pMsg + sizeof(DWORD));
            while( dwMsgSize > 0 && !pNet->IsTerminateSend())
            {
                nReturn = send(pNet->GetSocket(), (char*)pTempPtr, dwMsgSize, 0);
                if(SOCKET_ERROR == nReturn)
                {
                    switch (errno)
                    {
                    case EWOULDBLOCK:
                    case ENOBUFS:
                        {
                            FD_ZERO(&fdwrite);
                            FD_SET(pNet->GetSocket(), &fdwrite);
                            select(pNet->GetSocket() + 1, NULL, &fdwrite, NULL, &block_time);
                        }
                        break;
                            
                    default:
                            shutdown(pNet->GetSocket(), 1);
                            free(pMsg);
                            goto __thread_send_end;
                            break;
                    }
                    free(pMsg);
                    shutdown(pNet->GetSocket(), 1);
                    goto __thread_send_end;
                }
                else
                {
                    pTempPtr += nReturn;
                    dwMsgSize -= nReturn;
                }
            }
            
            free(pMsg);
            pTempPtr = pMsg = NULL;
        }
        
    }
__thread_send_end:
    pNet->SetConnected(false);
    pthread_exit(NULL);
    return NULL;
}

static void* ThreadReceive(void* data)
{
    ClientNet* pNet = (ClientNet*)data;
    
    fd_set fdread;
    timeval block_time;
    block_time.tv_sec = 0;
    block_time.tv_usec = GT_NET_BLOCK_TIME;
    
    int nResult = 0;
    DWORD dwMsgSize = 0;
    DWORD dwTempSize = 0;
    char* pTempPtr = NULL;
    
    while (!pNet->IsTerminateReceive()) {
        dwTempSize = sizeof(DWORD);
        pTempPtr = (char*)&dwMsgSize;
        
        while (dwTempSize > 0)
        {
            FD_ZERO(&fdread);
            FD_SET(pNet->GetSocket(), &fdread);
            if(1 == select(pNet->GetSocket() + 1, &fdread, NULL, NULL, &block_time))
            {
                nResult = recv(pNet->GetSocket(), pTempPtr, dwTempSize, 0);
                if( 0 == nResult)
                    goto __thread_receive_end;
                
                if( SOCKET_ERROR == nResult)
                {
                    shutdown(pNet->GetSocket(), 1);
                    goto __thread_receive_end;
                }
                else{
                    dwTempSize -= nResult;
                    pTempPtr += nResult;
                }
                
                if(pNet->IsTerminateReceive())
                    goto __thread_receive_end;
                
            }
        }
        
        if( SOCKET_ERROR != nResult)
        {
            char* pBufferIn = (char*)malloc(dwMsgSize+dwMsgSize/64+64);
			dwTempSize = sizeof(DWORD) + dwMsgSize;
			pTempPtr = pBufferIn;
			while( dwTempSize > 0 )
			{
				FD_ZERO(&fdread);
				FD_SET(pNet->GetSocket(), &fdread);
				if( 1 == select(pNet->GetSocket() + 1, &fdread, NULL, NULL, &block_time) )
				{
					nResult = recv(pNet->GetSocket(), pTempPtr, dwTempSize, 0);
					if( 0 == nResult )	// FIN
					{
						free(pBufferIn);
						goto __thread_receive_end;
					}
                    
					if( nResult == SOCKET_ERROR )
					{
						shutdown(pNet->GetSocket(), 1);
						free(pBufferIn);
						goto __thread_receive_end;
					}
					else
					{
						dwTempSize -= nResult;
						pTempPtr += nResult;
					}
					
					if( pNet->IsTerminateReceive() )
					{
						free(pBufferIn);
						goto __thread_receive_end;
					}
				}
			}
            
			char* pBufferOut = (char*)malloc(*(DWORD*)pBufferIn);
            
            
			tagFilterParam param;
			param.bCompress = false;
			param.bSuccess = false;
			param.dwInBufferSize = dwMsgSize+dwMsgSize/64+64;
			param.dwInDataSize = dwMsgSize;
			param.dwOutBufferSize = *(DWORD*)pBufferIn;
			param.dwOutDataSize = 0;
			param.eFilter = FMF_MiniLzo;
			param.lpMemIn = pBufferIn + sizeof(DWORD);
			param.lpMemOut = pBufferOut;
            
		
			//m_pFilter->Filter(&param);
            int nResult = lzo1x_decompress((lzo_byte*)param.lpMemIn, param.dwInDataSize, (lzo_byte*)param.lpMemOut, (lzo_uint*)(&param.dwOutDataSize), NULL);
            
            if( nResult == LZO_E_OK)
            {
                param.bSuccess = true;
                pNet->AddRevMsg((LPBYTE)pBufferOut, param.dwOutDataSize);
            }
            else
            {
                free(pBufferOut);
                param.bSuccess = false;
            }
            
            //test
            //msgNLC_Proof* pproof = (msgNLC_Proof*)pBufferOut;
            
			//m_pRecvMsgQueue->AddMsg(pBufferOut, param.dwOutDataSize);
           
			free(pBufferIn);
			//free(pBufferOut);
        }
    }
    
__thread_receive_end:
    pNet->SetConnected(false);
    pthread_exit(NULL);
    return NULL;
}


ClientNet::ClientNet()
{
    Clear();
}

ClientNet::~ClientNet()
{
    this->Destroy();
}


bool ClientNet::Init(bool bNagle)
{
    pthread_mutex_init(&m_TerminateConnectMutex, NULL);
    pthread_mutex_init(&m_TerminateSendMutex, NULL);
    pthread_mutex_init(&m_TerminateReceiveMutex, NULL);
    pthread_mutex_init(&m_SendMutex, NULL);
    pthread_mutex_init(&m_RecvMutex, NULL);
    pthread_mutex_init(&m_ConnectedMutex, NULL);

    
    this->Clear();
    
    m_bNagle = bNagle;
    m_Sock = socket(AF_INET, SOCK_STREAM, 0);
    if(SOCKET_ERROR == m_Sock)
    {
        printf("socket error");
        close(m_Sock);
        return false;
    }
    
    int nRet = fcntl(m_Sock, F_GETFL, 0);
    if(SOCKET_ERROR == fcntl(m_Sock, F_SETFL, nRet | O_NONBLOCK))
    {
        printf("socket nonblock error");
        close(m_Sock);
        m_Sock = 0;
        return false;
    }
    
    if( !m_bNagle)
    {
        int bNoDelay = 1;
        setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
    }
    
    DWORD dwReUseAddr = true;
    setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&dwReUseAddr, sizeof(dwReUseAddr));
    
    linger lin;
    lin.l_linger = 0;
    lin.l_onoff = 1;
    setsockopt(m_Sock, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));
    
        
    return true;
}

void ClientNet::Destroy()
{
    SetTerminateConnect(true);
    SetTerminateSend(true);
    SetTerminateReceive(true);
    
    pthread_join(m_hConnect, NULL);
    pthread_join(m_hSend, NULL);
    pthread_join(m_hReceive, NULL);
    
    if(IsConnected())
    {
        GracefulCloseSocket(m_Sock, 20);
    }
    else
    {
        shutdown(m_Sock, 1);
        close(m_Sock);
        m_Sock = 0;
    }
    
    SetConnected(false);
    
    while (!m_pSendMsgQueue.empty()) {
        tagMsgData* pMsg = m_pSendMsgQueue.front();
        free(pMsg->pBuff);
        delete pMsg;
        
        m_pSendMsgQueue.pop();
    }
    
    while (!m_pRecvMsgQueue.empty()) {
        tagMsgData* pMsg = m_pRecvMsgQueue.front();
        free(pMsg->pBuff);
        delete pMsg;
        
        m_pRecvMsgQueue.pop();
    }
    
    pthread_mutex_destroy(&m_ConnectedMutex);
    pthread_mutex_destroy(&m_TerminateConnectMutex);
    pthread_mutex_destroy(&m_TerminateReceiveMutex);
    pthread_mutex_destroy(&m_TerminateSendMutex);
}

void ClientNet::Clear()
{
    
    
    m_strIP.clear();
    m_Sock = 0;
    m_bNagle = true;
    m_nPort = 0;
    
    SetConnected(false);
    SetTerminateConnect(false);
    SetTerminateSend(false);
    SetTerminateReceive(false);
    
    memset(m_szHostName, 0, sizeof(m_szHostName));
    //bzero(m_szHostName, sizeof(m_szHostName));
}

bool ClientNet::GracefulCloseSocket(int& socket, int nRetryTimes)
{
    if(0 == socket)
        return  true;
    
    shutdown(socket, 1);
    
    int nRetry = 0;
    fd_set fdread;
    timeval block_time;
    block_time.tv_sec = 0;
    block_time.tv_usec = GT_NET_BLOCK_TIME;
    
    while (nRetry++ < nRetryTimes) {
        FD_ZERO(&fdread);
        FD_SET(socket, &fdread);
        
        if(1 == select(m_Sock + 1, &fdread, NULL, NULL, &block_time))
        {
            char chTemp;
            int nResult = recv(socket, &chTemp, 1, 0);
            if( 0 == nResult)
            {
                close(socket);
                socket = 0;
                return  true;
            }
            
            if(SOCKET_ERROR == nResult)
                break;
        }
    }
    
    close(socket);
    socket = 0;
    return false;
}

void ClientNet::TryToConnect(string szIP, int nPort)
{
    this->Disconnect();
    
    m_strIP = szIP;
    m_nPort = nPort;
    
    pthread_create(&m_hConnect, NULL, ThreadConnect, (void*)this);
}

void ClientNet::CreatSendRevsThread()
{
    pthread_create(&m_hSend, NULL, ThreadSend, (void*)this);
    pthread_create(&m_hReceive, NULL, ThreadReceive, (void*)this);
}

bool ClientNet::IsTryingConnect()
{
    int kill_rc = pthread_kill(m_hConnect, 0);
    if(kill_rc == ESRCH)
        return false;
    
    return true;
}

void ClientNet::StopTryingConnect()
{
    this->Destroy();
    this->Init(m_bNagle);
}

void ClientNet::Disconnect()
{
    this->StopTryingConnect();
}

void ClientNet::SetConnected(bool bConnected)
{
    pthread_mutex_lock(&m_ConnectedMutex);
    m_bConnected = bConnected;
    pthread_mutex_unlock(&m_ConnectedMutex);
}

void ClientNet::SetTerminateConnect(bool bTerminateConnect)
{
    pthread_mutex_lock(&m_TerminateConnectMutex);
    m_bTerminateConnect = bTerminateConnect;
    pthread_mutex_unlock(&m_TerminateConnectMutex);
}

void ClientNet::SetTerminateSend(bool bTerminateSend)
{
    pthread_mutex_lock(&m_TerminateSendMutex);
    m_bTerminateSend = bTerminateSend;
    pthread_mutex_unlock(&m_TerminateSendMutex);
}

void ClientNet::SetTerminateReceive(bool bTerminateReceive)
{
    pthread_mutex_lock(&m_TerminateReceiveMutex);
    m_bTerminateReceive = bTerminateReceive;
    pthread_mutex_unlock(&m_TerminateReceiveMutex);
}

bool ClientNet::IsTerminateConnect()
{
    bool bFlag =false;
    pthread_mutex_lock(&m_TerminateConnectMutex);
    bFlag = m_bTerminateConnect;
    pthread_mutex_unlock(&m_TerminateConnectMutex);
    return  bFlag;
}

bool ClientNet::IsTerminateSend()
{
    bool bFlag =false;
    pthread_mutex_lock(&m_TerminateSendMutex);
    bFlag = m_bTerminateSend;
    pthread_mutex_unlock(&m_TerminateSendMutex);
    return  bFlag;
}

bool ClientNet::IsTerminateReceive()
{
    bool bFlag =false;
    pthread_mutex_lock(&m_TerminateReceiveMutex);
    bFlag = m_bTerminateReceive;
    pthread_mutex_unlock(&m_TerminateReceiveMutex);
    return  bFlag;
}

LPBYTE ClientNet::GetSendMsg(DWORD& dwMsgSize)
{
    dwMsgSize = 0;
    LPBYTE pReturn = NULL;
    pthread_mutex_lock(&m_SendMutex);
    if(m_pSendMsgQueue.empty())
    {
        pthread_mutex_unlock(&m_SendMutex);
        return NULL;
    }
    
    tagMsgData* pMsg = m_pSendMsgQueue.front();
    dwMsgSize = pMsg->dwSize;
    pReturn = pMsg->pBuff;
    delete  pMsg;
    m_pSendMsgQueue.pop();
    pthread_mutex_unlock(&m_SendMutex);
    return pReturn;
}

void ClientNet::AddRevMsg(LPBYTE pData, DWORD dwMsgSize)
{
    tagMsgData* pTagMsg = new tagMsgData;
    pTagMsg->pBuff = pData;
    pTagMsg->dwSize = dwMsgSize;
    
    pthread_mutex_lock(&m_RecvMutex);
    m_pRecvMsgQueue.push(pTagMsg);
    pthread_mutex_unlock(&m_RecvMutex);
}

bool ClientNet::IsConnected()
{
    bool bConnect =false;
    pthread_mutex_lock(&m_ConnectedMutex);
    bConnect = m_bConnected;
    pthread_mutex_unlock(&m_ConnectedMutex);
    return  bConnect;
}


bool ClientNet::Send(LPVOID pMsg, DWORD dwMsgSize)
{
    if(NULL == pMsg)
        return true;
    
    if(false == IsConnected())
        return  false;
    
    tagMsgData* pTagMsg = new tagMsgData;
    
    //发送出去数据的真实大小 ＝ msgbase ＋ 1个DWORD大小
    DWORD dwBuffSize = dwMsgSize + sizeof(DWORD);
    pTagMsg->dwSize = dwBuffSize;
    
    Byte* pBuff = (Byte*)malloc(dwBuffSize);//new Byte[dwBuffSize];
    
    //在数据头加上msgbase大小
    *(DWORD*)pBuff = dwMsgSize;
    memcpy(pBuff + sizeof(DWORD), pMsg, dwMsgSize);
    pTagMsg->pBuff = pBuff;
    
    pthread_mutex_lock(&m_SendMutex);
    m_pSendMsgQueue.push(pTagMsg);
    pthread_mutex_unlock(&m_SendMutex);
    return true;
}

LPBYTE ClientNet::Recv(DWORD& dwMsgSize)
{
    dwMsgSize = 0;
    LPBYTE pReturn = NULL;
    pthread_mutex_lock(&m_RecvMutex);
    if(m_pRecvMsgQueue.empty())
    {
        pthread_mutex_unlock(&m_RecvMutex);
        return NULL;
    }
    
    tagMsgData* pMsg = m_pRecvMsgQueue.front();
    dwMsgSize = pMsg->dwSize;
    pReturn = pMsg->pBuff;
    delete  pMsg;
    m_pRecvMsgQueue.pop();
    pthread_mutex_unlock(&m_RecvMutex);
    return pReturn;
}
