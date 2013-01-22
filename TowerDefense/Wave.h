#pragma once
#ifndef __Wave_h_
#define __Wave_h_

#include "Enemy.h"

class Game;
class ConfigElement;

class Wave
{
public:
    Wave(Game* game, ConfigElement* waveElement);

    void update(Ogre::Real time);

    bool isFinished() const;
    int getEnemiesLeft() const;

    EnemyType spawnEnemy(); // return enemy type ?

protected:
    Game* mGame;

    int mEnemiesLeft;

    std::vector<EnemyType> mTypes;
    // array of struct of enemy types ?
    
    Ogre::Real mTimeSinceLastEnemy;
    Ogre::Real mEnemyDelay;
};

#endif
