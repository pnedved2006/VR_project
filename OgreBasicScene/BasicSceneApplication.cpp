#include "BasicSceneApplication.h"


//-------------------------------------------------------------------------------------
BasicSceneApplication::BasicSceneApplication(void)
{
	mem = new SharedMemoryManager();
	scene_center=new Ogre::Vector3(2000,50,1700);
	time=0;
	amplitude =300;
}
//-------------------------------------------------------------------------------------
BasicSceneApplication::~BasicSceneApplication(void)
{
	delete mem;
}

/*
Look for a terrain picture
copied from tutorial
*/
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
 
}

//-------------------------------------------------------------------------------------
void BasicSceneApplication::createScene(void)
{
	
    // Create a directional and ambient light
	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();
    Ogre::Light* light = mSceneMgr->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

	// create the terrain
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("BasicSceneTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	// default configuration of the terrain, we give it the directional light
	configureTerrainDefaults(light);

	// We set the terrain to be loaded at start
	// Note if we had several terrain, we would have a loop there
	defineTerrain(0, 0);
    mTerrainGroup->loadAllTerrains(true);

	// define the blend map (???) of every terrain created
	if (mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

	// free temporary ressources if any
	mTerrainGroup->freeTemporaryResources();

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
    
	


	 // Create Crossbow
	Ogre::Entity* crossbow= mSceneMgr->createEntity("Crossbow", "crossbow.mesh");
	crossbowNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CrossbowNode", *scene_center+Ogre::Vector3(0,-25,450) );
	crossbowNode->attachObject( crossbow);
	crossbowNode->scale(Ogre::Vector3(5,5,5));

	// Create Ninja
		Ogre::Entity* entNinja = mSceneMgr->createEntity("ninja", "ninja.mesh");
		ninjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode", (*scene_center) + Ogre::Vector3(-500,0,-700));
		ninjaNode->attachObject(entNinja);

	const int nb_grass =100;
	const int nb_tree =4;
	const int nb_bird=10;
	//Generating birds
	
	
	Ogre::SceneNode* birdANode[nb_bird];
	Ogre::Entity* entBirdA[nb_bird];
	Ogre::SceneNode* birdBNode[nb_bird];
	Ogre::Entity* entBirdB[nb_bird];
	Ogre::SceneNode* birdCNode[nb_bird];
	Ogre::Entity* entBirdC[nb_bird];
	Ogre::SceneNode* birdDNode[nb_bird];
	Ogre::Entity* entBirdD[nb_bird];
	Ogre::SceneNode* birdENode[nb_bird];
	Ogre::Entity* entBirdE[nb_bird];
	int height =80;
	int width =0;
	int depth =0;
	int h ;
	int w ;
	int d ;

	for(int j=0; j<nb_bird; j++){
		Ogre::String num = Ogre::StringConverter::toString(j);
		h = height + (rand()%20);
		w = width + (int)pow((double)(-1),j)*(rand()%60);
		d = depth + 50*(4+(rand()%5));
		entBirdA[j] = mSceneMgr->createEntity("oiA"+num, "birdsdove_Figur.mesh");
		birdANode[j] = mSceneMgr->getRootSceneNode()->createChildSceneNode("birdANode"+num,*scene_center+Ogre::Vector3(w,h,d));
		birdANode[j]->attachObject(entBirdA[j]);

		h = height + (rand()%20);
		w = width + (int)pow((double)(-1),j)*(rand()%60);
		d = depth + 50*(4+(rand()%5));
		entBirdB[j] = mSceneMgr->createEntity("oiB"+num, "birdscanadian_g.mesh");
		birdBNode[j] = mSceneMgr->getRootSceneNode()->createChildSceneNode("birdBNode"+num,*scene_center+Ogre::Vector3(w,h,d));
		birdBNode[j]->attachObject(entBirdB[j]);

		h = height + (rand()%20);
		w = width + (int)pow((double)(-1),j)*(rand()%60);
		d = depth + 50*(4+(rand()%5));
		entBirdC[j] = mSceneMgr->createEntity("oiC"+num, "birdsmallard_Fi.mesh");
		birdCNode[j] = mSceneMgr->getRootSceneNode()->createChildSceneNode("birdCNode"+num,*scene_center+Ogre::Vector3(w,h,d));
		birdCNode[j]->attachObject(entBirdC[j]);

		h = height + (rand()%20);
		w = width + (int)pow((double)(-1),j)*(rand()%60);
		d = depth + 50*(4+(rand()%5));
		entBirdD[j] = mSceneMgr->createEntity("oiD"+num, "birdsbald_eagle.mesh");
		birdDNode[j] = mSceneMgr->getRootSceneNode()->createChildSceneNode("birdDNode"+num,*scene_center+Ogre::Vector3(w,h,d));
		birdDNode[j]->attachObject(entBirdD[j]);

		h = height + (rand()%20);
		w = width + (int)pow((double)(-1),j)*(rand()%60);
		d = depth + 50*(4+(rand()%5));
		entBirdE[j] = mSceneMgr->createEntity("oiE"+num, "birdsgull_Figur.mesh");
		birdENode[j] = mSceneMgr->getRootSceneNode()->createChildSceneNode("birdENode"+num,*scene_center+Ogre::Vector3(w,h,d));
		birdENode[j]->attachObject(entBirdE[j]);
	}

	

	// generating Grasses
	
	
	Ogre::SceneNode* grassNode[nb_grass];
	Ogre::Entity* entGrass[nb_grass];

	Ogre::Entity* entTree[nb_tree];
	Ogre::SceneNode* treeNode[nb_tree];
	
	
	int l=0;
	for(int k=0; k<nb_grass; k++){

	int w = (int)pow((double)(-1),k)*(rand()%200);
		int h= -5;
		int d = 10 + (rand()%450);
		Ogre::String num = Ogre::StringConverter::toString(k);
		entGrass[k] = mSceneMgr->createEntity("grass"+num, "birdsobjObject0.mesh");
		grassNode[k] = mSceneMgr->getRootSceneNode()->createChildSceneNode("grassNode"+num,*scene_center+Ogre::Vector3(w,h,d));
		grassNode[k]->attachObject(entGrass[k]);
	}

	// Create trees
	for (int l=0; l<nb_tree; l++){
		int w = (int)pow((double)(-1),l)*(rand()%200);
		int h= 5;
		int d = 10 + (rand()%450);
		Ogre::String num = Ogre::StringConverter::toString(l);
			entTree[l] = mSceneMgr->createEntity("tree"+num, "birdsMtree1.mesh");
			treeNode[l] = mSceneMgr->getRootSceneNode()->createChildSceneNode("TreeNode"+num,*scene_center+Ogre::Vector3(w,h,d));
			treeNode[l]->attachObject(entTree[l]);
			
	}

}

void BasicSceneApplication::createCamera(){
	// create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // set its position, direction  
	mCamera->setPosition(*scene_center+Ogre::Vector3(0,0,500));
    mCamera->lookAt(*scene_center);
    // set the near clip distance
    mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(5000);
	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    {
        mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicSceneApplication::createViewports(){
	// Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));    
}

/*
Callback function called at the beginning of every frame rendering
In our case, it  looks for the position of the face, and theweapon, and move
the camera and the weapon according to it
*/
bool BasicSceneApplication::frameStarted(const Ogre::FrameEvent& evt){
	double * pos_head=mem->readHeadPosition();
	// we specify the camera to follow head
	mCamera->setPosition(*scene_center+Ogre::Vector3(0,0,500)+Ogre::Vector3(pos_head[0]*350,pos_head[1]*100+50,0));
	mCamera->lookAt(*scene_center);
	

	//get the referential of the camera
	Ogre::Vector3 cam_pos=mCamera->getRealPosition();
	Ogre::Vector3 cam_dir=mCamera->getRealDirection();
	Ogre::Quaternion cam_rot=mCamera->getRealOrientation();
	Ogre::Vector3 cam_up=mCamera->getRealUp();

	time=time +evt.timeSinceLastFrame+1;
	//mCamera->setPosition(Ogre::Vector3(sin(time/3)*amplitude,0,0));
	ninjaNode->setPosition((*scene_center)+Ogre::Vector3(sin(double(time/100))*amplitude,0,-300));
	
	// positionate the crossbow (for the moment in the center of the screen)
	crossbowNode->setPosition(cam_pos-cam_up+3*cam_dir);
	crossbowNode->setOrientation(cam_rot);
	crossbowNode->rotate(Ogre::Quaternion(sqrt(0.5),0,-sqrt(0.5),0));
	double * quat=mem->readCrossBowOrientation();
	Ogre::Quaternion quat2=Ogre::Quaternion(quat[0],quat[1],quat[2],quat[3]);
	crossbowNode->setOrientation(quat2);
	return true;
}

/*
Give general parameters of the terrain. 
Copied from tutorial
*/
void BasicSceneApplication::configureTerrainDefaults(Ogre::Light* light){
	// global settings for the terrain (see basic tutorial 3 for understanding)
	mTerrainGlobals->setMaxPixelError(8);
    mTerrainGlobals->setCompositeMapDistance(3000);
	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	// Configure default import settings for if we use imported image
	// Sam : personaly, I haven't understand anything there, I have just copied
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600; // due terrain.png is 8 bpp
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

	// add textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

/*
Copied from tutorial
*/
void BasicSceneApplication::defineTerrain(long x, long y)
{
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
    {
        mTerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

/*
This function uses the three textures that we gave in configure
TerrainDefault to create hight, and texture. I've just copied it from tutorial
*/
void BasicSceneApplication::initBlendMaps(Ogre::Terrain* terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

void BasicSceneApplication::createFrameListener(void)
{
    BaseApplication::createFrameListener();
 
    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
}

bool BasicSceneApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
 
    if (mTerrainGroup->isDerivedDataUpdateInProgress())
    {
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported)
        {
            mInfoLabel->setCaption("Building terrain, please wait...");
        }
        else
        {
            mInfoLabel->setCaption("Updating textures, patience...");
        }
    }
    else
    {
        mTrayMgr->removeWidgetFromTray(mInfoLabel);
        mInfoLabel->hide();
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }
 
    return ret;
}

void BasicSceneApplication::destroyScene(void)
{
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        BasicSceneApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
