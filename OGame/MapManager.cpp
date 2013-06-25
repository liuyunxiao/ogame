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
#include "Monster.h"
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
    g_pSceneMgrPtr->setSkyBox(true, "Examples/SpaceSkyBox");
	g_pSceneMgrPtr->createLight("Light")->setPosition(75,75,75);
    
    MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);
    
    Entity* floor = g_pSceneMgrPtr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/Rockwall");
    floor->setCastShadows(false);
    SceneNode* floorNode = g_pSceneMgrPtr->getRootSceneNode()->createChildSceneNode(Vector3(0.0, 0.0, 0.0));
    floorNode->attachObject(floor);
    
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0,0,0));
    
    btVector3 norm(0,1,0);
    //btCollisionShape* groundShape = new btStaticPlaneShape(norm,0.0);
    btCollisionShape* groundShape = new btBoxShape(btVector3(50, 1, 50));
    btScalar mass(0.);	//rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        groundShape->calculateLocalInertia(mass,localInertia);
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
    btRigidBody* sFloorPlaneBody = new btRigidBody(rbInfo);
    
    Monster* pFloorMonster = new Monster();
    pFloorMonster->Init(floorNode, floor, sFloorPlaneBody);
    //add the body to the dynamics world
    BulletMgr::getSingletonPtr()->AddRigidBody(sFloorPlaneBody);
    BulletMgr::getSingletonPtr()->AddCollisionShape(groundShape);
}

void MapMgr::ShoutMonster()
{
    static int shoutnum = 0;
    String name = "sbox";
    
    Vector3 caPos = g_pOgreFramePtr->m_pCamera->getPosition();
    caPos.z -= 3;
    Vector3 caOr = g_pOgreFramePtr->m_pCamera->getDirection();
    
    String namenum = name + StringConverter::toString(++shoutnum);
    Entity* boxEnt = g_pSceneMgrPtr->createEntity(namenum, SceneManager::PT_CUBE);
    Ogre::SceneNode* boxNode = g_pSceneMgrPtr->getRootSceneNode()->createChildSceneNode(caPos);
    boxEnt->setMaterialName("Examples/Box");
    boxNode->attachObject(boxEnt);
    boxNode->scale(Ogre::Vector3(0.01, 0.01, 0.01));
    
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin(btVector3(caPos.x,caPos.y,caPos.z));
    btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
    btScalar mass(1.);
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        boxShape->calculateLocalInertia(mass,localInertia);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
    btRigidBody* boxBody=new btRigidBody(rbInfo);
    
    //boxBody->setActivationState(DISABLE_DEACTIVATION);
    g_pBulletMgr->AddRigidBody(boxBody);
    g_pBulletMgr->AddCollisionShape(boxShape);
    boxBody->setLinearVelocity(btVector3(caOr.x * 20, caOr.y * 20, caOr.z * 20));
    
    Monster* pMonster = new Monster();
    pMonster->Init(boxNode, boxEnt, boxBody);
    g_pObjectMgr->AddMonster(pMonster);
}