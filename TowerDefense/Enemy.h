#pragma once
#ifndef __Enemy_h_
#define __Enemy_h_

#include "GridItem.h"

struct EnemyType
{
    Ogre::String name;
    Ogre::String mesh;
    int health;
    int power;
    int moneyDrop;
    Ogre::Real speed;
};

class Jinx;

class Enemy : public GridItem
{
public:
    Enemy(EnemyType type, Game *game);
    virtual ~Enemy();

    void addTracker(Jinx *jinx);
    void removeFromTrackers();

    void setDestination(Ogre::Vector3 destination);

    bool canBeDeleted() const;
    bool isArrived() const;
    int getMoneyDrop() const;

    void takeDamage(int damages);
    virtual void update(Ogre::Real time);
    void walk(Ogre::Real time);

    void setPath(std::vector<Ogre::Vector3> path);
    void pushCheckpoint(Ogre::Vector3 point);

protected:
    void processNewDirection();

    std::vector<Jinx*> mTrackers;

    Ogre::AnimationState *mAnimationState;

    int mDamages;
    int mMoneyDrop;
    int mHealth;
    bool mAlive;
    bool mIsArrived;

    Ogre::Real mSpeed;
    Ogre::Vector3 mDestination;
    Ogre::Vector3 mDirection;
    Ogre::Real mDistance;

    std::queue<Ogre::Vector3> mPath;
};

#endif
