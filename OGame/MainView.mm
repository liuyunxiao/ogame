//
//  TestView.m
//  OGame
//
//  Created by mac on 13-6-6.
//
//

#import "MainView.h"
#include "OgreManager.h"
@implementation MainView

-(IBAction)testCicked:(id)sender
{
    g_OgreFramePtr->shoutBox();
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    g_OgreFramePtr->m_pCamera->yaw(Ogre::Radian(0.1));
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

@end
