#pragma once
#ifndef __Tower_h_
#define __Tower_h_

#include "GridItem.h"

class ConfigElement;
class GridCell;

class Tower : public GridItem
{
public:
    Tower(Game *mGame, ConfigElement* conf);

    Ogre::Real getRange() const;

    virtual void update(Ogre::Real time);
    bool fire();

    GridItem* getBetterTarget();
    void setRangeCells(std::vector<GridCell*> cells);

protected:

    Ogre::Real mFireDelay;
    Ogre::Real mFireCooldown;
    Ogre::Real mRange;
    int mDamages;
    Ogre::Real mJinxSpeed;

    std::vector<GridCell*> mRangeCells;
};

#endif;
