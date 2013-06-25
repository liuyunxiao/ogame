//
//  Player.h
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#ifndef __OGame__Player__
#define __OGame__Player__
#include "Role.h"
class btKinematicCharacterController;

class Player:public Role
{
public:
    Player();
    ~Player();
    
    virtual void Update(double delta);
    void SetRun(bool bRun);
    void Jump();
    bool InitControl(btKinematicCharacterController* pCharacter);
private:
    btKinematicCharacterController* mCharacterCtl;
    bool mbRun;
};
#endif /* defined(__OGame__Player__) */
