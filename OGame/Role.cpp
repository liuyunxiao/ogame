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

void Role::SetDirtion(Vector2 dir)
{
    if(mSceneNode)
    {
        Radian rad = dir.angleBetween(Vector2(1.0, 0));
        if(dir.y < 0.0)
        {
            rad = 2 * Math::PI - rad.valueRadians();
        }
        rad = rad.valueRadians() + Math::PI / 2;
        Quaternion qut;
        qut.FromAngleAxis(rad, Vector3::UNIT_Y);
        mSceneNode->setOrientation(qut);
    }
}