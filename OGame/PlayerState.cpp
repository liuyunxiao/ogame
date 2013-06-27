//
//  PlayerState.cpp
//  OGame
//
//  Created by mac on 13-6-27.
//
//

#include "PlayerState.h"
#include "AnimationCtrl.h"
#include "Player.h"

//idle
void FIdleState::Start()
{
    AnimationCtrl* pCtrl = mOwner->GetAniCtrl();
    pCtrl->PlayAni("IdleBase", eAPT_Base);
    pCtrl->PlayAni("IdleTop", eAPT_Top);
}
void FIdleState::Update()
{
    
}
void FIdleState::Exit()
{
    
}

//jump
void FJumpState::Start()
{
    AnimationCtrl* pCtrl = mOwner->GetAniCtrl();
    pCtrl->PlayAni("IdleBase", eAPT_Base);
    pCtrl->PlayAni("IdleTop", eAPT_Top);
}

void FJumpState::Update()
{
    
}

void FJumpState::Exit()
{
    
}

//run
void FRunState::Start()
{
    
}

void FRunState::Update()
{
    
}

void FRunState::Exit()
{
    
}