//
//  TestView.m
//  OGame
//
//  Created by mac on 13-6-6.
//
//

#import "MainView.h"
#include "OgreManager.h"
#include "Player.h"
#include "ObjectManager.h"
#include "MapManager.h"
@implementation MainView

-(IBAction)testCicked:(id)sender
{
    Player* pPlayer = ObjectMgr::getSingletonPtr()->GetPlayer();
    if(!pPlayer)
        return;
    
    pPlayer->Jump();
}

-(IBAction)shoutClicked:(id)sender
{
    MapMgr::getSingletonPtr()->ShoutMonster();
    float fps = OgreMgr::getSingletonPtr()->m_pRenderWnd->getStatistics().lastFPS;

    texFps.text = [NSString stringWithFormat:@"%f", fps];
}
@end
