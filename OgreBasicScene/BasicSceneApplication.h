#ifndef __BasicSceneApplication_h_
#define __BasicSceneApplication_h_

#include "BaseApplication.h"
#include "SharedMemoryManager.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

class BasicSceneApplication : public BaseApplication
{

private :
	// defined to create a terrain
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    bool mTerrainsImported;
    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
	OgreBites::Label* mInfoLabel;
	void destroyScene(void);


public:
    BasicSceneApplication(void);
    virtual ~BasicSceneApplication(void);

protected:
	// Where we put the scene
    virtual void createScene(void);

	// we create the camera and viewport
	virtual void createCamera(void);
	virtual void createViewports(void);

	// Here I defined something that happen when one frame 
	// is being built : it will look in the shared file, to find
	// the position of head, weapon and give it to the program
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	SharedMemoryManager * mem;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual void createFrameListener(void);

	Ogre::Vector3 * scene_center;
	Ogre::SceneNode* crossbowNode;
	Ogre::SceneNode* ninjaNode;
	static const int distance =500;
};

#endif // #ifndef __BasicSceneApplication_h_
