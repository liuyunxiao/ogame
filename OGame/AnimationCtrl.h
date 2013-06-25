//
//  AnimationCtrl.h
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#ifndef __OGame__AnimationCtrl__
#define __OGame__AnimationCtrl__
#include "Object_define.h"
class Role;
class AnimationCtrl
{
public:
    AnimationCtrl(Role* role);
    ~AnimationCtrl();
    
    void PlayAni(String name, EAniPartType partType, bool bLoop = true);
    void UpdateAnis(double delta);
    
private:
    Role*           mOwner;
    AnimationState* mAniParts[eAPT_All];
};
#endif /* defined(__OGame__AnimationCtrl__) */
