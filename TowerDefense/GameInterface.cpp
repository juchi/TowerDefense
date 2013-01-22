#include "stdafx.h"

#include "GameInterface.h"

#include "StateManager.h"
#include "CEGUIEvent.h"

GameInterface* GameInterface::mInstance = 0;

GameInterface::GameInterface()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TowerDefense.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("OgreTrayImages", "MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor());

    createGameSheet();
    createPauseSheet();

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.getWindow("TowerDefense/GameSheet");
    CEGUI::System::getSingleton().setGUISheet(sheet);

    mInfos["money"].value = 0;
    mInfos["money"].format = "Money: %d";
    mInfos["health"].value = 0;
    mInfos["health"].format = "Health: %d";
    mInfos["cost"].value = 0;
    mInfos["cost"].format = "Cost: %d";
    mInfos["wave"].value = 0;
    mInfos["wave"].format = "Wave: %d";
    mInfos["enemies"].value = 0;
    mInfos["enemies"].format = "Enemies left: %d";
}

GameInterface::~GameInterface()
{
    CEGUI::OgreRenderer::destroy(*mRenderer);
}

GameInterface* GameInterface::getInstance()
{
    if (mInstance == 0) {
        mInstance = new GameInterface();
    }

    return mInstance;
}

void GameInterface::createGameSheet()
{    
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "TowerDefense/GameSheet");
    
    mWndInfo = wmgr.createWindow("OgreTray/StaticText", "TowerDefense/InfoText");
	mWndInfo->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.15f, 0)));
    mWndInfo->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85f, 0), CEGUI::UDim(0.85f, 0)));
	sheet->addChildWindow(mWndInfo);
}

void GameInterface::createPauseSheet()
{
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "TowerDefense/PauseSheet");

    CEGUI::Window* title = wmgr.createWindow("OgreTray/StaticText", "TowerDefense/Pause/Title");
    title->setSize(CEGUI::UVector2(CEGUI::UDim(0.20f, 0), CEGUI::UDim(0.10f, 0)));
    title->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40f, 0), CEGUI::UDim(0.20f, 0)));
    sheet->addChildWindow(title);

    CEGUI::Window* button = wmgr.createWindow("OgreTray/Button", "TowerDefense/Pause/Resume");
	button->setSize(CEGUI::UVector2(CEGUI::UDim(0.20f, 0), CEGUI::UDim(0.10f, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40f, 0), CEGUI::UDim(0.35f, 0)));
    button->setText("Resume");
    button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameInterface::leavePause, this));
    sheet->addChildWindow(button);

    button = wmgr.createWindow("OgreTray/Button", "TowerDefense/Pause/Quit");
   	button->setSize(CEGUI::UVector2(CEGUI::UDim(0.20f, 0), CEGUI::UDim(0.10f, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40f, 0), CEGUI::UDim(0.50f, 0)));
    button->setText("Quit");
    sheet->addChildWindow(button);
}

bool GameInterface::leavePause(const CEGUI::EventArgs &e)
{
    StateManager::getInstance()->popState();
    return true;
}

void GameInterface::showPauseMenu()
{
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.getWindow("TowerDefense/GameSheet");
    sheet->addChildWindow("TowerDefense/PauseSheet");
}

void GameInterface::hidePauseMenu()
{
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.getWindow("TowerDefense/GameSheet");
    sheet->removeChildWindow("TowerDefense/PauseSheet");
}

void GameInterface::showAlert(const char* text)
{
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.getWindow("TowerDefense/GameSheet");
	CEGUI::Window *lost = wmgr.createWindow("OgreTray/Button", "TowerDefense/LostLabel");
	lost->setText(text);
	lost->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.1f, 0)));
    lost->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.45f, 0)));
	sheet->addChildWindow(lost);
}

void GameInterface::updateData(char* key, int value)
{
    mInfos[key].value = value;
}

void GameInterface::updateInfosDisplay()
{
    std::string text = "";

    std::map<char*, InfoData>::iterator it;
    for (it = mInfos.begin(); it != mInfos.end(); it++) {
        InfoData data = it->second;
        
        char string[32];
        sprintf(string, data.format, data.value);

        text += string;
        text += '\n';
    }

    mWndInfo->setText(text);
}
