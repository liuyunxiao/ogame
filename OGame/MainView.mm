//
//  TestView.m
//  OGame
//
//  Created by mac on 13-6-6.
//
//

#import "MainView.h"
#include "OgreFramework.h"
@implementation MainView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/


-(IBAction)testDown:(id)sender
{
    g_OgreFramePtr->shoutBox();
    //g_OgreFramePtr->m_pCamera->pitch(Ogre::Radian(0.01));
    //bBtnTestDown = YES;
}

-(IBAction)testUp:(id)sender
{
    //bBtnTestDown = NO;
    //g_OgreFramePtr->m_pCamera->pitch(Ogre::Radian(0.01));
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
