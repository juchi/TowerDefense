#include "stdafx.h"

#include "Field.h"
#include "GridCell.h"
#include "Astar.h"

Field::Field(Ogre::SceneManager *sceneMgr) : mGridWidth(0), mGridHeight(0)
{
    mFieldWidth = 1000;
    mFieldHeight = 1000;
    mGrid.resize(0);

    buildGrid();
    generateEnemyPath();

    mSceneMgr = sceneMgr;
}

void Field::buildGrid()
{
    // TODO : use resource manager to load map data ?
    std::ifstream file("../media/mymap.tdmap");
    if (!file) {
        throw new std::exception("Can't find map file.");
    }

    std::string line;
    getline(file, line);
    mGridWidth = line.length();
    mGrid.resize(mGridWidth);
    
    do {
        if (line.length() != mGridWidth) {
            throw new std::exception("Map file data is not consistent.");
        }
        int type;
        char val;
        for (int x = 0; x < mGridWidth; x++) {
            val = line.at(x);
            type = atoi(&val);

            if (type == 2) {
                mSpawnPoint = std::pair<int, int>(x, mGridHeight);
            } else if (type == 3) {
                mPlayerBase = std::pair<int, int>(x, mGridHeight);
            }
            mGrid[x].push_back(new GridCell(std::pair<int, int>(x, mGridHeight), type));
        }
        mGridHeight++;
    } while (getline(file, line));
}

void Field::generateEnemyPath()
{
    mEnemyPath.clear();

    std::vector<std::pair<int, int>> directions;
    directions.push_back(std::pair<int, int>(0, 1));
    directions.push_back(std::pair<int, int>(0, -1));
    directions.push_back(std::pair<int, int>(1, 0));
    directions.push_back(std::pair<int, int>(-1, 0));

    Astar astar;
    astar.setDirections(directions);
    astar.setEmptyValue(1);
    astar.setStartPoint(mSpawnPoint);
    astar.setEndPoint(mPlayerBase);

    std::vector<std::vector<int>> grid;
    grid.resize(mGrid.size());
    // copy cells data into the int grid
    for (int x = 0; x < mGrid.size(); x++) {
        grid[x].clear();
        for (int y = 0; y < mGrid[x].size(); y++) {
            grid[x].push_back(mGrid[x][y]->getType() ? 1 : 0);
        }
    }

    astar.setGrid(grid);

    if (astar.run()) {
        std::vector<std::pair<int, int>> path;
        path = astar.getPath();

        std::vector<std::pair<int, int>>::iterator it;
        int size = path.size();
        int i = 0;
        for (it = path.begin(); it < path.end(); it++) {
            mEnemyPath.push_back(getWorldPosition((*it)));
            mGrid[it->first][it->second]->setDistance(size - i++);
            mReversePath.push_front(mGrid[it->first][it->second]);
        }
    } else {
        throw new std::exception("The map doesn't seem to have a path from enemy spawn point to player base.");
    }
}

std::vector<Ogre::Vector3> Field::getEnemyPath() const
{
    return mEnemyPath;
}

bool Field::validateGridCoords(int x, int y)
{
    if (x < 0 || y < 0) {
        return false;
    }
    if (x >= mGridWidth || y >= mGridHeight) {
        return false;
    }

    return true;
}

Ogre::Real Field::getFieldHeight()
{
    return mFieldWidth;
}

Ogre::Real Field::getFieldWidth()
{
    return mFieldWidth;
}

int Field::getGridHeight() const
{
    return mGridHeight;
}

int Field::getGridWidth() const
{
    return mGridWidth;
}

void Field::createTerrain()
{
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, mFieldWidth, mFieldHeight, 10, 10, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
    mEntity = mSceneMgr->createEntity("GroundEntity", "ground");
    mEntity->setMaterialName("Ground/Grass");
    mEntity->setCastShadows(false);
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(mEntity);
    node->setPosition(mFieldWidth/2, 0,  mFieldHeight/2);


    int x, y;
    for (x = 0; x < mGridWidth; x++) {
        for (y = 0; y < mGridHeight; y++) {
            if (mGrid[x][y]->getType() >= 1) {
                createPathTile(x, y);
            }
        }
    }
}

void Field::createPathTile(int x, int y)
{
    char name[16];
    sprintf(name, "tile%2d%2d", x, y);

    Ogre::String entityName(name);
    entityName += "entity";

    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, mFieldWidth/mGridWidth, mFieldHeight/mGridHeight, 10, 10, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
    Ogre::Entity *entity = mSceneMgr->createEntity(entityName, name);
    entity->setMaterialName("Ground/Path");
    entity->setCastShadows(false);
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    node->setPosition(getWorldPosition(x, y) + (Ogre::Vector3::UNIT_Y * 0.1f));
}

