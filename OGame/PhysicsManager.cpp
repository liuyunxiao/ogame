//
//  PhysicsManager.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "PhysicsManager.h"

template<> PhysicsMgr* Singleton<PhysicsMgr>::msSingleton = 0;
PhysicsMgr* PhysicsMgr::getSingletonPtr()
{
    return msSingleton;
}

PhysicsMgr& PhysicsMgr::getSingleton()
{
    return (*msSingleton);
}

PhysicsMgr::PhysicsMgr():
    mDynamicsWorld(0),
    mCollisionConfig(0),
    mCollisionDispatcher(0),
    mConstraintSolver(0),
    mBroadphase(0)
{
    
}

PhysicsMgr::~PhysicsMgr()
{
    //delete dynamics world
	delete mDynamicsWorld;
    
	//delete solver
	delete mConstraintSolver;
    
	//delete broadphase
	delete mBroadphase;
    
	//delete dispatcher
	delete mCollisionDispatcher;
    
	delete mCollisionConfig;
}

bool PhysicsMgr::Init()
{
    mCollisionConfig = new btDefaultCollisionConfiguration();

	mBroadphase = new btDbvtBroadphase();
    
    btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	btAxisSweep3* sweepBP = new btAxisSweep3(worldMin,worldMax);
	mBroadphase = sweepBP;
	
	mCollisionDispatcher = new btCollisionDispatcher(mCollisionConfig);
	mConstraintSolver = new btSequentialImpulseConstraintSolver;
	mDynamicsWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher,mBroadphase,mConstraintSolver,mCollisionConfig);
    
	mDynamicsWorld->setGravity(btVector3(0,-10,0));
}

btPairCachingGhostObject* PhysicsMgr::CreateGhostObj()
{
    return (new btPairCachingGhostObject());
}

void PhysicsMgr::CreateCharactor()
{
    btTransform startTransform;
	startTransform.setIdentity ();
	startTransform.setOrigin (btVector3(0.0, 0.0, 0.0));
    
//	mCollisionObject = new btPairCachingGhostObject();
//	mCollisionObject->setWorldTransform(startTransform);
//    
//	sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
//	btScalar characterHeight=1.75;
//	btScalar characterWidth =1.75;
//	btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
//	m_ghostObject->setCollisionShape (capsule);
//	m_ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);
//    
//	btScalar stepHeight = btScalar(0.35);
//	m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);
//    
//    mCollisionObject = new btPairCachingGhostObject();
}

