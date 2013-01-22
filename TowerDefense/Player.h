#pragma once
#ifndef __Player_h_
#define __Player_h_

class Game;

class Player
{
public:
    Player(Game *game);

    int getMoney() const;
    void earnMoney(int money);
    bool spendMoney(int money);
    bool hasMoney(int money) const;

    int getHealth() const;
    void takeDamages(int damages);
    void addPoints(int points);
    int getScore() const;

protected:
    int mHealth;
    int mMoney;
    int mScore;

    Game *mGame;
};

#endif