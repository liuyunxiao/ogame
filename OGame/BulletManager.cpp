//
//  PhysicsManager.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "BulletManager.h"
#include "GObject.h"

template<> BulletMgr* Singleton<BulletMgr>::msSingleton = 0;
BulletMgr::BulletMgr():
    mDynamicsWorld(0),
    mCollisionConfig(0),
    mCollisionDispatcher(0),
    mConstraintSolver(0),
    mBroadphase(0)
{
    
}

BulletMgr::~BulletMgr()
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

bool BulletMgr::Init()
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
    
    sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    mDynamicsWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;
	mDynamicsWorld->setGravity(btVector3(0,-10,0));
    
    return true;
}

void BulletMgr::Update(double delta)
{
    if (mDynamicsWorld)
	{
		mDynamicsWorld->stepSimulation(delta, 2);//deltaTime);
		{
			static int i=0;
			if (i<10)
			{
				i++;
				//CProfileManager::dumpAll();
			}
		}
	}
    
	for (int i=0;i<mRigidBodys.size();i++)
	{
        const btTransform& trans = mRigidBodys[i]->getCenterOfMassTransform();
        
        GObject* pObj = (GObject*)mRigidBodys[i]->getUserPointer();
        if(pObj)
            pObj->UpdatePhyTransform(trans);
    }
    
//    int nNumManifold = mDynamicsWorld->getDispatcher()->getNumManifolds();
//    for(int i = 0;i < nNumManifold; ++i)
//    {
//        btPersistentManifold* contactManifld = mDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
//        
//        int numContacts = contactManifld->getNumContacts();
//        for (int j = 0; j < numContacts; ++j)
//        {
//            btManifoldPoint& contactPoint = contactManifld->getContactPoint(j);
//        }
//        if(numContacts > 0)
//        {
//            btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifld->getBody0());
//            btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifld->getBody1());
//            
//            GObject* pObjA = (GObject*)obA->getUserPointer();
//            GObject* pObjB = (GObject*)obB->getUserPointer();
//        }
//    }
}

void BulletMgr::AddRigidBody(btRigidBody* pBody)
{
    if(NULL == pBody)
        return;
    mDynamicsWorld->addRigidBody(pBody);
    mRigidBodys.push_back(pBody);
}


