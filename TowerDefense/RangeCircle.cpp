#include "stdafx.h"

#include "RangeCircle.h"
#include "Game.h"

RangeCircle::RangeCircle(Game* game) : mGame(game), mManual(0), mNode(0), mRadius(0.0)
{
    mSceneMgr = game->getSceneMgr();
    mManual = mSceneMgr->createManualObject(Game::generateUniqueName("range"));
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Game::generateUniqueName("rangenode"));
    mNode->attachObject(mManual);
}

RangeCircle::~RangeCircle()
{
    mNode->detachObject(mManual);
    mSceneMgr->destroySceneNode(mNode);
    mSceneMgr->destroyManualObject(mManual);
}

void RangeCircle::setPosition(Ogre::Vector3 pos)
{
    mNode->setPosition(pos);
}

void RangeCircle::setRadius(Ogre::Real radius)
{
    mRadius = radius;

    Ogre::Vector3 pos = Ogre::Vector3::UNIT_Y * 0.1f;

    mManual->clear();
    mManual->begin("", Ogre::RenderOperation::OT_LINE_STRIP);

    for (int deg = 0; deg <= 360; deg += 20) {
        pos.x = Ogre::Math::Cos(Ogre::Degree((float)deg)) * mRadius;
        pos.z = Ogre::Math::Sin(Ogre::Degree((float)deg)) * mRadius;
        mManual->position(pos);
    }

    mManual->end();
}

void RangeCircle::setVisible(bool visible)
{
    mNode->setVisible(visible);
}
