#pragma once
#ifndef __Jinx_h_
#define __Jinx_h_

#include <Ogre.h>

class Enemy;
class Game;

class Jinx
{
public:
    Jinx(Ogre::SceneManager *sceneMgr, Game *game);

    void setPosition(const Ogre::Vector3 &position);
    void setPower(const int &power);
    void setSpeed(const Ogre::Real &speed);
    void setTarget(Enemy *target);
    void removeTarget();

    void update(const Ogre::Real time);

    bool isDead() const;
    bool canBeDeleted() const;

protected:
    Game *mGame;
    Ogre::SceneManager *mSceneMgr;

    Ogre::ParticleSystem *mSystem;
    Ogre::SceneNode *mNode;
    Enemy *mTarget;
    Ogre::Vector3 mDestination;

    Ogre::Real mSpeed;
    int mPower;
    bool mDead;
    Ogre::Real mTimeSinceDead;
};

#endif
