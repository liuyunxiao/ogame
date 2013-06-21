//
//  ObjectManager.cpp
//  OGame
//
//  Created by mac on 13-6-21.
//
//

#include "ObjectManager.h"
#include "Player.h"
template<> ObjectMgr* Singleton<ObjectMgr>::msSingleton = 0;
ObjectMgr::ObjectMgr():mPlayer(0)
{
    
}

ObjectMgr::~ObjectMgr()
{
    delete mPlayer;
}

bool ObjectMgr::Init()
{
    mPlayer = new Player();
    return true;
}

void ObjectMgr::Update(double delta)
{
    if(mPlayer)
        mPlayer->Update(delta);
}

Player* ObjectMgr::GetPlayer() const
{
    return mPlayer;
}