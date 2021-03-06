//
//  PlayerFSM.cpp
//  OGame
//
//  Created by mac on 13-6-25.
//
//

#include "PlayerFSM.h"
#include "Player.h"
#include "FSMState.h"
#include "PlayerState.h"

PlayerFSM::PlayerFSM(Player* pPlayer):mpOwner(pPlayer),mpCurState(0),mpPreState(0)
{
    InitDefaultState();
}

PlayerFSM::~PlayerFSM()
{
    
}

void PlayerFSM::InitDefaultState()
{
    FState* state = new FIdleState(mpOwner);
    RegisterState("Idle", state);
    state = new FJumpState(mpOwner);
    RegisterState("Jump", state);
    state = new FRunState(mpOwner);
    RegisterState("Run", state);
    
    ChangeToState("Idle");
}

void PlayerFSM::RegisterState(String name, FState* state)
{
    mStates.insert(make_pair(name, state));
}

bool PlayerFSM::ChangeToState(String name)
{
    if(mpCurState)
    {
        if(!mpCurState->CanStop())
            return false;
    }
    
    std::map<String, FState*>::iterator itor = mStates.find(name);
    if(itor == mStates.end())
        return false;
    
    FState* newState = itor->second;
    
    if(mpCurState)
        mpCurState->Exit();
    
    mpCurState = newState;
    mpCurState->Start();
}

void PlayerFSM::Update()
{
    if(mpCurState)
        mpCurState->Update();
}
