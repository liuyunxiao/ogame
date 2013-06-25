//
//  PlayerFSM.h
//  OGame
//
//  Created by mac on 13-6-25.
//
//

#ifndef __OGame__PlayerFSM__
#define __OGame__PlayerFSM__

class Player;
class FState
{
public:
    FState(Player* pPlayer):mOwner(pPlayer)
    {}
    virtual void Start(){}
    virtual void Update(){}
    virtual void Exit(){}
protected:
    Player*     mOwner;
};

class FJumpState: public FState
{
public:
    virtual void Start();
    virtual void Update();
    virtual void Exit();
private:
    
};

#endif /* defined(__OGame__PlayerFSM__) */
