#include "OgreManager.h"
#include "macUtils.h"
#import "MainView.h"
#import "PlayerControlView.h"
#import <QuartzCore/CAEAGLLayer.h>

template<> OgreMgr* Ogre::Singleton<OgreMgr>::msSingleton = 0;
OgreMgr::OgreMgr()
{
	m_MoveSpeed			= 0.1f;
	m_RotateSpeed       = 0.3f;
    
	m_bShutDownOgre     = false;
	m_iNumScreenShots   = 0;
    
	m_pRoot				= 0;
	m_pSceneMgr			= 0;
	m_pRenderWnd        = 0;
	m_pCamera			= 0;
	m_pViewport			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;
    
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = macBundlePath() + "/Contents/Resources/";
#elif defined(OGRE_IS_IOS)
    m_ResourcePath = macBundlePath() + "/";
#else
    m_ResourcePath = "";
#endif
    m_FrameEvent        = FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||
bool OgreMgr::Init(String wndTitle)
{
    new LogManager();

    m_pTimer = OGRE_NEW Ogre::Timer();
	m_pTimer->reset();
    
	m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);
    
    String pluginsPath;
    // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
    pluginsPath = m_ResourcePath + "plugins.cfg";
#endif
    
    m_pRoot = new Ogre::Root(pluginsPath, Ogre::macBundlePath() + "/ogre.cfg");
    
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.load();
#endif
    
	if(!m_pRoot->showConfigDialog())
		return false;
	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);
    
	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	
	m_pCamera = m_pSceneMgr->createCamera("Camera");
	m_pCamera->setPosition(Vector3(0, 35, 35));
	m_pCamera->lookAt(Vector3(0, 4, 0));
	m_pCamera->setNearClipDistance(1);
    
	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
	m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
	m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));
	
	m_pViewport->setCamera(m_pCamera);
    
	String secName, typeName, archName;
	ConfigFile cf;
    cf.load(m_ResourcePath + "resources.cfg");
    
	ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || defined(OGRE_IS_IOS)
            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
                archName = Ogre::String(m_ResourcePath + archName);
#endif
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	m_pRenderWnd->setActive(true);

    NSArray *items = [[NSBundle mainBundle] loadNibNamed:@"MainView" owner:nil options:nil];
    MainView* v = (MainView*)[items objectAtIndex:0];
    
    UIView* pView = NULL;
    m_pRenderWnd->getCustomAttribute("VIEW", &pView);
    v.userInteractionEnabled = YES;
    [pView addSubview:v];
    
    items = [[NSBundle mainBundle] loadNibNamed:@"PlayerControlView" owner:nil options:nil];
    
//    PlayerControlView* subView = (PlayerControlView*)[items objectAtIndex:0];
//    subView.userInteractionEnabled = YES;
//    [pView addSubview:subView];
    //[pView insertSubview:subView aboveSubview:v];
    //[pView bringSubviewToFront:subView];
    
    m_pSceneMgr->setSkyBoxEnabled(true);
    
