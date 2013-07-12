//
//  TerrainManager.h
//  OGame
//
//  Created by mac on 13-6-26.
//
//

#ifndef __OGame__TerrainManager__
#define __OGame__TerrainManager__
#ifdef USING_OGRE_TERRAIN
class TerrainMgr :public Singleton<TerrainMgr>
{
public:
    TerrainMgr();
    ~TerrainMgr();
    
    bool Init();
    
    void defineTerrain(long x, long y, bool flat = false);
    void getTerrainImage(bool flipX, bool flipY, Image& img);
    void initBlendMaps(Terrain* terrain);
private:
    TerrainMgr(const TerrainMgr&);
	TerrainMgr& operator= (const TerrainMgr&);
    
    TerrainGlobalOptions*   mTerrainGlobals;
	TerrainGroup*           mTerrainGroup;
    Vector3                 mTerrainPos;
    bool                    mTerrainsImported;
};
#define g_pTerrainMgr TerrainMgr::getSingletonPtr()
#endif
#endif /* defined(__OGame__TerrainManager__) */
