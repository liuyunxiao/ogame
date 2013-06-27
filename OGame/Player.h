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
class PlayerFSM;
class Player:public Role
{
public:
    Player();
    ~Player();
    
    virtual void Update(double delta);
    virtual void OnCollied(GObject* obj);
    virtual void Init(SceneNode* node, Entity* ent = 0, btCollisionObject* collison = 0);
    
    bool InitControl(btKinematicCharacterController* pCharacter);
    //test
    void SetRun(bool bRun);
    void Jump();
    
    
private:
    btKinematicCharacterController* mCharacterCtl;
    bool mbRun;
    
    PlayerFSM*          mFSM;
};
#endif /* defined(__OGame__Player__) */
