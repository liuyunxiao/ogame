//
//  BulletManager.h
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#ifndef __OGame__PhysicsManager__
#define __OGame__PhysicsManager__
class BulletMgr:public Singleton<BulletMgr>
{
public:
    BulletMgr();
    ~BulletMgr();
    
    bool Init();
    void Update(double delta);
    
    void AddRigidBody(btRigidBody* pBody);
    
    btDiscreteDynamicsWorld* GetWord()const {return mDynamicsWorld;}
private:
	BulletMgr(const BulletMgr&);
	BulletMgr& operator= (const BulletMgr&);
    
    btDiscreteDynamicsWorld*                mDynamicsWorld;
    btCollisionConfiguration*               mCollisionConfig;
    btCollisionDispatcher*                  mCollisionDispatcher;
    btSequentialImpulseConstraintSolver*    mConstraintSolver;
    btBroadphaseInterface*                  mBroadphase;
    
    btAlignedObjectArray<btCollisionShape*> mCollisionShapes;
    btAlignedObjectArray<btRigidBody*>      mRigidBodys;
};

#endif /* defined(__OGame__PhysicsManager__) */
