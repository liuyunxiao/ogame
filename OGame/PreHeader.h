//
//  PreHeader.h
//  OGame
//
//  Created by mac on 13-6-21.
//
//

#ifndef OGame_PreHeader_h
#define OGame_PreHeader_h

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "btGhostObject.h"
#include "btKinematicCharacterController.h"


#include "Ogre.h"
using namespace Ogre;


//#define USING_OGRE_TERRAIN
#ifdef USING_OGRE_TERRAIN
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"
#endif


#endif


