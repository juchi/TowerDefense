#include "stdafx.h"

#include "TowerPreview.h"
#include "ConfigElement.h"
#include "Game.h"

TowerPreview::TowerPreview(Game* game, ConfigElement* conf) : GridItem(game)
{
    mValid = true;

    Ogre::SceneManager* sceneMgr = mGame->getSceneMgr();
    mEntity = sceneMgr->createEntity(Game::generateUniqueName("towerPreview"), conf->getFirstChildElement("mesh")->getXmlElement()->Attribute("file"));
    mNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Game::generateUniqueName("towerPreviewNode"));
    mNode->attachObject(mEntity);
    mNode->setVisible(false);

    Ogre::MaterialPtr origMaterialPtr = mEntity->getSubEntity(0)->getMaterial();
    Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().create("TowerPreview", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    origMaterialPtr->copyDetailsTo(materialPtr);

    materialPtr->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
    materialPtr->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);

    mTus = materialPtr->getTechnique(0)->getPass(0)->createTextureUnitState();
    mTus->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue(1.0, 0.0, 0.0));
    mTus->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, 0.5);

    mEntity->getSubEntity(0)->setMaterial(materialPtr);
}

void TowerPreview::setValid(bool valid)
{
    if (mValid != valid) {
        mValid = valid;
        updateColour();
    }
}

void TowerPreview::setVisible(bool visible)
{
    mNode->setVisible(visible);
}

void TowerPreview::updateColour()
{
    Ogre::ColourValue colour;
    if (mValid) {
        colour = Ogre::ColourValue(0.0, 1.0, 0.0);
    } else {
        colour = Ogre::ColourValue(1.0, 0.0, 0.0);
    }
    mTus->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour);
}
