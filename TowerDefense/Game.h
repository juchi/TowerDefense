
#pragma once
#ifndef __Game_h_
#define __Game_h_

#include <stdlib.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class ConfigElement;
class Enemy;
struct EnemyType;
class Field;
class GameInterface;
class Jinx;
class Player;
class Tower;
class TowerBuilder;
class Wave;

class Game //: public BaseApplication
{
public:
    Game(void);
    virtual ~Game(void);

	bool update(const Ogre::Real timeSinceLastFrame);

    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	void manageLeftClick( const OIS::MouseEvent &arg);

    Ogre::Ray createRayFromMouse(const OIS::MouseEvent &arg);

    void gameLost();
    void hitPlayerBasis(int damages);

    void addEnemy(EnemyType type);
    void addTower(int x, int y, ConfigElement* conf);
    void registerJinx(Jinx* jinx);

    Field* getField();
    Ogre::SceneManager* getSceneMgr();

    static Ogre::String generateUniqueName(Ogre::String base = "");

    void playerKill(Enemy* enemy);
    void infoDataChanged();
    void updateDisplay();


protected:
	void initCEGUI();
	void createField();
    virtual void createScene(void);

	bool mLeftPress;
	bool mRightPress;

	bool mQuit;
    bool mGameEnded;
    bool mShouldUpdateInterfaceInfo;
    bool mBuildMode;
    int mTimeFactor;

    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;

    GameInterface* mGameInterface;

    std::vector<Enemy*> mEnemies;
    std::vector<Tower*> mTowers;
    std::vector<Jinx*>  mJinxes;
    Player* mPlayer;
    Field* mField;
    TowerBuilder* mTowerBuilder;

    Wave*             mCurrentWave;
    std::queue<Wave*> mWaves;
    int               mWaveIndex;
};

#endif // #ifndef __Game_h_
