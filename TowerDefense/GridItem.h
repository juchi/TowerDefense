#pragma once
#ifndef __GridItem_h_
#define __GridItem_h_

#include <Ogre.h>

class Game;

class GridItem
{
public:
    GridItem(Game *game);
    virtual ~GridItem();

    virtual void setPosition(Ogre::Vector3 position);
    Ogre::Vector3 getPosition();
    int getType();
    virtual void update(Ogre::Real time) = 0;

    Ogre::String getName();
    std::pair<int, int> getGridCoords();
    void setGridCoords(int x, int y);
    void setGridCoords(std::pair<int, int> coords);

protected:
    Game *mGame;

    Ogre::Entity *mEntity;
    Ogre::SceneNode *mNode;
    Ogre::SceneManager *mSceneMgr;

    std::pair<int, int> mCoords;

    int mType;
};

#endif
