#include "OgreFramework.h"
#include "macUtils.h"
//#import "cocos2d.h"
//#import "EAGLView.h"
//#import "CCDirectorCaller.h"
//#include "AppDelegate.h"
#import "MainView.h"
#import <QuartzCore/CAEAGLLayer.h>
#include "btBulletDynamicsCommon.h"
//#import "OGRender2View.h"
using namespace Ogre;
static btDiscreteDynamicsWorld* sDynamicsWorld=0;
static btCollisionConfiguration* sCollisionConfig=0;
static btCollisionDispatcher* sCollisionDispatcher=0;
static btSequentialImpulseConstraintSolver* sConstraintSolver;
static btBroadphaseInterface* sBroadphase=0;
btAlignedObjectArray<btCollisionShape*> sCollisionShapes;
btAlignedObjectArray<btRigidBody*> sBoxBodies;
btRigidBody* sFloorPlaneBody=0;
int numBodies = 10;
namespace Ogre
{
    template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;
};

OgreFramework::OgreFramework()
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
    
	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;
    
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = macBundlePath() + "/Contents/Resources/";
#elif defined(OGRE_IS_IOS)
    m_ResourcePath = macBundlePath() + "/";
#else
    m_ResourcePath = "";
#endif
    m_pTrayMgr          = 0;
    m_FrameEvent        = Ogre::FrameEvent();
}

//|||||||||||||||||||||||||||||||||||||||||||||||
#if defined(OGRE_IS_IOS)
bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MultiTouchListener *pMouseListener)
#else
bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
#endif
{
    new Ogre::LogManager();

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
	m_pCamera->setPosition(Vector3(0, 8, 25));
	m_pCamera->lookAt(Vector3(0, 4, 0));
	m_pCamera->setNearClipDistance(1);
    
	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
	m_pViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
	m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));
	
	m_pViewport->setCamera(m_pCamera);

	unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);
    
	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
    
	//m_pInputMgr = OIS::InputManager::createInputSystem(paramList);
    
#if !defined(OGRE_IS_IOS)
    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
    
	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();
#else
	//m_pMouse = static_cast<OIS::MultiTouch*>(m_pInputMgr->createInputObject(OIS::OISMultiTouch, true));
#endif
    
#if !defined(OGRE_IS_IOS)
	if(pKeyListener == 0)
		m_pKeyboard->setEventCallback(this);
	else
		m_pKeyboard->setEventCallback(pKeyListener);
#endif
    
//	if(pMouseListener == 0)
//		m_pMouse->setEventCallback(this);
//	else
//		m_pMouse->setEventCallback(pMouseListener);
    
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load(m_ResourcePath + "resources.cfg");
    
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
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
    
	
	
	//m_pTrayMgr = new OgreBites::SdkTrayManager("TrayMgr", m_pRenderWnd, m_pMouse, this);
    //m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    
    //m_pTrayMgr->createButton(OgreBites::TL_RIGHT, "StartStop", "Start Sample", 120);
    //m_pTrayMgr->hideCursor();

    
	m_pRenderWnd->setActive(true);
    
    //windowResized(m_pRenderWnd);
    UIWindow* pWindow = NULL;
    m_pRenderWnd->getCustomAttribute("WINDOW", &pWindow);
    
    NSArray *items = [[NSBundle mainBundle] loadNibNamed:@"MainView" owner:nil options:nil];
    MainView* v = (MainView*)[items objectAtIndex:0];
    
    UIView* pView = NULL;
    m_pRenderWnd->getCustomAttribute("VIEW", &pView);
    v.userInteractionEnabled = YES;
    [pView addSubview:v];
    
    // create a floor mesh resource
    MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);
    
    // create a floor entity, give it a material, and place it at the origin
    Entity* floor = m_pSceneMgr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/Rockwall");
    floor->setCastShadows(false);
    SceneNode* floorNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0.0, 0.0, 0.0));
    
    floorNode->attachObject(floor);
    
    Ogre::SceneNode* mBodyNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3::UNIT_Y * 5);
    Ogre::Entity* mBodyEnt = g_SceneMgrPtr->createEntity("SinbadBody", "Sinbad.mesh");
    mBodyNode->attachObject(mBodyEnt);
    //mBodyNode->setVisible(false);
    
    mBodyNode->attachObject(g_SceneMgrPtr->createParticleSystem("smoke", "Examples/Smoke"));
    
    Ogre::Entity* mSword1 = g_SceneMgrPtr->createEntity("SinbadSword1", "Sword.mesh");
    Ogre::Entity* mSword2 = g_SceneMgrPtr->createEntity("SinbadSword2", "Sword.mesh");
    mBodyEnt->attachObjectToBone("Handle.L", mSword1);
    mBodyEnt->attachObjectToBone("Handle.R", mSword2);
    
    
    
    m_pAnim = mBodyEnt->getAnimationState("RunBase");
    m_pAnim->setEnabled(true);
    m_pAnim->setLoop(true);
    
    
    m_pAnimUp = mBodyEnt->getAnimationState("RunTop");
    m_pAnimUp->setEnabled(true);
    m_pAnimUp->setLoop(true);
    
    sCollisionConfig = new btDefaultCollisionConfiguration();
	
	sBroadphase = new btDbvtBroadphase();
	
	sCollisionDispatcher = new btCollisionDispatcher(sCollisionConfig);
	sConstraintSolver = new btSequentialImpulseConstraintSolver;
	sDynamicsWorld = new btDiscreteDynamicsWorld(sCollisionDispatcher,sBroadphase,sConstraintSolver,sCollisionConfig);
	sDynamicsWorld->setGravity(btVector3(0,-10,0));
    
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0,0,0));
    
    btVector3 norm(0,1,0);
    btCollisionShape* groundShape = new btStaticPlaneShape(norm,0.0);
    btScalar mass(0.);	//rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        groundShape->calculateLocalInertia(mass,localInertia);
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
    sFloorPlaneBody = new btRigidBody(rbInfo);
    //add the body to the dynamics world
    sDynamicsWorld->addRigidBody(sFloorPlaneBody);
    
