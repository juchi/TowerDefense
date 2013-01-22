#include "stdafx.h"

#include "GridCell.h"

GridCell::GridCell(std::pair<int, int> coords, char type) : mType(type), mDistanceBase(0)
{
    mCoords = coords;
}

void GridCell::addItem(GridItem* item)
{
    mItems.push_back(item);
}

std::vector<GridItem*> GridCell::getItems()
{
    return mItems;
}

void GridCell::clear()
{
    mItems.clear();
}

std::pair<int, int> GridCell::getCoordinates() const
{
    return mCoords;
}

void GridCell::setCoordinates(std::pair<int, int> coords)
{
    mCoords = coords;
}

char GridCell::getDistance() const
{
    return mDistanceBase;
}

void GridCell::setDistance(char distance)
{
    if (mType > 0) {
        mDistanceBase = distance;
    }
}

char GridCell::getType() const
{
    return mType;
}

bool GridCell::isFree() const
{
    return mType == 0 && mItems.empty();
}
