//
//  AnimationCtrl.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "AnimationCtrl.h"
#include "Role.h"

AnimationCtrl::AnimationCtrl(Role* role):mOwner(role)
{
    bzero(mAniParts, sizeof(mAniParts));
}

AnimationCtrl::~AnimationCtrl()
{
    
}

void AnimationCtrl::PlayAni(String name, EAniPartType partType, bool bLoop)
{
    if(!mOwner->GetEntity())
    {
        return;
    }
    
    AnimationState* ani = mOwner->GetEntity()->getAnimationState(name);
    if(NULL == ani)
        return;
    
    if(mAniParts[partType])
    {
        mAniParts[partType]->setEnabled(false);
    }
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