//    sCollisionConfig = new btDefaultCollisionConfiguration();
//	
//	sBroadphase = new btDbvtBroadphase();
//	
//	sCollisionDispatcher = new btCollisionDispatcher(sCollisionConfig);
//	sConstraintSolver = new btSequentialImpulseConstraintSolver;
//	sDynamicsWorld = new btDiscreteDynamicsWorld(sCollisionDispatcher,sBroadphase,sConstraintSolver,sCollisionConfig);
//	sDynamicsWorld->setGravity(btVector3(0,-10,0));
//    
//    btTransform groundTransform;
//    groundTransform.setIdentity();
//    groundTransform.setOrigin(btVector3(0,0,0));
//    
//    btVector3 norm(0,1,0);
//    btCollisionShape* groundShape = new btStaticPlaneShape(norm,0.0);
//    btScalar mass(0.);	//rigidbody is dynamic if and only if mass is non zero, otherwise static
//    bool isDynamic = (mass != 0.f);
//    btVector3 localInertia(0,0,0);
//    if (isDynamic)
//        groundShape->calculateLocalInertia(mass,localInertia);
//    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
//    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
//    sFloorPlaneBody = new btRigidBody(rbInfo);
//    //add the body to the dynamics world
//    sDynamicsWorld->addRigidBody(sFloorPlaneBody);
//
//    String name = "box";
//    for (int i=0;i<numBodies;i++)
//	{
//        String namenum = name + StringConverter::toString(i);
//        Entity* boxEnt = m_pSceneMgr->createEntity(namenum, SceneManager::PT_CUBE);
//        Ogre::SceneNode* boxNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,10+i*3,0));
//        
//        boxEnt->setMaterialName("Examples/Box");
//        boxNode->attachObject(boxEnt);
//        boxNode->scale(Ogre::Vector3(0.01, 0.01, 0.01));
//        
//		btTransform bodyTransform;
//		bodyTransform.setIdentity();
//		bodyTransform.setOrigin(btVector3(0,10+i*3,0));
//		btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
//		btScalar mass(1.);//positive mass means dynamic/moving  object
//		bool isDynamic = (mass != 0.f);
//		btVector3 localInertia(0,0,0);
//		if (isDynamic)
//            boxShape->calculateLocalInertia(mass,localInertia);
//		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//		btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
//		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
//		btRigidBody* boxBody=new btRigidBody(rbInfo);
//		sBoxBodies.push_back(boxBody);
//        
//        boxBody->setUserPointer(boxNode);
//		
//		//most applications shouldn't disable deactivation, but for this demo it is better.
//		boxBody->setActivationState(DISABLE_DEACTIVATION);
//		//add the body to the dynamics world
//		sDynamicsWorld->addRigidBody(boxBody);
//	}
    
	return true;
}

void OgreMgr::shoutBox()
{
//    static int shoutnum = 0;
//    String name = "sbox";
//
//    Vector3 caPos = m_pCamera->getPosition();
//    caPos.z -= 3;
//    
//    Vector3 caOr = m_pCamera->getDirection();
//    
//        String namenum = name + StringConverter::toString(++shoutnum);
//        Entity* boxEnt = m_pSceneMgr->createEntity(namenum, SceneManager::PT_CUBE);
//        Ogre::SceneNode* boxNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(caPos);
//        
//        boxEnt->setMaterialName("Examples/Box");
//        boxNode->attachObject(boxEnt);
//        boxNode->scale(Ogre::Vector3(0.01, 0.01, 0.01));
//        
//		btTransform bodyTransform;
//		bodyTransform.setIdentity();
//		bodyTransform.setOrigin(btVector3(caPos.x,caPos.y,caPos.z));
//		btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
//		btScalar mass(1.);//positive mass means dynamic/moving  object
//		bool isDynamic = (mass != 0.f);
//		btVector3 localInertia(0,0,0);
//		if (isDynamic)
//            boxShape->calculateLocalInertia(mass,localInertia);
//		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//		btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
//		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
//		btRigidBody* boxBody=new btRigidBody(rbInfo);
//		sBoxBodies.push_back(boxBody);
//        
//        boxBody->setUserPointer(boxNode);
//		
//		//most applications shouldn't disable deactivation, but for this demo it is better.
//		boxBody->setActivationState(DISABLE_DEACTIVATION);
//		//add the body to the dynamics world
//		sDynamicsWorld->addRigidBody(boxBody);
//    //boxBody->setLinearVelocity(btVector3(0, 0, 3));
//
//    boxBody->setLinearVelocity(btVector3(caOr.x * 10, caOr.y * 10, caOr.z * 10));
    //boxBody->applyCentralForce(btVector3(caOr.x * 100, caOr.y * 100, caOr.z * 100));
	
}

OgreMgr::~OgreMgr()
{
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.unload();
#endif
    if(m_pRoot)     delete m_pRoot;
}

void OgreMgr::Update(double timeSinceLastFrame)
{
	m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;
    
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
}
