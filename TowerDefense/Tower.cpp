#include "stdafx.h"

#include "Tower.h"
#include "ConfigElement.h"
#include "Enemy.h"
#include "Field.h"
#include "Jinx.h"
#include "Game.h"
#include "GridCell.h"

Tower::Tower(Game *game, ConfigElement* conf) : GridItem(game)
{
    mType = 1;
    mDamages = 0;
    mFireDelay = 1.0;
    mFireCooldown = 0.0;
    mRange = 0.0f;
    mJinxSpeed = 250.0;

    tinyxml2::XMLElement *node = conf->getXmlElement();

    mDamages = atoi(node->Attribute("damages"));
    mFireDelay = Ogre::Real(atoi(node->Attribute("firedelay")));
    mRange = Ogre::Real(atoi(node->Attribute("range")));
    mJinxSpeed = Ogre::Real(atoi(node->FirstChildElement("jinx")->Attribute("speed")));
    Ogre::String meshFile(node->FirstChildElement("mesh")->Attribute("file"));
    float angle = (float)atof(node->FirstChildElement("mesh")->Attribute("angle"));

    Ogre::String name = Game::generateUniqueName(node->Value());
    mEntity = mSceneMgr->createEntity(name, meshFile);
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    mNode->attachObject(mEntity);
    mNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(angle));
}

Ogre::Real Tower::getRange() const
{
    return mRange;
}

void Tower::update(Ogre::Real time)
{
    mFireCooldown += time;
    if (mFireCooldown > mFireDelay) {
        if (fire()) {
            mFireCooldown = 0.0;
        }
    }
}

void Tower::setRangeCells(std::vector<GridCell*> cells)
{
    mRangeCells = cells;
}

GridItem* Tower::getBetterTarget()
{
    std::vector<GridCell*>::iterator itCell;
    std::vector<GridItem*>::iterator itItem;
    std::vector<GridItem*> items;
    for (itCell = mRangeCells.begin(); itCell < mRangeCells.end(); itCell++) {
        if ((*itCell)->getItems().empty()) {
            continue;
        }
        items = (*itCell)->getItems();
        for (itItem = items.begin(); itItem < items.end(); itItem++) {
            if ((*itItem)->getPosition().distance(getPosition()) < mRange) {
                return (*itItem);
            }
        }
    }
    return 0;
}

bool Tower::fire()
{
    //GridItem *item = mGame->getField()->getClosestItem(getGridCoords(), 2);
    GridItem *item = getBetterTarget();

    if (item != 0 && item->getType() == 2) {
        Enemy* enemy = static_cast<Enemy*>(item);

        if (getPosition().distance(enemy->getPosition()) < mRange) {
            Jinx *jinx = new Jinx(mSceneMgr, mGame);
            jinx->setPosition(getPosition());
            jinx->setPower(mDamages);
            jinx->setSpeed(mJinxSpeed);
            jinx->setTarget(enemy);
            return true;
        }
    }
    return false;
}
