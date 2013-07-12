//
//  PlayerFSM.h
//  OGame
//
//  Created by mac on 13-6-25.
//
//

#ifndef __OGame__PlayerFSM__
#define __OGame__PlayerFSM__

class FState;
class Player;
class PlayerFSM
{
public:
    PlayerFSM(Player* pPlayer);
    ~PlayerFSM();
    void Update();
    void InitDefaultState();
    void RegisterState(String name, FState* state);
    bool ChangeToState(String name);
private:
    std::map<String, FState*>   mStates;
    FState*                     mpPreState;
    FState*                     mpCurState;
    Player*                     mpOwner;
};

#endif /* defined(__OGame__PlayerFSM__) */