std::pair<int, int> Field::getEnemyStartCell()
{
    return std::pair<int, int>(mGridWidth, 5);
}

std::pair<int, int> Field::getPlayerBase()
{
    return std::pair<int, int>(0, 5);
}

bool Field::isCaseFree(int x, int y)
{
    if (!validateGridCoords(x, y)) {
        return false;
    }
    return mGrid[x][y]->isFree();
}

Ogre::Vector3 Field::getWorldPosition(std::pair<int, int> position)
{
    return getWorldPosition(position.first, position.second);
}

Ogre::Vector3 Field::getWorldPosition(int x, int y)
{
    Ogre::Real xPos, zPos;
    xPos = Ogre::Real(x + 0.5) * (mFieldWidth / mGridWidth);
    zPos = Ogre::Real(y + 0.5) * (mFieldHeight / mGridHeight);
    return Ogre::Vector3(xPos, 0, zPos);
}

GridItem* Field::getClosestItem(std::pair<int, int> coords, int type)
{
    GridItem* closestItem = 0;
    Ogre::Real tmpDist, distance = mFieldWidth * mFieldHeight;
    Ogre::Vector3 position = getWorldPosition(coords.first, coords.second);

    std::vector<std::vector<GridCell*>>::iterator itLine;
    std::vector<GridCell*>::iterator itCol;
    for (itLine = mGrid.begin(); itLine < mGrid.end(); itLine++) {
        for (itCol = itLine->begin(); itCol < itLine->end(); itCol++) {
            std::vector<GridItem*> items = (*itCol)->getItems();
            if (!items.empty()) {
                std::vector<GridItem*>::iterator item;
                for (item = items.begin(); item < items.end(); item++) {
                    tmpDist = position.distance((*item)->getPosition());
                    if (tmpDist < distance && (*item)->getType() == type) {
                        distance = tmpDist;
                        closestItem = (*item);
                    }
                }
            }
        }
    }

    return closestItem;
}

std::vector<GridCell*> Field::getCellsInRange(std::pair<int, int> coords, Ogre::Real range)
{
    std::vector<GridCell*> cells;
    Ogre::Vector3 pos = getWorldPosition(coords);

    std::deque<GridCell*>::iterator it;
    for (it = mReversePath.begin(); it < mReversePath.end(); it++) {
        if (getWorldPosition((*it)->getCoordinates()).distance(pos) < range) {
            cells.push_back(*it);
        }
    }

    return cells;
}

void Field::addObject(std::pair<int, int> coords, GridItem* item)
{
    return addObject(coords.first, coords.second, item);
}

void Field::addObject(int x, int y, GridItem* item)
{
    if (x < 0 ||x >= mGridWidth || y < 0 || y >= mGridHeight) {
        return;
    }
    mGrid[x][y]->addItem(item);
}

void Field::removeObject(std::pair<int, int> coords)
{
    removeObject(coords.first, coords.second);
}

void Field::removeObject(int x, int y)
{
    if (x < 0 ||x >= mGridWidth || y < 0 || y >= mGridHeight) {
        return;
    }
    mGrid[x][y]->clear();
}

std::pair<int, int> Field::getGridPosition(const Ogre::Vector3 position)
{
    std::pair<int, int> gridPos;

    gridPos.first = (int)floor(mGridWidth * position.x / mFieldWidth);
    gridPos.second = (int)floor(mGridHeight * position.z / mFieldHeight);

    if (gridPos.first < 0 || gridPos.first >= mGridWidth || gridPos.second < 0 || gridPos.second >= mGridHeight) {
        return std::pair<int, int>(-1, -1);
    }

    return gridPos;
}

std::pair<int, int> Field::getGridPosition(const Ogre::Ray ray)
{
    Ogre::RaySceneQuery *query = mSceneMgr->createRayQuery(ray);
    Ogre::RaySceneQueryResult &result = query->execute();

    Ogre::RaySceneQueryResult::iterator it;
    for (it = result.begin(); it < result.end(); it++) {
        if (it->movable && it->movable->getName() == mEntity->getName()) {
            Ogre::Vector3 intersection = ray.getPoint(it->distance);
            return getGridPosition(intersection);
        }
    }

    return std::pair<int, int>(-1, -1);
}
