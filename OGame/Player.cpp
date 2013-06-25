//
//  Player.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "Player.h"


Player::Player():mCharacterCtl(0)
{
    
}

Player::~Player()
{
    
}

bool Player::InitControl(btKinematicCharacterController* pCharacter)
{
    mCharacterCtl = pCharacter;
}

void Player::Update(double delta)
{
    Role::Update(delta);
    
    if(mCharacterCtl)
    {
        btTransform xform;
		xform = mCollisionObject->getWorldTransform ();
        
		btVector3 forwardDir = xform.getBasis()[2];
        //	printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
		btVector3 upDir = xform.getBasis()[1];
		btVector3 strafeDir = xform.getBasis()[0];
		forwardDir.normalize ();
		upDir.normalize ();
		strafeDir.normalize ();
        
		btVector3 walkDirection = btVector3(0.8, 0.0, 0.0);
		btScalar walkVelocity = btScalar(1.1) * 4.0; // 4 km/h -> 1.1 m/s
		btScalar walkSpeed = walkVelocity * delta;
        
        
		mCharacterCtl->setWalkDirection(walkDirection*walkSpeed);

        UpdatePhyTransform(mCollisionObject->getWorldTransform());
    }
}