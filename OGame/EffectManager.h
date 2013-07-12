//
//  EffectManager.h
//  OGame
//
//  Created by mac on 13-6-27.
//
//

#ifndef __OGame__EffectManager__
#define __OGame__EffectManager__

class EffectMgr:public Singleton<EffectMgr>
{
public:
    EffectMgr();
    ~EffectMgr();
    
    
private:
    EffectMgr(const EffectMgr&);
    EffectMgr& operator=(const EffectMgr&);
};

#define g_pEffectMgr EffectMgr::getSingletonPtr()
#endif /* defined(__OGame__EffectManager__) */
