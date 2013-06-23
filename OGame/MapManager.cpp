//
//  MapManager.cpp
//  OGame
//
//  Created by mac on 13-6-21.
//
//

#include "MapManager.h"

#include "OgreManager.h"
#include "BulletManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "AnimationCtrl.h"

template<> MapMgr* Singleton<MapMgr>::msSingleton = 0;
MapMgr::MapMgr()
{

}

MapMgr::~MapMgr()
{
    
}

bool MapMgr::Init()
{
    return true;
}

void MapMgr::Update(double delta)
{
    
}

bool MapMgr::EnterMap(String name)
{
    g_SceneMgrPtr->setSkyBox(true, "Examples/SpaceSkyBox");
    
	g_SceneMgrPtr->createLight("Light")->setPosition(75,75,75);
    
    MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);
    
    Entity* floor = g_SceneMgrPtr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/Rockwall");
    floor->setCastShadows(false);
    SceneNode* floorNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Vector3(0.0, 0.0, 0.0));
    floorNode->attachObject(floor);
    
    SceneNode* mBodyNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * 5);
    Entity* mBodyEnt = g_SceneMgrPtr->createEntity("SinbadBody", "Sinbad.mesh");
    mBodyNode->attachObject(mBodyEnt);

    //mBodyNode->attachObject(g_SceneMgrPtr->createParticleSystem("smoke", "Examples/Smoke"));
    
    Entity* mSword1 = g_SceneMgrPtr->createEntity("SinbadSword1", "Sword.mesh");
    Entity* mSword2 = g_SceneMgrPtr->createEntity("SinbadSword2", "Sword.mesh");
    mBodyEnt->attachObjectToBone("Handle.L", mSword1);
    mBodyEnt->attachObjectToBone("Handle.R", mSword2);
    
    Player* player = ObjectMgr::getSingletonPtr()->GetPlayer();
    player->Init(mBodyNode, mBodyEnt);
    
    //todo
    player->mAniCtrl = new AnimationCtrl(mBodyEnt);
    player->mAniCtrl->PlayAni("RunBase", eAPT_Base);
    player->mAniCtrl->PlayAni("RunTop", eAPT_Top);
}