//
//  PlayerState.h
//  OGame
//
//  Created by mac on 13-6-27.
//
//

#ifndef __OGame__PlayerState__
#define __OGame__PlayerState__

#include "FSMState.h"
class FIdleState: public FState
{
public:
    FIdleState(Player* pPlayer):FState(pPlayer){}
    virtual void Start();
    virtual void Update();
    virtual void Exit();
private:
};

class FJumpState: public FState
{
    enum EJumpStateType
    {
        eJST_Start,
        eJST_Jumping,
        eJST_Over,
        eJST_End,
    };
public:
    FJumpState(Player* pPlayer):FState(pPlayer){}
    virtual void Start();
    virtual void Update();
    virtual void Exit();
    virtual bool CanStop();
private:
    EJumpStateType          mCurStateType;
};

class FRunState: public FState
{
public:
    FRunState(Player* pPlayer):FState(pPlayer){}
    virtual void Start();
    virtual void Update();
    virtual void Exit();
private:
};

#endif /* defined(__OGame__PlayerState__) */
