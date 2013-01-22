#include "stdafx.h"

#include "Game.h"

#include "Config.h"
#include "ConfigElement.h"
#include "Enemy.h"
#include "Field.h"
#include "GameInterface.h"
#include "Jinx.h"
#include "Player.h"
#include "Tower.h"
#include "TowerBuilder.h"
#include "Wave.h"

Game::Game(void) : mLeftPress(false), mRightPress(false), mQuit(false), mGameEnded(false), mField(0), mWaveIndex(0),
    mCurrentWave(0), mGameInterface(0), mShouldUpdateInterfaceInfo(false), mBuildMode(false), mTimeFactor(1)
{
    Config::getSingleton()->loadFile("config.xml");

    mSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("sceneMgr");

    initCEGUI();
    createField();
    createScene();
    
    mPlayer = new Player(this);
    mTowerBuilder = new TowerBuilder(this, mPlayer);

    ConfigElement *waveElement = Config::getSingleton()->getRoot()->getFirstChildElement("waves")->getFirstChildElement("wave");
    while (waveElement->getXmlElement()) {
        mWaves.push(new Wave(this, waveElement));
        waveElement = waveElement->getNextSiblingElement("wave");
    }
    infoDataChanged();
}

Game::~Game(void)
{
    delete mTowerBuilder;
    delete mPlayer;
    delete mField;
    delete mGameInterface;

    while (!mWaves.empty()) {
        delete mWaves.front();
        mWaves.pop();
    }

    delete mSceneMgr;
}

void Game::initCEGUI()
{
    mGameInterface = GameInterface::getInstance();
}

void Game::createField()
{
    mField = new Field(mSceneMgr);
    mField->createTerrain();
}

void Game::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.0, 0.0, 0.0));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Ogre::Light *light = mSceneMgr->createLight("light");
    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(1.0, 1.0, 1.0);
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(10.0, -20.0, -5);

    mCamera = mSceneMgr->getCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(mField->getFieldWidth()/2, 700, 3 * mField->getFieldHeight() / 2));
    mCamera->lookAt(mField->getFieldWidth()/2, 0, mField->getFieldHeight()/2);
}

void Game::addEnemy(EnemyType type)
{
    Enemy *enemy = new Enemy(type, this);
    
    std::pair<int, int> cell = mField->getEnemyStartCell();
    Ogre::Vector3 start, destination;
    start = mField->getWorldPosition(cell.first, cell.second);
    destination = mField->getWorldPosition(mField->getPlayerBase());

    enemy->setGridCoords(cell);
    enemy->setPosition(start);
    enemy->setPath(mField->getEnemyPath());

    mEnemies.push_back(enemy);
}

void Game::addTower(int x, int y, ConfigElement* conf)
{
    Tower *tower = new Tower(this, conf);

    Ogre::Vector3 position = mField->getWorldPosition(x, y);
    tower->setPosition(position);
    tower->setGridCoords(x, y);
    mField->addObject(x, y, tower);
    
    std::vector<GridCell*> cells = mField->getCellsInRange(std::pair<int, int>(x, y), tower->getRange());
    tower->setRangeCells(cells);

    mTowers.push_back(tower);

    mBuildMode = false;
}

void Game::registerJinx(Jinx* jinx)
{
    mJinxes.push_back(jinx);
}

Field* Game::getField()
{
    return mField;
}

Ogre::SceneManager* Game::getSceneMgr()
{
    return mSceneMgr;
}

void Game::hitPlayerBasis(int damages)
{
    mPlayer->takeDamages(damages);
    infoDataChanged();
}

void Game::gameLost()
{
    mGameInterface->showAlert("You lost");

    mGameEnded = true;
}

