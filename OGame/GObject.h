//
//  GObject.h
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#ifndef __OGame__GObject__
#define __OGame__GObject__

class GObject
{
public:
    GObject();
    ~GObject();
    
    virtual void Destroy();
    
    virtual void Update();
    virtual void UpdatePhyTransform(const btTransform& trans);
    
    btCollisionObject*  GetCollisionObj() const {return mCollisionObject;}
    SceneNode*          GetSceneNode() const    {return mSceneNode;}
    Entity*             GetEntity() const       {return mEntity;}
    
    bool                IsColliedObj()  {return (mCollisionObject != 0);}
    bool                IsSceneNode()   {return (mSceneNode != 0);}
    bool                IsEntity()      {return (mEntity != 0);}
    
protected:
    Entity*             mEntity;
    SceneNode*          mSceneNode;
    btCollisionObject*  mCollisionObject;
};

#endif /* defined(__OGame__GObject__) */
