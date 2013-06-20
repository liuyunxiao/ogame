//
//  PhyFactory.h
//  OGame
//
//  Created by mac on 13-6-20.
//
//

#ifndef OGame_PhyFactory_h
#define OGame_PhyFactory_h

struct ShapeCfg
{
    ShapeCfg(){};
}

struct BoxShapCfg:public ShapeCfg
{
    btVector3   size;
}

struct CapsuelCfg:public ShapeCfg
{
}

#endif
