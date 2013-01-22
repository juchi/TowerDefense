#pragma once
#ifndef __TowerBuilder_h_
#define __TowerBuilder_h_

class ConfigElement;
class Field;
class Player;
class Game;
class TowerPreview;

class TowerBuilder
{
public:
    TowerBuilder(Game* game, Player* player);

    void buildTower(int x, int y);

    int getPrice() const;

    void setPreviewPosition(int x, int y);
    void hidePreview();

protected:
    int mPrice;
    float mPriceFactor;

    Field*  mField;
    Game*   mGame;
    Player* mPlayer;

    ConfigElement* mConfData;

    TowerPreview* mTowerPreview;
};

#endif
