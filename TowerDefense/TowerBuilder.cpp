#include "stdafx.h"

#include "TowerBuilder.h"
#include "Config.h"
#include "ConfigElement.h"
#include "Field.h"
#include "Game.h"
#include "Player.h"
#include "RangeCircle.h"
#include "TowerPreview.h"

TowerBuilder::TowerBuilder(Game* game, Player* player) : mGame(game), mPlayer(player), mPrice(0), mPriceFactor(0.0)
{
    mField = mGame->getField();

    mConfData = Config::getSingleton()->getRoot()->getFirstChildElement("towers")->getFirstChildElement("tower");
    mPrice = atoi(mConfData->getXmlElement()->Attribute("price"));
    mPriceFactor = (float)atof(mConfData->getXmlElement()->Attribute("pricefactor"));
    Ogre::Real range = (float)atof(mConfData->getXmlElement()->Attribute("range"));

    mTowerPreview = new TowerPreview(mGame, mConfData);
    mCircle = new RangeCircle(game);
    mCircle->setRadius(range);
    
    setPreviewVisible(false);
}

void TowerBuilder::buildTower(int x, int y)
{
    if (mPlayer->hasMoney(mPrice) && mField->isCaseFree(x, y)) {
        mPlayer->spendMoney(mPrice);
        mGame->addTower(x, y, mConfData);
        mPrice = (int) (mPrice * mPriceFactor);
        mGame->infoDataChanged();
        setPreviewVisible(false);
    }
}

int TowerBuilder::getPrice() const
{
    return mPrice;
}

void TowerBuilder::setPreviewPosition(int x, int y)
{
    Ogre::Vector3 &pos = mField->getWorldPosition(x, y);
    mTowerPreview->setPosition(pos);
    mTowerPreview->setValid(mField->isCaseFree(x, y));

    mCircle->setPosition(mTowerPreview->getPosition());

    setPreviewVisible(true);
}

void TowerBuilder::setPreviewVisible(bool visible)
{
    mTowerPreview->setVisible(visible);
    mCircle->setVisible(visible);
}