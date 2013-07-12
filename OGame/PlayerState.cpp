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
#include "PlayerFSM.h"

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
    pCtrl->PlayAni("JumpStart", eAPT_Base, false);
    pCtrl->PlayAni("JumpStart", eAPT_Top, false);
    mCurStateType = eJST_Start;
}

void FJumpState::Update()
{
    if(eJST_Start == mCurStateType)
    {
        AnimationCtrl* pCtrl = mOwner->GetAniCtrl();
        if(pCtrl->IsAniEnd(eAPT_Base) && pCtrl->IsAniEnd(eAPT_Top))
        {
            pCtrl->PlayAni("JumpLoop", eAPT_Base);
            pCtrl->PlayAni("JumpLoop", eAPT_Top);
        
            mCurStateType = eJST_Jumping;
        }
    }
    else if(eJST_Jumping == mCurStateType)
    {
        if(!mOwner->IsJumping())
        {
            AnimationCtrl* pCtrl = mOwner->GetAniCtrl();
            pCtrl->PlayAni("JumpEnd", eAPT_Base, false);
            pCtrl->PlayAni("JumpEnd", eAPT_Top, false);
            
            mCurStateType = eJST_Over;
        }
    }
    else if(eJST_Over == mCurStateType)
    {
        AnimationCtrl* pCtrl = mOwner->GetAniCtrl();
        if(pCtrl->IsAniEnd(eAPT_Base) && pCtrl->IsAniEnd(eAPT_Top))
        {
            mOwner->GetFSM()->ChangeToState("Run");
            mCurStateType = eJST_End;
        }
    }
}

bool FJumpState::CanStop()
{
    if(eJST_End != mCurStateType)
        return false;
    
    return true;
}

void FJumpState::Exit()
{
    mCurStateType = eJST_End;
}

//run
void FRunState::Start()
{
    AnimationCtrl* pCtrl = mOwner->GetAniCtrl();
    pCtrl->PlayAni("RunBase", eAPT_Base);
    pCtrl->PlayAni("RunTop", eAPT_Top);
}

void FRunState::Update()
{
    
}

void FRunState::Exit()
{
    
}