#include "stdafx.h"

#include "GridItem.h"
#include "Game.h"

GridItem::GridItem(Game *game) : mGame(game), mType(0)
{
    mSceneMgr = mGame->getSceneMgr();
    setGridCoords(-1, -1);
}

GridItem::~GridItem()
{
    mNode->detachObject(mEntity);
    mSceneMgr->destroyEntity(mEntity);
    mSceneMgr->destroySceneNode(mNode);
}

void GridItem::setPosition(Ogre::Vector3 position)
{
    Ogre::AxisAlignedBox bb = mEntity->getBoundingBox();
    Ogre::Vector3 center = bb.getCenter();
    mNode->setPosition(position + (bb.getSize().y / 2 - center.y) * Ogre::Vector3::UNIT_Y);
}

Ogre::Vector3 GridItem::getPosition()
{
    return mNode->getPosition();
}

int GridItem::getType()
{
    return mType;
}

Ogre::String GridItem::getName()
{
    return mEntity->getName();
}

std::pair<int, int> GridItem::getGridCoords()
{
    return mCoords;
}

void GridItem::setGridCoords(int x, int y)
{
    std::pair<int, int> coords(x, y);
    setGridCoords(coords);
}

void GridItem::setGridCoords(std::pair<int, int> coords)
{
    mCoords = coords;
}