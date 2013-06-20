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

class AnimationCtrl
{
public:
    AnimationCtrl(Entity* ent);
    ~AnimationCtrl();
    
    void PlayAni(String name, EAniPartType partType, bool bLoop = true);
    void UpdateAnis(double delta);
    
private:
    Entity*         mEntity;
    AnimationState* mAniParts[eAPT_All];
};
#endif /* defined(__OGame__AnimationCtrl__) */
