//
//  Role.h
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#ifndef __OGame__Role__
#define __OGame__Role__
#include "GObject.h"

class AnimationCtrl;

class Role: public GObject
{
public:
    Role();
    ~Role();
    
    virtual void Update(double delta);
    virtual void SetDirtion(Vector2 dir);
//protected:
    AnimationCtrl*      mAniCtrl;
};

#endif /* defined(__OGame__Role__) */
