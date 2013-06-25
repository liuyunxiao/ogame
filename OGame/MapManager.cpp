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
    g_SceneMgrPtr->setSkyBox(true, "Examples/SpaceSkyBox");
    
	g_SceneMgrPtr->createLight("Light")->setPosition(75,75,75);
    
    MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);
    
    Entity* floor = g_SceneMgrPtr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/Rockwall");
    floor->setCastShadows(false);
    SceneNode* floorNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Vector3(0.0, 0.0, 0.0));
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
    
    
    SceneNode* mBodyNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode();
    Entity* mBodyEnt = g_SceneMgrPtr->createEntity("SinbadBody", "Sinbad.mesh");
    mBodyNode->attachObject(mBodyEnt);
    //mBodyNode->yaw(Radian(Math::PI / 2));

    //mBodyNode->attachObject(g_SceneMgrPtr->createParticleSystem("smoke", "Examples/Smoke"));
    
    Entity* mSword1 = g_SceneMgrPtr->createEntity("SinbadSword1", "Sword.mesh");
    Entity* mSword2 = g_SceneMgrPtr->createEntity("SinbadSword2", "Sword.mesh");
    mBodyEnt->attachObjectToBone("Handle.L", mSword1);
    mBodyEnt->attachObjectToBone("Handle.R", mSword2);
    
    btTransform startTransform;
	startTransform.setIdentity ();
    
	startTransform.setOrigin (btVector3(0.0, 24.0, 0.0));
    
    
	btPairCachingGhostObject* pGhostObject = new btPairCachingGhostObject();
	pGhostObject->setWorldTransform(startTransform);
    
	btScalar characterHeight=5;
	btScalar characterWidth =3;
	btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
	pGhostObject->setCollisionShape (capsule);
	pGhostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
    
	btScalar stepHeight = btScalar(0.35);
    btKinematicCharacterController* pCharactorCtrl = new btKinematicCharacterController (pGhostObject,capsule,stepHeight);
    
    BulletMgr::getSingletonPtr()->GetWord()->addCollisionObject(pGhostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
    
	BulletMgr::getSingletonPtr()->GetWord()->addAction(pCharactorCtrl);
    
    BulletMgr::getSingletonPtr()->GetWord()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(pGhostObject->getBroadphaseHandle(),BulletMgr::getSingletonPtr()->GetWord()->getDispatcher());
    //BulletMgr::getSingletonPtr()->AddRigidBody(pGhostObject);
    
    pCharactorCtrl->reset();
    pCharactorCtrl->warp (btVector3(0.0, 24.0, 0.0));
    pCharactorCtrl->setJumpSpeed(8.0);
    pCharactorCtrl->setFallSpeed(95.0);
    btTransform& form = pGhostObject->getWorldTransform();
    btQuaternion quat;
    //quat.setRotation(btVector3(0.0, 1.0, 0.0), -3.14 / 2.0);
    //form.setRotation(quat);
    //startTransform.getRotation().setRotation(btVector3(0.0, 1.0, 0.0), 3.14 / 2.0);
    
    Player* player = ObjectMgr::getSingletonPtr()->GetPlayer();
    player->Init(mBodyNode, mBodyEnt, pGhostObject);
    player->InitControl(pCharactorCtrl);
    //todo
    player->mAniCtrl = new AnimationCtrl(mBodyEnt);
    //player->mAniCtrl->PlayAni("RunBase", eAPT_Base);
    //player->mAniCtrl->PlayAni("RunTop", eAPT_Top);
    
    

    String sName = "box";
    for (int i=0;i< 10;i++)
    {
        String namenum = name + StringConverter::toString(i);
        Entity* boxEnt = g_SceneMgrPtr->createEntity(namenum, SceneManager::PT_CUBE);
        Ogre::SceneNode* boxNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,10+i*3,0));
    
        boxEnt->setMaterialName("Examples/Box");
        boxNode->attachObject(boxEnt);
        boxNode->scale(Ogre::Vector3(0.01, 0.01, 0.01));
    
        btTransform bodyTransform;
        bodyTransform.setIdentity();
        bodyTransform.setOrigin(btVector3(0,10+i*3,0));
        btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
        btScalar mass(1.);//positive mass means dynamic/moving  object
        bool isDynamic = (mass != 0.f);
        btVector3 localInertia(0,0,0);
   		if (isDynamic)
                boxShape->calculateLocalInertia(mass,localInertia);
    		
        btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
    	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
   		btRigidBody* boxBody=new btRigidBody(rbInfo);
        
        Monster* pMonster = new Monster();
        pMonster->Init(boxNode, boxEnt, boxBody);

        ObjectMgr::getSingletonPtr()->AddMonster(pMonster);
    
        //most applications shouldn't disable deactivation, but for this demo it is better.
        boxBody->setActivationState(DISABLE_DEACTIVATION);
    		//add the body to the dynamics world
        
        BulletMgr::getSingletonPtr()->AddRigidBody(boxBody);
    }
}

void MapMgr::ShoutMonster()
{
    static int shoutnum = 0;
    String name = "sbox";
    
    Vector3 caPos = OgreMgr::getSingletonPtr()->m_pCamera->getPosition();
    caPos.z -= 3;
    
    Vector3 caOr = OgreMgr::getSingletonPtr()->m_pCamera->getDirection();
    
    String namenum = name + StringConverter::toString(++shoutnum);
    Entity* boxEnt = g_SceneMgrPtr->createEntity(namenum, SceneManager::PT_CUBE);
    Ogre::SceneNode* boxNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(caPos);
    
    boxEnt->setMaterialName("Examples/Box");
    boxNode->attachObject(boxEnt);
    boxNode->scale(Ogre::Vector3(0.01, 0.01, 0.01));
    
    btTransform bodyTransform;
    bodyTransform.setIdentity();
    bodyTransform.setOrigin(btVector3(caPos.x,caPos.y,caPos.z));
    btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
    btScalar mass(1.);//positive mass means dynamic/moving  object
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        boxShape->calculateLocalInertia(mass,localInertia);
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
    btRigidBody* boxBody=new btRigidBody(rbInfo);

    
    //most applications shouldn't disable deactivation, but for this demo it is better.
    boxBody->setActivationState(DISABLE_DEACTIVATION);
    //add the body to the dynamics world
    BulletMgr::getSingletonPtr()->AddRigidBody(boxBody);
    //boxBody->setLinearVelocity(btVector3(0, 0, 3));
    
    Monster* pMonster = new Monster();
    pMonster->Init(boxNode, boxEnt, boxBody);
    ObjectMgr::getSingletonPtr()->AddMonster(pMonster);
    
    boxBody->setLinearVelocity(btVector3(caOr.x * 20, caOr.y * 20, caOr.z * 20));
    //boxBody->applyCentralForce(btVector3(caOr.x * 100, caOr.y * 100, caOr.z * 100));
}