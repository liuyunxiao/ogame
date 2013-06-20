//
//  PhysicsManager.h
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#ifndef __OGame__PhysicsManager__
#define __OGame__PhysicsManager__
#include "OgreSingleton.h"

class PhysicsMgr:public Singleton<PhysicsMgr>
{
public:
    PhysicsMgr();
    ~PhysicsMgr();
    
    static PhysicsMgr* getSingletonPtr();
    static PhysicsMgr& getSingleton();
    
    bool Init();
    void Update();
    
    void CreateCharactor();
    
    btPairCachingGhostObject* CreateGhostObj();
private:
    btDiscreteDynamicsWorld*                mDynamicsWorld;
    btCollisionConfiguration*               mCollisionConfig;
    btCollisionDispatcher*                  mCollisionDispatcher;
    btSequentialImpulseConstraintSolver*    mConstraintSolver;
    btBroadphaseInterface*                  mBroadphase;
};

#endif /* defined(__OGame__PhysicsManager__) */
