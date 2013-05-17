#include "BasicAppWithBullet.h"

//-------------------------------------------------------------------------------------
BasicAppWithBullet::BasicAppWithBullet(void)
{
	gravity=Ogre::Vector3(0,-9.81,0);
	alignbox=  Ogre::AxisAlignedBox(Ogre::Vector3 (-10000, -10000, -10000), Ogre::Vector3 (10000,  10000,  10000));
	timeSinceLastBall=Ogre::Real(0.0f);
	timeSinceBeginning=Ogre::Real(0.0f);
	timeSinceLastTarget=Ogre::Real(0.0f);
	score=0;
}
//-------------------------------------------------------------------------------------
BasicAppWithBullet::~BasicAppWithBullet(void)
{
	//delete pointers used by bullet
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
    while (mBodies.end() != itBody)
    {   
        delete *itBody;
        ++itBody;
    }   

    // OgreBullet physic delete - Shapes
    std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
    while (mShapes.end() != itShape)
    {   
        delete *itShape;
        ++itShape;
    }
    mBodies.clear();
    mShapes.clear();
    delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    delete mWorld;
}

//-------------------------------------------------------------------------------------
void BasicAppWithBullet::createScene(void)
{
    Ogre::Entity *ent;
    Ogre::Plane p;
    p.normal = Ogre::Vector3(0,1,0); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
                "FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                p, 200000, 200000, 20, 20, true, 1, 9000, 9000, Ogre::Vector3::UNIT_Z);

    // Create an entity (the floor)
    ent = mSceneMgr->createEntity("floor", "FloorPlane");
    ent->setMaterialName("Examples/GrassFloor");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

    OgreBulletCollisions::CollisionShape *Shape;
    Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0,1,0), 0); // (normal vector, distance)
    OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody(
            "BasePlane",
            mWorld);
    defaultPlaneBody->setStaticShape(Shape, 0.1, 0.8); // (shape, restitution, friction)
	
    // push the created objects to the deques
    mShapes.push_back(Shape);
    mBodies.push_back(defaultPlaneBody);

	// build the target
	Entity *target = mSceneMgr->createEntity(
        "Target",
        "target.mesh");            
    target->setCastShadows(true);
    target->setMaterialName("Examples/TargetS");
    nodeTarget = mSceneMgr->getRootSceneNode()->createChildSceneNode("NodeTarget");
    nodeTarget->attachObject(target);
	nodeTarget->setPosition(0,10,0);
	nodeTarget->rotate(Ogre::Quaternion(0.7,0.7,0,0));
	// assign a dynamic to the target
	OgreBulletCollisions::StaticMeshToShapeConverter shapeConverter= OgreBulletCollisions::StaticMeshToShapeConverter(target);
	OgreBulletCollisions::CollisionShape *sceneTargetShape =0;
	sceneTargetShape=shapeConverter.createConvex();
	//OgreBulletCollisions::BoxCollisionShape *ccs=new OgreBulletCollisions::BoxCollisionShape(Vector3(2,1,1));
	//sceneTargetShape=ccs;
    targetBody = new OgreBulletDynamics::RigidBody(
                   "TargetShape",
                   mWorld);
    targetBody->setShape(nodeTarget,
                          sceneTargetShape,
                               0.3f,         // dynamic body restitution
                               0.9f,         // dynamic body friction
                               0.0f,          // dynamic bodymass
							   Ogre::Vector3(0,10,0),    // starting position of the box
                               Quaternion(0.7,0.7,0,0));// orientation of the box
	targetBody->enableActiveState();
    targetBody->setKinematicObject(true);
    targetBody->disableDeactivation();
	mNumEntitiesInstanced++;    
	mShapes.push_back(sceneTargetShape);
    mBodies.push_back(targetBody);  

	
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
	
	// create a sky
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
}

