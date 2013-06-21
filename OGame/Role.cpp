//
//  Role.cpp
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#include "Role.h"
#include "AnimationCtrl.h"

Role::Role():mAniCtrl(0)
{
    
}

Role::~Role()
{
    
}

void Role::Update(double delta)
{
    GObject::Update(delta);
    
    if(mAniCtrl)
        mAniCtrl->UpdateAnis(delta);
}