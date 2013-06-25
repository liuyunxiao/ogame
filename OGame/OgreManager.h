//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_FRAMEWORK_H
#define OGRE_FRAMEWORK_H



#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// dx10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  endif
#  if OGRE_VERSION >= 0x10800
#    if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#      define OGRE_IS_IOS 1
#    endif
#  else
#    if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#      define OGRE_IS_IOS 1
#    endif
#  endif
#  ifdef OGRE_IS_IOS
#    undef OGRE_STATIC_CgProgramManager
#    undef OGRE_STATIC_GL
#    define OGRE_STATIC_GLES 1
#    ifdef OGRE_USE_GLES2
#      define OGRE_STATIC_GLES2 1
#      define USE_RTSHADER_SYSTEM
#      undef OGRE_STATIC_GLES
#    endif
#    ifdef __OBJC__
#      import <UIKit/UIKit.h>
#    endif
#  endif
#  include "OgreStaticPluginLoader.h"
#endif



//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreMgr : public Singleton<OgreMgr>
{
public:
	OgreMgr();
	~OgreMgr();
    
    bool Init(String wndTitle);
	void Update(double timeSinceLastFrame);
    
	bool isOgreToBeShutDown()const{return m_bShutDownOgre;}
	Root*                   m_pRoot;
	SceneManager*			m_pSceneMgr;
	RenderWindow*			m_pRenderWnd;
	Camera*                 m_pCamera;
	Viewport*				m_pViewport;
	Log*                    m_pLog;
	Timer*                  m_pTimer;
    
protected:
   // Added for Mac compatibility
   String                   m_ResourcePath;
    
private:
	OgreMgr(const OgreMgr&);
	OgreMgr& operator= (const OgreMgr&);

    FrameEvent              m_FrameEvent;
	int                     m_iNumScreenShots;
    
	bool                    m_bShutDownOgre;
	
	Vector3                 m_TranslateVector;
	Real                    m_MoveSpeed; 
	Degree                  m_RotateSpeed; 
	float                   m_MoveScale; 
	Degree                  m_RotScale;
#ifdef OGRE_STATIC_LIB
    StaticPluginLoader      m_StaticPluginLoader;
#endif
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 
#define g_pSceneMgrPtr OgreMgr::getSingletonPtr()->m_pSceneMgr
#define g_pOgreFramePtr OgreMgr::getSingletonPtr()
//|||||||||||||||||||||||||||||||||||||||||||||||
