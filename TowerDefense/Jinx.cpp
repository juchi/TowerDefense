#include "stdafx.h"

#include "Enemy.h"
#include "Jinx.h"
#include "Game.h"

Jinx::Jinx(Ogre::SceneManager *sceneMgr, Game *game) : mSceneMgr(sceneMgr), mGame(game), mTarget(0), mSpeed(0.0), mPower(0), mDead(false), mTimeSinceDead(0.0)
{
    Ogre::String name = Game::generateUniqueName("jinx");
    mSystem = mSceneMgr->createParticleSystem(name, "Tower/Jinx");
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mNode->attachObject(mSystem);
    mGame->registerJinx(this);
}

void Jinx::setPower(const int &power)
{
    mPower = power;
}

void Jinx::setSpeed(const Ogre::Real &speed)
{
    mSpeed = speed;
}

void Jinx::setPosition(const Ogre::Vector3 &position)
{
    mNode->setPosition(position);
}

void Jinx::setTarget(Enemy *target)
{
    // forbidden to reassign new target
    if (mTarget == 0) {
        mTarget = target;
        mTarget->addTracker(this);
    }
}

void Jinx::removeTarget()
{
    mTarget = 0;
}

void Jinx::update(const Ogre::Real time)
{
    if (mDead) {
        mTimeSinceDead += time;
        return;
    }
    if (mTarget) {
        mDestination = mTarget->getPosition();
    }
    Ogre::Vector3 direction = mDestination - mNode->getPosition();
    Ogre::Real distance = direction.normalise();

    Ogre::Vector3 move = direction * time * mSpeed;
    mNode->translate(move);

    if (distance < 1.0f) {
        if (mTarget) {
            mTarget->takeDamage(mPower);
        }
        mSystem->setEmitting(false);
        mDead = true;
    }
}

bool Jinx::canBeDeleted() const
{
    return mTimeSinceDead > 2.0;
}

bool Jinx::isDead() const
{
    return mDead;
}
