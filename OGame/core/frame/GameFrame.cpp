//
//  GameFrame.cpp
//  RiddleGame
//
//  Created by quwei on 13-4-27.
//
//

#include "GameFrame.h"


GameFrameMgr g_GameFrameMgr;

GameFrameMgr::~GameFrameMgr()
{
    UnRegisterAllNetCmd();
}
void GameFrameMgr::RegisterNetCmd(DWORD dwCmdID, NETFUN func, cocos2d::CCObject *pObj)
{
    if(NULL == func || NULL == pObj)
        return;
    
    NetCmdList* pList = NULL;
    NetCmdMapItor itor = m_mapNetCmd.find(dwCmdID);
    
    if (itor == m_mapNetCmd.end())
    {
        pList = new NetCmdList;
        m_mapNetCmd.insert(std::make_pair(dwCmdID, pList));
    }
    
    if(NULL == pList)
        return;
    
    NetCmdHandler* pHandler = new NetCmdHandler;
    pHandler->pObj = pObj;
    pHandler->func = func;
    pList->push_back(pHandler);
}

void GameFrameMgr::UnRegisterObjNetCmd(cocos2d::CCObject *pObj)
{
    NetCmdMapItor mapItor = m_mapNetCmd.begin();
    for(; mapItor != m_mapNetCmd.end(); ++mapItor)
    {
        NetCmdList* pList = mapItor->second;
        NetCmdListItor listItor = pList->begin();
        for (; listItor != pList->end();)
        {
            NetCmdHandler* pHandler = *listItor;
            if(pHandler->pObj == pObj)
            {
                delete pHandler;
                listItor = pList->erase(listItor);
            }
            else
                ++listItor;
        }
    }
}

void GameFrameMgr::UnRegisterAllNetCmd()
{
    NetCmdMapItor mapItor = m_mapNetCmd.begin();
    for(; mapItor != m_mapNetCmd.end(); ++mapItor)
    {
        NetCmdList* pList = mapItor->second;
        NetCmdListItor listItor = pList->begin();
        for (; listItor != pList->end(); ++listItor)
        {
            NetCmdHandler* pHandler = *listItor;
            delete pHandler;
        }
        pList->clear();
        delete pList;
    }
    
    m_mapNetCmd.clear();
}

void GameFrameMgr::HandleNetMessage(msgBase *netMsg)
{
    if(NULL == netMsg)
        return;
    
    NetCmdMapItor mapItor = m_mapNetCmd.find(netMsg->dwID);
    if(mapItor != m_mapNetCmd.end())
    {
        NetCmdList* pList = mapItor->second;
        NetCmdListItor listItor = pList->begin();
        for (; listItor != pList->end(); ++listItor)
        {
            NetCmdHandler* pHandler = *listItor;
            (pHandler->pObj->*(pHandler->func))(netMsg);
        }

    }
}


GameFrame::~GameFrame()
{
    UnRegisterNetCmd();
}

void GameFrame::RegisterNetCmd(DWORD dwCmdID, NETFUN fun)
{
    g_GameFrameMgr.RegisterNetCmd(dwCmdID, fun, this);
}

void GameFrame::UnRegisterNetCmd()
{
    g_GameFrameMgr.UnRegisterObjNetCmd(this);
}