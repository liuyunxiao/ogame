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

template<> ObjectMgr* Singleton<ObjectMgr>::msSingleton = 0;
ObjectMgr::ObjectMgr():mPlayer(0)
{
    
}

ObjectMgr::~ObjectMgr()
{
    delete mPlayer;
    
    for(int i = 0; i < mMonsters.size(); ++i)
    {
        delete mMonsters[i];
    }
    mMonsters.clear();
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