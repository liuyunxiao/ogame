//
//  Player.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "Player.h"
#include "AnimationCtrl.h"
#include "OgreManager.h"
#include "PlayerFSM.h"

Player::Player():mCharacterCtl(0),mbRun(false)
{
    
}

Player::~Player()
{
    
}

void Player::Init(SceneNode* node, Entity* ent, btCollisionObject* collison)
{
    Role::Init(node, ent, collison);
    mFSM = new PlayerFSM(this);
}

bool Player::InitControl(btKinematicCharacterController* pCharacter)
{
    mCharacterCtl = pCharacter;
}

void Player::SetRun(bool bRun)
{
    mbRun = bRun;
    if(mbRun)
    {
        mAniCtrl->PlayAni("RunBase", eAPT_Base);
        mAniCtrl->PlayAni("RunTop", eAPT_Top);
    }
    else
    {
        mAniCtrl->PlayAni("IdleBase", eAPT_Base);
        mAniCtrl->PlayAni("IdleTop", eAPT_Top);
        
    }
}

 void Player::Jump()
{
    if(mCharacterCtl)
    {
        mCharacterCtl->jump();
        mAniCtrl->PlayAni("JumpLoop", eAPT_Base);
        mAniCtrl->PlayAni("JumpLoop", eAPT_Top);
    }
}

void Player::Update(double delta)
{
    Role::Update(delta);
    
    if(mCharacterCtl)
    {
        btVector3 walkDir(0.0, 0.0, 0.0);
        
        btTransform& xform = mCollisionObject->getWorldTransform ();
		btVector3 forwardDir = xform.getBasis()[2];
        forwardDir.setZ(-forwardDir.z());
        forwardDir.normalize();
        //forwardDir.rotate(btVector3(0.0, 1.0, 0.0), -3.14 / 2.0);
        forwardDir.normalize();
		btScalar walkVelocity = btScalar(1.1) * 10.0; // 4 km/h -> 1.1 m/s
		btScalar walkSpeed = walkVelocity * delta;
        if(mbRun)
        {
            walkDir += forwardDir;
        }
        mCharacterCtl->setWalkDirection(-walkDir*walkSpeed);
        
        UpdatePhyTransform(mCollisionObject->getWorldTransform());
        
        Vector3 pos = OgreMgr::getSingletonPtr()->m_pCamera->getPosition();
        Vector3 dir = OgreMgr::getSingletonPtr()->m_pCamera->getDirection();
        dir.normalise();
        Vector3 playerPos = mSceneNode->getPosition();
        
        Vector3 camPos = playerPos - 50 * dir;
        OgreMgr::getSingletonPtr()->m_pCamera->setPosition(camPos);
        
    }
}

void Player::OnCollied(GObject* obj)
{
    if(obj->GetCollisionObj())
    {
        btRigidBody* body = btRigidBody::upcast(obj->GetCollisionObj());
        if(body)
        {
            body->setAngularVelocity(btVector3(0.0, 2.0, 0.0));
            body->setAngularFactor(btVector3(0.0, -0.2, 0.0));
        }
    }
}