void BasicAppWithBullet::createCamera(void){
	BaseApplication::createCamera();
	mCamera->setPosition(Ogre::Vector3(0,10,30));
	mCamera->lookAt(Ogre::Vector3(0,10,0));
}

void BasicAppWithBullet::createFrameListener(void){
	BaseApplication::createFrameListener();
	mNumEntitiesInstanced = 0; // how many shapes are created

	// Start Bullet
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, alignbox , gravity);

	// add Debug info display tool
	debugDrawer = new OgreBulletCollisions::DebugDrawer();
	debugDrawer->setDrawWireframe(true);   // we want to see the Bullet containers
	mWorld->setDebugDrawer(debugDrawer);
	mWorld->setShowDebugShapes(true);      // enable it if you want to see the Bullet containers
	Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
	node->attachObject(static_cast <Ogre::SimpleRenderable *> (debugDrawer));
	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);

}

bool BasicAppWithBullet::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
    if(!processUnbufferedInput(evt)) return false;
    return ret;
}

bool BasicAppWithBullet::frameStarted(const FrameEvent& evt)
       {
          bool ret = BaseApplication::frameStarted(evt);
          mWorld->stepSimulation(evt.timeSinceLastFrame);   // update Bullet Physics animation

		  timeSinceBeginning+=evt.timeSinceLastFrame;
		  timeSinceLastTarget+=evt.timeSinceLastFrame;
		  timeSinceLastBall+=evt.timeSinceLastFrame;
		  if (timeSinceLastBall>0.33){
			  timeSinceLastBall=0.0f;
			  times_to_live.push_back(PointTTL(mNumEntitiesInstanced,timeSinceBeginning));
			  throwBall();
		  }

		  deleteOldBall(timeSinceBeginning);
		  

		  if(timeSinceLastTarget>0.15 && timeSinceBeginning>1){
			  for (int i=0;i<times_to_live.size();i++){
				  	String nameNode="SphereNode"+StringConverter::toString(times_to_live[i].id);
					Vector3 pos=mSceneMgr->getSceneNode(nameNode)->getPosition();
					if (pos.z>0 && pos.z<3){
						Vector3 ecart=pos-nodeTarget->getPosition();
						ecart.z=0;
						double dist=ecart.length();
						if (dist<2)
							score+=10-5*dist;
						timeSinceLastTarget=0;
					}

			  }

			  timeSinceLastTarget=0;
		  }

		  	btTransform btt=targetBody->getBulletRigidBody()->getWorldTransform();
			btt.setOrigin(btt.getOrigin()+btVector3(0.02*Math::Cos(timeSinceBeginning/2),0,0));
			targetBody->getBulletRigidBody()->getMotionState()->setWorldTransform(btt);
			if (timeSinceBeginning>1){
			String nameNode="SphereNode"+StringConverter::toString(times_to_live[0].id);
			
			mInfoLabel->setCaption(StringConverter::toString(Ogre::Real(score)));
			}
			mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
			mInfoLabel->show();
		  return ret;
       }
 
 bool BasicAppWithBullet::frameEnded(const FrameEvent& evt)
       {
          bool ret = BaseApplication::frameEnded(evt);
          mWorld->stepSimulation(evt.timeSinceLastFrame);   // update Bullet Physics animation
          return ret;
		  if(timeSinceLastTarget>0.15 && timeSinceBeginning>1){
			  for (int i=0;i<times_to_live.size();i++){
				  	String nameNode="SphereNode"+StringConverter::toString(times_to_live[i].id);
					Vector3 pos=mSceneMgr->getSceneNode(nameNode)->getPosition();
					if (pos.z>0 && pos.z<3){
						Vector3 ecart=pos-nodeTarget->getPosition();
						ecart.z=0;
						double dist=ecart.length();
						if (dist<2)
							score+=10-5*dist;
						timeSinceLastTarget=0;
					}

			  }

			  timeSinceLastTarget=0;
		  }
       }
