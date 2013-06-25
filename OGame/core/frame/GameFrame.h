//
//  GameFrame.h
//  RiddleGame
//
//  Created by quwei on 13-4-27.
//
//

#ifndef __RiddleGame__GameFrame__
#define __RiddleGame__GameFrame__

#include "cocos2d.h"
#include "../define/type_define.h"
#include "../define/net_define.h"
USING_NS_CC;
typedef DWORD (CCObject::* NETFUN)(msgBase* pMsg);

typedef struct _NetCmdHandler
{
    CCObject*       pObj;
    NETFUN          func;
}NetCmdHandler;

class GameFrameMgr
{
    typedef std::list<NetCmdHandler*> NetCmdList;
    typedef NetCmdList::iterator NetCmdListItor;
    typedef std::map<DWORD, NetCmdList*> NetCmdMap;
    typedef NetCmdMap::iterator NetCmdMapItor;
public:
    ~GameFrameMgr();
    
    void HandleNetMessage(msgBase* netMsg);
    
    void RegisterNetCmd(DWORD dwCmdID, NETFUN func, CCObject* pObj);
    void UnRegisterObjNetCmd(CCObject* pObj);
    void UnRegisterAllNetCmd();
private:
    NetCmdMap m_mapNetCmd;
};

extern GameFrameMgr g_GameFrameMgr;

class GameFrame: public CCLayer
{
public:
    ~GameFrame();
protected:
    void RegisterNetCmd(DWORD dwCmdID, NETFUN fun);
    void UnRegisterNetCmd();
};

#endif /* defined(__RiddleGame__GameFrame__) */
