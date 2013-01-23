#pragma once
#ifndef __TowerBuilder_h_
#define __TowerBuilder_h_

class ConfigElement;
class Field;
class Player;
class Game;
class RangeCircle;
class TowerPreview;

class TowerBuilder
{
public:
    TowerBuilder(Game* game, Player* player);

    void buildTower(int x, int y);

    int getPrice() const;

    void setPreviewPosition(int x, int y);
    void setPreviewVisible(bool visible);

protected:
    int mPrice;
    float mPriceFactor;

    Field*  mField;
    Game*   mGame;
    Player* mPlayer;

    ConfigElement* mConfData;

    TowerPreview* mTowerPreview;
    RangeCircle*  mCircle;
};

#endif
