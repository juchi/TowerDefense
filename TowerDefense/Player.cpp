#include "stdafx.h"

#include "Config.h"
#include "ConfigElement.h"
#include "Player.h"
#include "Game.h"

Player::Player(Game *game) : mGame(game), mMoney(0), mHealth(0)
{
    XMLElement* node = Config::getSingleton()->getRoot()->getFirstChildElement("player")->getXmlElement();
    mMoney = atoi(node->Attribute("money"));
    mHealth = atoi(node->Attribute("health"));
}

int Player::getMoney() const
{
    return mMoney;
}

void Player::earnMoney(int money)
{
    mMoney += money;
    mGame->infoDataChanged();
}

bool Player::spendMoney(int money)
{
    if (hasMoney(money)) {
        mMoney -= money;
        mGame->infoDataChanged();
        return true;
    }
    return false;
}

bool Player::hasMoney(int money) const
{
    return money <= mMoney;
}

int Player::getHealth() const
{
    return mHealth;
}

void Player::takeDamages(int damages)
{
    mHealth -= damages;
    if (mHealth < 0) {
        mHealth = 0;
    }
}

void Player::addPoints(int points)
{
    mScore += points;
}

int Player::getScore() const
{
    return mScore;
}
