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
    //g_OgreFramePtr->shoutBox();
    
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

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
     
    //g_OgreFramePtr->m_pCamera->yaw(Ogre::Radian(0.1));
}


@end