//    {
//        btTransform bodyTransform;
//		bodyTransform.setIdentity();
//		bodyTransform.setOrigin(btVector3(0,5,0));
//        const AxisAlignedBox& boundBox = mBodyEnt->getBoundingBox();
//        Vector3 boundSize = boundBox.getSize();
//		btCollisionShape* boxShape = new btBoxShape(btVector3(boundSize.x / 2,boundSize.y / 2,boundSize.z / 2));
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
//        boxBody->setUserPointer(mBodyNode);
//		
//		//most applications shouldn't disable deactivation, but for this demo it is better.
//		boxBody->setActivationState(DISABLE_DEACTIVATION);
//		//add the body to the dynamics world
//		sDynamicsWorld->addRigidBody(boxBody);
//    }
    
    //m_pCamera->setPolygonMode(PM_WIREFRAME);
    String name = "box";
    for (int i=0;i<numBodies;i++)
	{
        String namenum = name + StringConverter::toString(i);
        Entity* boxEnt = m_pSceneMgr->createEntity(namenum, SceneManager::PT_CUBE);
        Ogre::SceneNode* boxNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,10+i*3,0));
        
        boxEnt->setMaterialName("Examples/Box");
        boxNode->attachObject(boxEnt);
        boxNode->scale(Ogre::Vector3(0.01, 0.01, 0.01));
        
		btTransform bodyTransform;
		bodyTransform.setIdentity();
		bodyTransform.setOrigin(btVector3(0,10+i*3,0));
		btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
		btScalar mass(1.);//positive mass means dynamic/moving  object
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0,0,0);
		if (isDynamic)
            boxShape->calculateLocalInertia(mass,localInertia);
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
		btRigidBody* boxBody=new btRigidBody(rbInfo);
		sBoxBodies.push_back(boxBody);
        
        boxBody->setUserPointer(boxNode);
		
		//most applications shouldn't disable deactivation, but for this demo it is better.
		boxBody->setActivationState(DISABLE_DEACTIVATION);
		//add the body to the dynamics world
		sDynamicsWorld->addRigidBody(boxBody);
	}
    
	return true;
}

