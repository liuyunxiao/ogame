//
//  ObjectManager.h
//  OGame
//
//  Created by mac on 13-6-21.
//
//

#ifndef __OGame__ObjectManager__
#define __OGame__ObjectManager__

class Player;

class ObjectMgr:public Singleton<ObjectMgr>
{
public:
    ObjectMgr();
    ~ObjectMgr();
    
    bool Init();
    void Update(double delta);
    
    Player* GetPlayer() const;
private:
    ObjectMgr(const ObjectMgr&);
    ObjectMgr& operator= (const ObjectMgr&);
    
    Player*             mPlayer;
};

#endif /* defined(__OGame__ObjectManager__) */
