#pragma once
#ifndef __GridCell_h_
#define __GridCell_h_

class GridItem;

class GridCell
{
public:
    GridCell(std::pair<int, int> coords, char type = 0);

    char getType() const;
    char getDistance() const;
    void setDistance(char distance);
    bool isFree() const;

    void addItem(GridItem* item);
    std::vector<GridItem*> getItems();

    std::pair<int, int> getCoordinates() const;
    void setCoordinates(std::pair<int, int>);

    void clear();

protected:
    char mType; // type of cell - 0 empty - 1 path - 2 spawn - 3 base
    char mDistanceBase;
    std::vector<GridItem*> mItems;
    std::pair<int, int> mCoords;
};

#endif
