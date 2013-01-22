#include "stdafx.h"

#include "Enemy.h"
#include "Jinx.h"
#include "Game.h"
#include "Field.h"

Enemy::Enemy(EnemyType type, Game *game) : GridItem(game)
{
    mType = 2;
    mDistance = 0.0;
    mDestination = Ogre::Vector3::ZERO;
    mDirection = Ogre::Vector3::ZERO;
    mAlive = true;
    mIsArrived = false;

    mDamages = type.power;
    mHealth = type.health;
    mMoneyDrop = type.moneyDrop;
    mSpeed = type.speed;

    Ogre::String name = Game::generateUniqueName(type.name);
    mEntity = mSceneMgr->createEntity(name, type.mesh);
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
    mNode->attachObject(mEntity);
    //mNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(180));

    mAnimationState = mEntity->getAnimationState("Walk");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);
}

Enemy::~Enemy()
{
}

int Enemy::getMoneyDrop() const
{
    return mMoneyDrop;
}

void Enemy::addTracker(Jinx* jinx)
{
    mTrackers.push_back(jinx);
}

void Enemy::removeFromTrackers()
{
    std::vector<Jinx*>::iterator it;
    for (it = mTrackers.begin(); it < mTrackers.end(); it++) {
        (*it)->removeTarget();
    }
    mTrackers.clear();
}

void Enemy::takeDamage(const int damages)
{
    if (damages < 0 || mAlive == false) {
        return;
    }
    mHealth -= damages;

    if (mHealth <= 0) {
        removeFromTrackers();
        mAlive = false;
        mGame->getField()->removeObject(mCoords);
        mGame->playerKill(this);
        mAnimationState = mEntity->getAnimationState("Die");
        mAnimationState->setLoop(false);
        mAnimationState->setEnabled(true);
    }
}

void Enemy::pushCheckpoint(Ogre::Vector3 point)
{
    mPath.push(point);
}

void Enemy::setPath(std::vector<Ogre::Vector3> path)
{
    while (!path.empty()) {
        pushCheckpoint(path.front());
        path.erase(path.begin());
    }
}

void Enemy::setDestination(Ogre::Vector3 destination)
{
    mDestination = destination;
    processNewDirection();
}

void Enemy::processNewDirection()
{
    mDirection = mDestination - mNode->getPosition();
    mDistance = mDirection.normalise();
    Ogre::Vector3 origin = mNode->getOrientation() * Ogre::Vector3::UNIT_X;

    if (1.0f + origin.dotProduct(mDirection) < 0.001f) {
        mNode->yaw(Ogre::Degree(180));
    } else {
        mNode->rotate(origin.getRotationTo(mDirection));
    }
}

void Enemy::update(Ogre::Real time)
{
    mAnimationState->addTime(time);

    if (mAlive) {
        walk(time);
    }
}

void Enemy::walk(Ogre::Real time)
{
    if (mDistance < 0.001f) {
        if (mPath.empty()) {
            mIsArrived = true;
            removeFromTrackers();
            mAlive = false;
            mAnimationState->setLoop(false);
            mGame->getField()->removeObject(mCoords);
            mGame->hitPlayerBasis(mDamages);
        } else {
            setDestination(mPath.front());
            mPath.pop();
        }
    }

    Ogre::Real move = mSpeed * time;
    mDistance -= move;
    mNode->translate(mDirection * move, Ogre::Node::TS_WORLD);
    
    std::pair<int, int> newCoords = mGame->getField()->getGridPosition(getPosition());
    if (newCoords.first != -1 && mCoords != newCoords) {        
        mGame->getField()->removeObject(mCoords);
        mCoords = newCoords;
        mGame->getField()->addObject(mCoords, this);
    }
}

bool Enemy::canBeDeleted() const
{
    return !mAlive && mAnimationState->hasEnded();
}
bool Enemy::isArrived() const
{
    return mIsArrived;
}