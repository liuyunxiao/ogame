//
//  TerrainManager.cpp
//  OGame
//
//  Created by mac on 13-6-26.
//
//

#include "TerrainManager.h"

#ifdef USING_OGRE_TERRAIN
#include "OgreManager.h"

#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0

#define TERRAIN_FILE_PREFIX String("testTerrain")
#define TERRAIN_FILE_SUFFIX String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

template<> TerrainMgr* Singleton<TerrainMgr>::msSingleton = 0;

TerrainMgr::TerrainMgr():mTerrainsImported(false),mTerrainPos(0,0,0)
{
    
}

TerrainMgr::~TerrainMgr()
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

bool TerrainMgr::Init()
{
    mTerrainGlobals = OGRE_NEW TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW TerrainGroup(g_pSceneMgrPtr, Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
    mTerrainGroup->setOrigin(mTerrainPos);
    
    Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
    Light* l = g_pSceneMgrPtr->createLight("tstLight");
    l->setType(Light::LT_DIRECTIONAL);
    l->setDirection(lightdir);
    l->setDiffuseColour(ColourValue::White);
    l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
    
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);
    //mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
    //mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
    //mTerrainGlobals->setLightMapSize(256);
    
    //matProfile->setLightmapEnabled(false);
    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(l->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(g_pSceneMgrPtr->getAmbientLight());
    //mTerrainGlobals->setCompositeMapAmbient(ColourValue::Red);
    mTerrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());
    
    // Configure default import settings for if we use imported image
    Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = TERRAIN_SIZE;
    defaultimp.worldSize = TERRAIN_WORLD_SIZE;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
    
    bool blankTerrain = false;
    for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
        for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
            defineTerrain(x, y, blankTerrain);
    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);
    
    if(mTerrainsImported)
    {
        TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    mTerrainGroup->freeTemporaryResources();
    return true;
}

void TerrainMgr::initBlendMaps(Terrain* terrain)
{
    TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Real minHeight0 = 70;
    Real fadeDist0 = 40;
    Real minHeight1 = 70;
    Real fadeDist1 = 15;
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Real tx, ty;
            
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Real val = (height - minHeight0) / fadeDist0;
            val = Math::Clamp(val, (Real)0, (Real)1);
            //*pBlend0++ = val;
            
            val = (height - minHeight1) / fadeDist1;
            val = Math::Clamp(val, (Real)0, (Real)1);
            *pBlend1++ = val;
            
            
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    //blendMap0->loadImage("blendmap1.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    blendMap0->update();
    blendMap1->update();
    
    // set up a colour map
    /*
     if (!terrain->getGlobalColourMapEnabled())
     {
     terrain->setGlobalColourMapEnabled(true);
     Image colourMap;
     colourMap.load("testcolourmap.jpg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
     terrain->getGlobalColourMap()->loadImage(colourMap);
     }
     */
    
}

void TerrainMgr::defineTerrain(long x, long y, bool flat)
{
    // if a file is available, use it
    // if not, generate file from import
    
    // Usually in a real project you'll know whether the compact terrain data is
    // available or not; I'm doing it this way to save distribution size
    
    if (flat)
    {
        mTerrainGroup->defineTerrain(x, y, 0.0f);
    }
    else
    {
        String filename = mTerrainGroup->generateFilename(x, y);
        if (ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
        {
            mTerrainGroup->defineTerrain(x, y);
        }
        else
        {
            Image img;
            getTerrainImage(x % 2 != 0, y % 2 != 0, img);
            mTerrainGroup->defineTerrain(x, y, &img);
            mTerrainsImported = true;
        }
        
    }
}

void TerrainMgr::getTerrainImage(bool flipX, bool flipY, Image& img)
{
    img.load("terrain.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
    
}

#endif