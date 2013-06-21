//
//  AnimationCtrl.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "AnimationCtrl.h"


AnimationCtrl::AnimationCtrl(Entity* ent):mEntity(ent)
{
    bzero(mAniParts, sizeof(mAniParts));
}

AnimationCtrl::~AnimationCtrl()
{
    
}

void AnimationCtrl::PlayAni(String name, EAniPartType partType, bool bLoop)
{
    //assert(partType < 0 || partType >= eAPT_All);
    AnimationState* ani = mEntity->getAnimationState(name);
    if(NULL == ani)
        return;
    
    mAniParts[partType] = ani;
    mAniParts[partType]->setLoop(bLoop);
    mAniParts[partType]->setEnabled(true);
}

void AnimationCtrl::UpdateAnis(double delta)
{
    for(int i = 0; i < eAPT_All; ++i)
    {
        if(mAniParts[i])
        {
            mAniParts[i]->addTime(delta);
        }
    }
}