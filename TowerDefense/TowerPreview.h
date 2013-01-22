#pragma once
#ifndef __TowerPreview_h_
#define __TowerPreview_h_

#include "GridItem.h"

class ConfigElement;

class TowerPreview : public GridItem
{
public:
    TowerPreview(Game* game, ConfigElement* conf);

    virtual void update(Ogre::Real time){}

    void setValid(bool valid);
    void setVisible(bool visible);
    void updateColour();

protected:
    bool mValid; // can the tower be built or not (defines the color)
    Ogre::TextureUnitState* mTus;
};

#endif
