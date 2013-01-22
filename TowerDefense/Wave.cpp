#include "stdafx.h"

#include "Config.h"
#include "ConfigElement.h"
#include "Enemy.h"
#include "Game.h"
#include "Wave.h"

Wave::Wave(Game* game, ConfigElement* waveElement) : mGame(game), mEnemiesLeft(0), mTimeSinceLastEnemy(0.0f), mEnemyDelay(0.0f)
{
    const char *enemyTypename = waveElement->getFirstChildElement("enemy")->getXmlElement()->GetText();

    tinyxml2::XMLElement *node = Config::getSingleton()->getRoot()->getFirstChildElement("enemies")->getFirstChildElement(enemyTypename)->getXmlElement();

    EnemyType type;
    type.name = Ogre::String(enemyTypename);
    type.power = atoi(node->Attribute("damages"));
    type.health = atoi(node->Attribute("health"));
    type.moneyDrop = atoi(node->Attribute("moneydrop"));
    type.speed = atoi(node->Attribute("speed"));
    type.mesh = node->FirstChildElement("mesh")->Attribute("file");

    mTypes.push_back(type);

    node = waveElement->getFirstChildElement("number")->getXmlElement();
    mEnemiesLeft = atoi(node->GetText());

    node = waveElement->getFirstChildElement("frequency")->getXmlElement();
    mEnemyDelay = atof(node->GetText());
}

bool Wave::isFinished() const
{
    return mEnemiesLeft == 0;
}

int Wave::getEnemiesLeft() const
{
    return mEnemiesLeft;
}

void Wave::update(Ogre::Real time)
{
    mTimeSinceLastEnemy += time;

    if (mTimeSinceLastEnemy > mEnemyDelay) {
        mGame->addEnemy(spawnEnemy());
        mTimeSinceLastEnemy = 0.0f;
    }
}

EnemyType Wave::spawnEnemy()
{
    if (mEnemiesLeft > 0) {
        mEnemiesLeft--;
    }
    return mTypes.front();
}