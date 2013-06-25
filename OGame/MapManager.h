//
//  MapManager.h
//  OGame
//
//  Created by mac on 13-6-21.
//
//

#ifndef __OGame__MapManager__
#define __OGame__MapManager__

class MapMgr:public Singleton<MapMgr>
{
public:
    MapMgr();
    ~MapMgr();
    
    bool Init();
    void Update(double delta);
    
    bool EnterMap(String name);
    
    void ShoutMonster();
private:
    MapMgr(const MapMgr&);
    MapMgr& operator= (const MapMgr&);
};
#define g_pMapMgr MapMgr::getSingletonPtr()
#endif /* defined(__OGame__MapManager__) */
