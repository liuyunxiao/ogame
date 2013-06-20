//
//  GObject.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "GObject.h"
#include "OgreFramework.h"

GObject::GObject():mSceneNode(0),mCollisionObject(0),mEntity(0)
{
    
}

GObject::~GObject()
{
    
}

void GObject::Destroy()
{
    if(mSceneNode)
    {
    }
}

void GObject::UpdatePhyTransform(const btTransform &trans)
{
    if(!mSceneNode || !mCollisionObject)
        return;
    
    if(mCollisionObject->isStaticObject())
        return;
    
    const btVector3& pos = trans.getOrigin();
    mSceneNode->setPosition(Vector3(pos.getX(),pos.getY(),pos.getZ()));
    
    btQuaternion qua = trans.getRotation();
    mSceneNode->setOrientation(Quaternion(qua.getW(), qua.getX(), qua.getY(), qua.getZ()));
}