void OgreFramework::shoutBox()
{
    static int shoutnum = 0;
    String name = "sbox";

    Vector3 caPos = m_pCamera->getPosition();
    caPos.z -= 3;
    
    Vector3 caOr = m_pCamera->getDirection();
    
        String namenum = name + StringConverter::toString(++shoutnum);
        Entity* boxEnt = m_pSceneMgr->createEntity(namenum, SceneManager::PT_CUBE);
        Ogre::SceneNode* boxNode = g_SceneMgrPtr->getRootSceneNode()->createChildSceneNode(caPos);
        
        boxEnt->setMaterialName("Examples/Box");
        boxNode->attachObject(boxEnt);
        boxNode->scale(Ogre::Vector3(0.01, 0.01, 0.01));
        
		btTransform bodyTransform;
		bodyTransform.setIdentity();
		bodyTransform.setOrigin(btVector3(caPos.x,caPos.y,caPos.z));
		btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
		btScalar mass(1.);//positive mass means dynamic/moving  object
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0,0,0);
		if (isDynamic)
            boxShape->calculateLocalInertia(mass,localInertia);
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,boxShape,localInertia);
		btRigidBody* boxBody=new btRigidBody(rbInfo);
		sBoxBodies.push_back(boxBody);
        
        boxBody->setUserPointer(boxNode);
		
		//most applications shouldn't disable deactivation, but for this demo it is better.
		boxBody->setActivationState(DISABLE_DEACTIVATION);
		//add the body to the dynamics world
		sDynamicsWorld->addRigidBody(boxBody);
    //boxBody->setLinearVelocity(btVector3(0, 0, 3));

    boxBody->setLinearVelocity(btVector3(caOr.x * 10, caOr.y * 10, caOr.z * 10));
    //boxBody->applyCentralForce(btVector3(caOr.x * 100, caOr.y * 100, caOr.z * 100));
	
}

void OgreFramework::transformInputState(OIS::MultiTouchState &state)
{
    int w = m_pRenderWnd->getViewport(0)->getActualWidth();
    int h = m_pRenderWnd->getViewport(0)->getActualHeight();
    int absX = state.X.abs;
    int absY = state.Y.abs;
    int relX = state.X.rel;
    int relY = state.Y.rel;
    
    state.X.abs = absY;
    state.Y.abs = h - absX;
    state.X.rel = relY;
    state.Y.rel = -relX;
    
    UIInterfaceOrientation interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
    switch (interfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
            break;
        case UIInterfaceOrientationLandscapeLeft:
            state.X.abs = w - absY;
            state.Y.abs = absX;
            state.X.rel = -relY;
            state.Y.rel = relX;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            state.X.abs = w - absX;
            state.Y.abs = h - absY;
            state.X.rel = -relX;
            state.Y.rel = -relY;
            break;
        case UIInterfaceOrientationLandscapeRight:
            state.X.abs = absY;
            state.Y.abs = h - absX;
            state.X.rel = relY;
            state.Y.rel = -relX;
            break;
    }
}

OgreFramework::~OgreFramework()
{
    if(m_pInputMgr) OIS::InputManager::destroyInputSystem(m_pInputMgr);
    if(m_pTrayMgr)  delete m_pTrayMgr;
#ifdef OGRE_STATIC_LIB
    m_StaticPluginLoader.unload();
#endif
    if(m_pRoot)     delete m_pRoot;
}

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#if !defined(OGRE_IS_IOS)
	
	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
        m_bShutDownOgre = true;
        return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		m_pRenderWnd->writeContentsToTimestampedFile("BOF_Screenshot_", ".png");
		return true;
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		
		if(mode == 2)
		{
			m_pCamera->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if(mode == 0)
		{
            m_pCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
		}
		else if(mode == 1)
		{
			m_pCamera->setPolygonMode(PM_POINTS);
			mode = 2;
		}
	}
    
	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		if(m_pTrayMgr->isLogoVisible())
        {
            m_pTrayMgr->hideLogo();
            m_pTrayMgr->hideFrameStats();
        }
        else
        {
            m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
        }
	}
    
#endif
	return true;
}

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

#if defined(OGRE_IS_IOS)
bool OgreFramework::touchMoved(const OIS::MultiTouchEvent &evt)
{
    OIS::MultiTouchState state = evt.state;
    transformInputState(state);
    OIS::MultiTouchEvent orientedEvt((OIS::Object*)evt.device, state);
    if(m_pTrayMgr->injectMouseMove(orientedEvt))
    {
        return true;
    }
    
    m_pCamera->yaw(Degree(orientedEvt.state.X.rel * -0.1));
	m_pCamera->pitch(Degree(orientedEvt.state.Y.rel * -0.1));
    

	
	return false;
}