bool BasicAppWithBullet::processUnbufferedInput(const Ogre::FrameEvent& evt){
          if(mKeyboard->isKeyDown(OIS::KC_B) ){
       
          }
          return true;
}

void BasicAppWithBullet::throwBall(){
	// This function will throw a ball. 
	// It consists in :
	// creating a mesh called Sphere+num_id, attached to a node callde SpereNode+num_id
	// creating a DynamicShape without name
	// a rigid body called SphereRIgid
	//
			Vector3 size = Vector3::ZERO;   // size of the box
             // starting position of the ball
			Vector3 position = (mCamera->getDerivedPosition() - mCamera->getDerivedUp());
             
			 // create an ordinary, Ogre mesh with texture
             Entity *entity = mSceneMgr->createEntity(
                   "Sphere" + StringConverter::toString(mNumEntitiesInstanced),
                   "rock.mesh");            
             entity->setCastShadows(true);
			 
             
			 // we need the bounding box of the box to be able to set the size of the Bullet-box
             AxisAlignedBox boundingB = entity->getBoundingBox();
			 size = boundingB.getHalfSize(); 
             size *= 0.96f;   // Bullet margin is a bit bigger so we need a smaller size
                               // (Bullet 2.76 Physics SDK Manual page 18)
             entity->setMaterialName("Examples/RockySpherical");
             SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("SphereNode" + StringConverter::toString(mNumEntitiesInstanced));
             node->attachObject(entity);
			 AxisAlignedBox bb=entity->getBoundingBox();
			 
			 float factor=2;
             node->scale(factor, factor, factor);   // the cube is too big for us
             size *= factor;   
			 // don't forget to scale down the Bullet-box too
             // after that create the Bullet shape with the calculated size
			 OgreBulletCollisions::StaticMeshToShapeConverter shapeConverter= OgreBulletCollisions::StaticMeshToShapeConverter(entity);
			 
			 OgreBulletCollisions::CollisionShape *sceneBoxShape;
			 sceneBoxShape= shapeConverter.createConvex();
             // and the Bullet rigid body
             OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
                   "SphereRigid" + StringConverter::toString(mNumEntitiesInstanced),
                   mWorld);
             defaultBody->setShape(   node,
                               sceneBoxShape,
                               0.1f,         // dynamic body restitution
                               0.9f,         // dynamic body friction
                               1.5f,          // dynamic bodymass
                               position,      // starting position of the box
                               Quaternion(0,0,0,1));// orientation of the box
			 
             mNumEntitiesInstanced++;    
			 
             defaultBody->setLinearVelocity(
                      mCamera->getDerivedDirection().normalisedCopy() * 40.0f ); // shooting speed
                   // push the created objects to the deques
			 
             mShapes.push_back(sceneBoxShape);
             mBodies.push_back(defaultBody);     
}

void BasicAppWithBullet::deleteOldBall(Ogre::Real current_time){
	Ogre::Real life=Ogre::Real(10.0f);
	while(times_to_live.size()>0 && current_time-times_to_live[0].date_creation>life){
		/*
		
		mSceneMgr->destroyEntity("Sphere"+StringConverter::toString(times_to_live[0].id));
		*/
		// we look through the list of shape and rigid body to find the one that correspond to this ball and delete it
		for (int i=0;i<mBodies.size();i++)
		{   
			Ogre::String rbname=mBodies[i]->getName();
			if (rbname.compare("SphereRigid"+StringConverter::toString(times_to_live[0].id))==0){

				delete mBodies[i];
				delete mShapes[i];
				mBodies.erase(mBodies.begin()+i);
				mShapes.erase(mShapes.begin()+i);
				break;
			}
		}
		String nameNode="SphereNode"+StringConverter::toString(times_to_live[0].id);
		(mSceneMgr->getSceneNode(nameNode))->detachAllObjects();
		//mSceneMgr->destroySceneNode(nameNode);
		

			times_to_live.pop_front();
	}
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
        BasicAppWithBullet app;

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
