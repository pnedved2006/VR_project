/*
-----------------------------------------------------------------------------
Filename:    BasicAppWithBullet.h
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.8.x Application Wizard for VC10 (May 2012)
      https://bitbucket.org/jacmoe/ogreappwizards
-----------------------------------------------------------------------------
*/
#ifndef __BasicAppWithBullet_h_
#define __BasicAppWithBullet_h_

#include "BaseApplication.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif
#include "OgreBulletDynamicsRigidBody.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btMotionState.h"
#include "SharedMemoryManager.h"
#include "iostream"
#include <WinBase.h>

class PointTTL{
public : 
	int id;
	Ogre::Real date_creation;

	PointTTL(int id_c, Ogre::Real date_creation_c){
		id=id_c;
		date_creation=date_creation_c;
	}
};

class BasicAppWithBullet : public BaseApplication
{
public:
    BasicAppWithBullet(void);
    virtual ~BasicAppWithBullet(void);

protected:
	FrameListener * mFrameListener;
    virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual void createCamera(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);
	void throwBall(void);

private :
	// variables used with bullet
	Ogre::Vector3 gravity;
	Ogre::AxisAlignedBox alignbox;
	OgreBulletDynamics::DynamicsWorld *mWorld;   // OgreBullet World
    OgreBulletCollisions::DebugDrawer *debugDrawer;
    int mNumEntitiesInstanced;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	
	// used to create the ball thrower
	Ogre::Real timeSinceLastBall;
	Ogre::Real timeSinceBeginning;
	std::deque<PointTTL> times_to_live;
	void deleteOldBall(Ogre::Real current_time);

	// the node on which is attached the target
	Ogre::SceneNode *nodeTarget;
	OgreBulletDynamics::RigidBody *targetBody;
	Ogre::Real timeSinceLastTarget;

	// ceci est utile pour la computation du score
	OgreBites::Label* mInfoLabel;
	double score;

	// the node of the crossbow. 
	Ogre::Vector3 crossbowDirection;
	Ogre::SceneNode * crossbowNode;

	// And finaly this is used to make the landscape
	Ogre::Vector3 *scene_center;

	// finally the shared memory manager
	SharedMemoryManager * mem;
};

#endif // #ifndef __BasicAppWithBullet_h_
