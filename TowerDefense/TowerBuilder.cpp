#include "stdafx.h"

#include "TowerBuilder.h"
#include "Config.h"
#include "ConfigElement.h"
#include "Field.h"
#include "Game.h"
#include "Player.h"
#include "TowerPreview.h"

TowerBuilder::TowerBuilder(Game* game, Player* player) : mGame(game), mPlayer(player), mPrice(0), mPriceFactor(0.0)
{
    mField = mGame->getField();

    mConfData = Config::getSingleton()->getRoot()->getFirstChildElement("towers")->getFirstChildElement("tower");
    mPrice = atoi(mConfData->getXmlElement()->Attribute("price"));
    mPriceFactor = atof(mConfData->getXmlElement()->Attribute("pricefactor"));

    mTowerPreview = new TowerPreview(mGame, mConfData);
}

void TowerBuilder::buildTower(int x, int y)
{
    if (mPlayer->hasMoney(mPrice) && mField->isCaseFree(x, y)) {
        mPlayer->spendMoney(mPrice);
        mGame->addTower(x, y, mConfData);
        mPrice = (int) (mPrice * mPriceFactor);
        mGame->infoDataChanged();
        hidePreview();
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
    mTowerPreview->setVisible(true);
    mTowerPreview->setValid(mField->isCaseFree(x, y));
}

void TowerBuilder::hidePreview()
{
    mTowerPreview->setVisible(false);
}