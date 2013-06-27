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
    std::map<String, FState*>::iterator itor = mStates.find(name);
    if(itor == mStates.end())
        return false;
    
    FState* newState = itor->second;
    
    if(mpPreState)
        mpPreState->Exit();
    
    mpPreState = newState;
    newState->Start();
}
