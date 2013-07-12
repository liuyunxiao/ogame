//
//  ObjectManager.cpp
//  OGame
//
//  Created by mac on 13-6-21.
//
//

#include "ObjectManager.h"

#include "Player.h"
#include "Monster.h"
#include "OgreManager.h"
#include "BulletManager.h"
template<> ObjectMgr* Singleton<ObjectMgr>::msSingleton = 0;
ObjectMgr::ObjectMgr():mPlayer(0)
{
    
}

ObjectMgr::~ObjectMgr()
{
    for(int i = 0; i < mMonsters.size(); ++i)
    {
        delete mMonsters[i];
    }
    mMonsters.clear();
    
    delete mPlayer;
}

bool ObjectMgr::Init()
{
    mPlayer = new Player();
    
    SceneNode* pNode = g_pSceneMgrPtr->getRootSceneNode()->createChildSceneNode();
    Entity* pEnt = g_pSceneMgrPtr->createEntity("SinbadBody", "Sinbad.mesh");
    pNode->attachObject(pEnt);
    
    float fScaleFactor = 1.8 / pEnt->getBoundingBox().getSize().y;
    float fWidth = pEnt->getBoundingBox().getSize().x * fScaleFactor;
    pNode->scale(Vector3(fScaleFactor));
    Vector3 aabbSize = pEnt->getBoundingBox().getSize() * fScaleFactor;
    
    Entity* pSwordEnt1 = g_pSceneMgrPtr->createEntity("SinbadSword1", "Sword.mesh");
    Entity* pSwordEnt2 = g_pSceneMgrPtr->createEntity("SinbadSword2", "Sword.mesh");
    pEnt->attachObjectToBone("Handle.L", pSwordEnt1);
    pEnt->attachObjectToBone("Handle.R", pSwordEnt2);
    
    btTransform startTransform;
	startTransform.setIdentity ();
	startTransform.setOrigin (btVector3(0.0, 24.0, 0.0));
    
	btPairCachingGhostObject* pGhostObject = new btPairCachingGhostObject();
	pGhostObject->setWorldTransform(startTransform);
    
	btScalar characterHeight=1.8;
	btScalar characterWidth =fWidth / 2.0f;
	//btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
    btConvexShape* boxShape = new btBoxShape(btVector3(aabbSize.x, aabbSize.y, aabbSize.z));
	pGhostObject->setCollisionShape (boxShape);
	pGhostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
    
	btScalar stepHeight = btScalar(0.35);
    btKinematicCharacterController* pCharactorCtrl = new btKinematicCharacterController (pGhostObject,boxShape,stepHeight);
    
    g_pBulletMgr->GetWord()->addCollisionObject(pGhostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
    
	g_pBulletMgr->GetWord()->addAction(pCharactorCtrl);
    
    g_pBulletMgr->GetWord()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(pGhostObject->getBroadphaseHandle(),g_pBulletMgr->GetWord()->getDispatcher());
    
    g_pBulletMgr->SetGhostObject(pGhostObject);
    g_pBulletMgr->AddCollisionShape(boxShape);
    
    pCharactorCtrl->reset();
    pCharactorCtrl->warp (btVector3(0.0, 24.0, 0.0));
    pCharactorCtrl->setGravity(20.0);
    //pCharactorCtrl->setJumpSpeed(5.0);
    ///pCharactorCtrl->setFallSpeed(95.0);
    
    mPlayer->Init(pNode, pEnt, pGhostObject);
    mPlayer->InitControl(pCharactorCtrl);
    return true;
}

void ObjectMgr::Update(double delta)
{
    if(mPlayer)
        mPlayer->Update(delta);
    
    for(int i = 0; i < mMonsters.size(); ++i)
    {
        mMonsters[i]->Update(delta);
    }
}

Player* ObjectMgr::GetPlayer() const
{
    return mPlayer;
}

void ObjectMgr::AddMonster(Monster* pMonster)
{
    if(!pMonster)
        return;
    
    mMonsters.push_back(pMonster);
}