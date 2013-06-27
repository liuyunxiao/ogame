//
//  FSMState.h
//  OGame
//
//  Created by mac on 13-6-27.
//
//

#ifndef __OGame__FSMState__
#define __OGame__FSMState__

class Player;
class FState
{
public:
    FState(Player* pPlayer):mOwner(pPlayer)
    {}
    virtual void Start(){}
    virtual void Update(){}
    virtual void Exit(){}
    void AddChangeableState(FState* state)
    {
        mStateChangeable.push_back(state);
    }
    
    bool CanChangeToState(FState* state)
    {
        std::vector<FState*>::iterator itor = std::find(mStateChangeable.begin(), mStateChangeable.end(), state);
        
        return (itor != mStateChangeable.end());
    }
    
protected:
    Player*                 mOwner;
    std::vector<FState*>    mStateChangeable;
};

#endif /* defined(__OGame__FSMState__) */
