#pragma once
#ifndef __Field_h_
#define __Field_h_

#include <stdlib.h>

#include "GridItem.h"
#include "GridCell.h"

class Field
{
public:
    Field(Ogre::SceneManager *sceneMgr);

    Ogre::Real getFieldHeight();
    Ogre::Real getFieldWidth();
    int getGridHeight() const;
    int getGridWidth() const;

    void buildGrid();
    void createTerrain();
    void createPathTile(int x, int y);
    void generateEnemyPath();
    std::vector<Ogre::Vector3> getEnemyPath() const;

    void addObject(std::pair<int, int> coords, GridItem* item = 0);
    void addObject(int x, int y, GridItem* item = 0);
    void removeObject(std::pair<int, int> coords);
    void removeObject(int x, int y);

    bool validateGridCoords(int x, int y);
    bool isCaseFree(int x, int y);
    std::pair<int, int> getEnemyStartCell();
    std::pair<int, int> getPlayerBase();
    std::pair<int, int> getGridPosition(const Ogre::Ray ray);
    std::pair<int, int> getGridPosition(const Ogre::Vector3 position);

    Ogre::Vector3 getWorldPosition(std::pair<int, int> position);
    Ogre::Vector3 getWorldPosition(int x, int y);
    GridItem* getClosestItem(std::pair<int, int> position, int type);

    std::vector<GridCell*> getCellsInRange(std::pair<int, int> coords, Ogre::Real range);
    //GridItem* getFurthestEnemy(std::pair<int, int>, Ogre::Real range, int type);

protected:
    Ogre::Real mFieldWidth;
    Ogre::Real mFieldHeight;

    int mGridWidth;
    int mGridHeight;

    Ogre::SceneManager *mSceneMgr;
    Ogre::Entity *mEntity;

    std::vector<std::vector<GridCell*>> mGrid;

    std::deque<GridCell*>      mReversePath;
    std::vector<Ogre::Vector3> mEnemyPath;
    std::pair<int, int> mPlayerBase;
    std::pair<int, int> mSpawnPoint;
};

#endif
