#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreTextureManager.h>
#include <OgreResourceManager.h>
#include <OgreWindowEventUtilities.h>
#include "Compositor\OgreCompositorManager2.h"


using namespace std;

#define v4 Ogre::Vector4
#define v3 Ogre::Vector3
#define v2 Ogre::Vector2



int main()
{
	string pluginsf = "Resource/plugins.cfg";
	string resourcesf = "Resource/resources.cfg";

	// Bare basics

	Ogre::Root* root = new Ogre::Root(pluginsf);
	Ogre::RenderWindow* window;
	Ogre::SceneManager* sceneManager;

	Ogre::ConfigFile cf;
	cf.load(resourcesf);


	// Load Config
	/*
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
	*/

	// Initialise and start windows

	if (root->restoreConfig() || root->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		window = root->initialise(true, "TinyOgre Render Window");
	}
	else
	{
		return 1;
	}


	// Create scene manager

	sceneManager = root->createSceneManager(Ogre::ST_GENERIC, 2, 
		Ogre::InstancingThreadedCullingMethod::INSTANCING_CULLING_SINGLETHREAD);

	Ogre::SceneNode* headNode = sceneManager->getRootSceneNode()->createChildSceneNode();


	Ogre::Camera* mCamera = sceneManager->createCamera("Main Camera");

	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0, 5, 15));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(0.2f);
	mCamera->setFarClipDistance(1000.0f);
	mCamera->setAutoAspectRatio(true);

	Ogre::CompositorManager2* compManager = root->getCompositorManager2();


	
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	
	

	sceneManager->setAmbientLight(Ogre::ColourValue::Blue, Ogre::ColourValue::Black, v3(0, 1, 0), 1.0f);

	while (true)
	{
		Ogre::WindowEventUtilities::messagePump();

		if (window->isClosed())
		{
			return 0;
		}

		if (!root->renderOneFrame())
		{
			return 1;
		}
	}

	return 0;

}

