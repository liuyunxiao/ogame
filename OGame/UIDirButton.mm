//
//  UIDirButton.m
//  OGame
//
//  Created by liuyunxiao on 13-6-23.
//
//

#import "UIDirButton.h"

#include "ObjectManager.h"
#include "Player.h"
@implementation UIDirButton

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        
        
    }
    return self;
}

-(void)awakeFromNib
{
    UIPanGestureRecognizer *_panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panGestureUpdated:)];
    [self addGestureRecognizer:_panGesture];
    [_panGesture release];
    
    [self setImage:[UIImage imageNamed:@"btn_archor"] forState:UIControlStateNormal];
}

#pragma mark _drag

-(void)returnToOrigion
{
    CGRect btnRec = self.frame;
    CGRect viewRec = self.superview.frame;
    self.frame = CGRectMake(viewRec.size.width / 2 - btnRec.size.width / 2,
                            viewRec.size.height / 2 - btnRec.size.height / 2,
                            btnRec.size.width,
                            btnRec.size.height);
    
    Player* pPlayer = ObjectMgr::getSingletonPtr()->GetPlayer();
    if(!pPlayer)
        return;
    
    pPlayer->SetRun(false);
}
-(void) panGestureUpdated:(UIPanGestureRecognizer*)panGesture
{
    CGPoint point = [panGesture locationInView:self.superview];
    switch (panGesture.state)
    {
        case UIGestureRecognizerStateBegan:
        {
            break;
        }
        case UIGestureRecognizerStateChanged:
        {
            Player* pPlayer = ObjectMgr::getSingletonPtr()->GetPlayer();
            if(!pPlayer)
                break;
            
            CGRect btnRec = self.frame;
            CGRect viewRec = self.superview.frame;
            
            //转换到笛卡尔坐标系
            Vector2 curPos(point.x - viewRec.size.width / 2,
                           point.y - viewRec.size.height / 2);
            
            
            if(curPos.length() >= 60.0f)
            {
                Vector2 norPos = curPos;
                norPos.normalise();
                Vector2 newPos = 60.0f * norPos;
                self.frame = CGRectMake(newPos.x + viewRec.size.width / 2 - btnRec.size.width / 2, newPos.y + viewRec.size.height / 2 - btnRec.size.height / 2, btnRec.size.width, btnRec.size.height);
            }
            else
            {
                self.frame = CGRectMake(point.x - btnRec.size.width / 2, point.y - btnRec.size.height / 2, btnRec.size.width, btnRec.size.height);
            }
            pPlayer->SetDirtion(curPos);
            pPlayer->SetRun(true);
            break;
        }
        case UIGestureRecognizerStateEnded:
        {
            [self returnToOrigion];
            break;
        }
        case UIGestureRecognizerStateCancelled:
        {
            [self returnToOrigion];
            break;
        }
        case UIGestureRecognizerStateFailed:
        {
            [self returnToOrigion];
            break;
        }
        default:
            break;
            
    }
}


/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
