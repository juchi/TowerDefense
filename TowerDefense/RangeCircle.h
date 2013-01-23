#pragma once
#ifndef __RangeCircle_h_
#define __RangeCircle_h_

class Game;

class RangeCircle
{
public:
    RangeCircle(Game* game);
    ~RangeCircle();

    void setPosition(Ogre::Vector3 pos);
    void setRadius(Ogre::Real radius);
    void setVisible(bool visible);

protected:
    Ogre::Real mRadius;
    Ogre::ManualObject* mManual;
    Ogre::SceneNode* mNode;
    Ogre::SceneManager* mSceneMgr;

    Game* mGame;
};

#endif;