bool Game::update(const Ogre::Real timeSinceLastFrame)
{
	if (mQuit == true) {
		return false;
	}

    if (!mGameEnded) {
        Ogre::Real time = mTimeFactor * timeSinceLastFrame;

        if (mPlayer->getHealth() <= 0) {
            gameLost();
        }

        if (mCurrentWave && !mCurrentWave->isFinished()) {
            mCurrentWave->update(time);
        } else {
            delete mCurrentWave;
            mCurrentWave = 0;
            if (!mWaves.empty()) {
                mCurrentWave = mWaves.front();
                mWaves.pop();
                mWaveIndex++;
                infoDataChanged();
            }
        }
        
        std::vector<Enemy*>::iterator itEnemy;
        for (itEnemy = mEnemies.begin(); itEnemy < mEnemies.end();) {
            if ((*itEnemy)->canBeDeleted()) {
                delete (*itEnemy);
                itEnemy = mEnemies.erase(itEnemy);
            } else {
                (*itEnemy)->update(time);
                itEnemy++;
            }
        }

        std::vector<Tower*>::iterator itTower;
        for (itTower = mTowers.begin(); itTower < mTowers.end(); itTower++) {
            (*itTower)->update(time);
        }

        std::vector<Jinx*>::iterator itJinx;
        for (itJinx = mJinxes.begin(); itJinx < mJinxes.end();) {
            if ((*itJinx)->canBeDeleted()) {
                itJinx = mJinxes.erase(itJinx);
            } else {
                (*itJinx)->update(time);
                itJinx++;
            }
        }
        if (mShouldUpdateInterfaceInfo) { 
            updateDisplay();
        }
    }

    return true;
}

bool Game::mouseMoved(const OIS::MouseEvent &arg)
{
    if (mRightPress) {
        //return BaseApplication::mouseMoved(arg);
    }
    if (mBuildMode) {
        Ogre::Ray ray = createRayFromMouse(arg);
        std::pair<int, int> coords = mField->getGridPosition(ray);
        mTowerBuilder->setPreviewPosition(coords.first, coords.second);
    }

    return true;
}

bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (id == OIS::MB_Right) {
        mRightPress = true;
    } else if (id == OIS::MB_Left) {
        manageLeftClick(arg);
        mLeftPress = true;
    }
    return true;
}

void Game::manageLeftClick( const OIS::MouseEvent &arg)
{
    if (mBuildMode) {
        int x, y;
        Ogre::Ray ray = createRayFromMouse(arg);
        std::pair<int, int> coords = mField->getGridPosition(ray);
        x = coords.first;
        y = coords.second;

        mTowerBuilder->buildTower(x, y);
    }
}

Ogre::Ray Game::createRayFromMouse(const OIS::MouseEvent &arg)
{
    CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.d_x/float(arg.state.width), mousePos.d_y/float(arg.state.height));

    return mouseRay;
}

bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (id == OIS::MB_Right) {
        mRightPress = false;
    } else if (id == OIS::MB_Left) {
        mLeftPress = false;
    }
    return true;
}

bool Game::keyPressed( const OIS::KeyEvent &arg )
{
    if (arg.key == OIS::KC_B) {
        mBuildMode = !mBuildMode;
        mTowerBuilder->hidePreview();
        updateDisplay();
    } else if (arg.key == OIS::KC_V) {
        mTimeFactor *= 2;
        if (mTimeFactor > 4) {
            mTimeFactor = 1;
        }
    }
    return true;
}

bool Game::keyReleased( const OIS::KeyEvent &arg )
{
    return true;
}


Ogre::String Game::generateUniqueName(Ogre::String base)
{
    static int id = 0;

    const char* pattern = base.append("%d").c_str();
    char name[64];
    sprintf(name, pattern, id++);

    return Ogre::String(name);
}

void Game::playerKill(Enemy* enemy)
{
    mPlayer->earnMoney(enemy->getMoneyDrop());
    infoDataChanged();
}

void Game::infoDataChanged()
{
    mShouldUpdateInterfaceInfo = true;
}

void Game::updateDisplay()
{
    mGameInterface->updateData("money", mPlayer->getMoney());
    mGameInterface->updateData("health", mPlayer->getHealth());
    mGameInterface->updateData("enemies", mCurrentWave ? mCurrentWave->getEnemiesLeft() : 0);
    mGameInterface->updateData("wave", mWaveIndex);
    mGameInterface->updateData("cost", mTowerBuilder->getPrice());
    //mGameInterface->updateData("build", mBuildMode);
    mGameInterface->updateInfosDisplay();
    mShouldUpdateInterfaceInfo = false;
}
