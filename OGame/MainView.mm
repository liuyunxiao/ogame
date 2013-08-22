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

-(void)awakeFromNib
{
    UISwipeGestureRecognizer *recognizer;
    
    recognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeFrom:)];
    [recognizer setDirection:(UISwipeGestureRecognizerDirectionRight)];
    [self addGestureRecognizer:recognizer];
    [recognizer release];
    
    recognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeFrom:)];
    [recognizer setDirection:(UISwipeGestureRecognizerDirectionUp)];
    [self addGestureRecognizer:recognizer];
    [recognizer release];
    
    recognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeFrom:)];
    [recognizer setDirection:(UISwipeGestureRecognizerDirectionDown)];
    [self addGestureRecognizer:recognizer];
    [recognizer release];
    
    recognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipeFrom:)];
    [recognizer setDirection:(UISwipeGestureRecognizerDirectionLeft)];
    [self addGestureRecognizer:recognizer];
    [recognizer release];
}

-(void)handleSwipeFrom:(UISwipeGestureRecognizer*)swipeGesture
{
    if (swipeGesture.direction==UISwipeGestureRecognizerDirectionLeft)
    {
        Player* pPlayer = ObjectMgr::getSingletonPtr()->GetPlayer();
        if(!pPlayer)
            return;
        
        pPlayer->GoLeft();
        NSLog(@"UISwipeGestureRecognizerDirectionLeft");
    }
    else if(swipeGesture.direction==UISwipeGestureRecognizerDirectionRight)
    {
        Player* pPlayer = ObjectMgr::getSingletonPtr()->GetPlayer();
        if(!pPlayer)
            return;
        
        pPlayer->GoRight();
        NSLog(@"UISwipeGestureRecognizerDirectionRight");
    }
    else if(swipeGesture.direction==UISwipeGestureRecognizerDirectionDown)
    {
        NSLog(@"UISwipeGestureRecognizerDirectionDown");
    }
    else if(swipeGesture.direction==UISwipeGestureRecognizerDirectionUp)
    {
        Player* pPlayer = ObjectMgr::getSingletonPtr()->GetPlayer();
        if(!pPlayer)
            return;
        
        pPlayer->Jump();
        NSLog(@"UISwipeGestureRecognizerDirectionUp");
    }
}

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
