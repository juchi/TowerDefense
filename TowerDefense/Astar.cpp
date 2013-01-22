#include "stdafx.h"

#include "Astar.h"

using namespace std;

Astar::Astar() : empty(0), mStart(0, 0), mEnd(0, 0)
{
}

bool Astar::run()
{
    mPath.resize(0);

    mPath.push_back(mStart);
    pair<int, int> currentPoint = mStart;

    vector<pair<int, int>>::iterator itDir;

    bool foundValidDir = false;
    bool success = false;

    while (!success) {
        foundValidDir = false;
        for (itDir = mDirections.begin(); !foundValidDir && itDir < mDirections.end(); itDir++) {
            currentPoint.first = mPath.back().first + (*itDir).first;
            currentPoint.second = mPath.back().second + (*itDir).second;

            if (isPositionValid(currentPoint)) {
                if (currentPoint == mEnd) { // success
                    mPath.push_back(currentPoint);
                    foundValidDir = true;
                    success = true;
                } else if (mGrid[currentPoint.first][currentPoint.second].value == empty && !closed(currentPoint)) { // path
                    mPath.push_back(currentPoint);
                    mGrid[currentPoint.first][currentPoint.second].closed = true;
                    foundValidDir = true;
                }
            }
        }
        if (!foundValidDir) {
            mPath.pop_back();
            if (mPath.empty()) {
                return false;
            }
        }
    }
    return true;
}

void Astar::setStartPoint(std::pair<int, int> point)
{
    mStart = point;
}

void Astar::setEndPoint(std::pair<int, int> point)
{
    mEnd = point;
}

void Astar::setDirections(vector<pair<int, int>> directions)
{
    mDirections = directions;
}

void Astar::setGrid(vector<vector<int>> grid)
{
    mGrid.clear();
    mGrid.resize(grid.size());
    for (int x = 0; x < grid.size(); x++) {
        mGrid[x].clear();
        for (int y = 0; y < grid[x].size(); y++) {
            Cell cell;
            cell.closed = false;
            cell.value = grid[x][y];
            mGrid[x].push_back(cell);
        }
    }
}

vector<pair<int, int>> Astar::getPath()
{
    return mPath;
}

bool Astar::isPositionValid(pair<int, int> pos)
{
    int x = pos.first;
    int y = pos.second;

    if (x < 0 || y < 0) {
        return false;
    }
    if (x >= mGrid.size() || y >= mGrid.front().size()) {
        return false;
    }
    return true;
}

void Astar::setEmptyValue(int value)
{
    empty = value;
}

bool Astar::closed(pair<int, int> pos)
{
    return mGrid[pos.first][pos.second].closed;
}