void OgreFramework::windowResized(Ogre::RenderWindow* rw)
{
    if (!m_pTrayMgr) return;
    
    Ogre::OverlayContainer* center = m_pTrayMgr->getTrayContainer(OgreBites::TL_CENTER);
    Ogre::OverlayContainer* left = m_pTrayMgr->getTrayContainer(OgreBites::TL_LEFT);
    
    if (center->isVisible() && rw->getWidth() < 1280 - center->getWidth())
    {
        while (center->isVisible())
        {
            m_pTrayMgr->moveWidgetToTray(m_pTrayMgr->getWidget(OgreBites::TL_CENTER, 0), OgreBites::TL_LEFT);
        }
    }
    else if (left->isVisible() && rw->getWidth() >= 1280 - left->getWidth())
    {
        while (left->isVisible())
        {
            m_pTrayMgr->moveWidgetToTray(m_pTrayMgr->getWidget(OgreBites::TL_LEFT, 0), OgreBites::TL_CENTER);
        }
    }
    

}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::touchPressed(const OIS:: MultiTouchEvent &evt)
{
//#pragma unused(evt)
    OIS::MultiTouchState state = evt.state;
    transformInputState(state);
    OIS::MultiTouchEvent orientedEvt((OIS::Object*)evt.device, state);
    m_pTrayMgr->getCursorContainer()->setPosition(orientedEvt.state.X.abs, orientedEvt.state.Y.abs);
    if(m_pTrayMgr->injectMouseDown(orientedEvt))
    {
        return true;
    }
	return false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::touchReleased(const OIS:: MultiTouchEvent &evt)
{
//#pragma unused(evt)
    OIS::MultiTouchState state = evt.state;
    transformInputState(state);
    OIS::MultiTouchEvent orientedEvt((OIS::Object*)evt.device, state);
    if(m_pTrayMgr->injectMouseUp(orientedEvt))
    {
        return true;
    }
	return false;
}

bool OgreFramework::touchCancelled(const OIS:: MultiTouchEvent &evt)
{
#pragma unused(evt)
	return true;
}
#else
bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
	m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
	
	return true;
}

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}
#endif

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
	m_MoveScale = m_MoveSpeed   * (float)timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * (float)timeSinceLastFrame;

#if OGRE_VERSION >= 0x10800
    m_pSceneMgr->setSkyBoxEnabled(true);
#endif
    
	m_TranslateVector = Vector3::ZERO;
    
	getInput();
	moveCamera();
    
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    m_pAnim->addTime(timeSinceLastFrame * 0.001);
    m_pAnimUp->addTime(timeSinceLastFrame * 0.001);
    //m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
    
    if (sDynamicsWorld)
	{
		sDynamicsWorld->stepSimulation(timeSinceLastFrame * 0.001, 2);//deltaTime);
		{
			static int i=0;
			if (i<10)
			{
				i++;
				CProfileManager::dumpAll();
			}
		}
	}
    
    float worldMat[16];
	for (int i=0;i<sBoxBodies.size();i++)
	{
        const btTransform& trans = sBoxBodies[i]->getCenterOfMassTransform();
        
        SceneNode* node = (SceneNode*)sBoxBodies[i]->getUserPointer();
        node->setPosition(Ogre::Vector3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));
        
        btQuaternion qua = trans.getRotation();
        node->setOrientation(Quaternion(qua.getW(), qua.getX(), qua.getY(), qua.getZ()));
        
        
    }
    
    int nNumManifold = sDynamicsWorld->getDispatcher()->getNumManifolds();
    for(int i = 0;i < nNumManifold; ++i)
    {
        btPersistentManifold* contactManifld = sDynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        
        int numContacts = contactManifld->getNumContacts();
        for (int j = 0; j < numContacts; ++j)
        {
            btManifoldPoint& contactPoint = contactManifld->getContactPoint(j);
        }
        if(numContacts > 0)
        {
            btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifld->getBody0());
            btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifld->getBody1());
            
            SceneNode* nodeA = (SceneNode*)obA->getUserPointer();
            SceneNode* nodeB = (SceneNode*)obB->getUserPointer();
        }
    }
}

void OgreFramework::moveCamera()
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)) 
		m_pCamera->moveRelative(m_TranslateVector);
	else
#endif

		m_pCamera->moveRelative(m_TranslateVector / 10);
}

void OgreFramework::getInput()
{
#if !defined(OGRE_IS_IOS)
	if(m_pKeyboard->isKeyDown(OIS::KC_A))
		m_TranslateVector.x = -m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_D))
		m_TranslateVector.x = m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_W))
		m_TranslateVector.z = -m_MoveScale;
	
	if(m_pKeyboard->isKeyDown(OIS::KC_S))
		m_TranslateVector.z = m_MoveScale;
#endif
}
