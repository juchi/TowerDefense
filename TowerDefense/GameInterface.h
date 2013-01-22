#pragma once
#ifndef __GameInterface_h_
#define __GameInterface_h_

#include <CEGUI.h>
#include <RendererModules\Ogre\CEGUIOgreRenderer.h>

struct InfoData
{
    int value;
    char* format;
};

class GameInterface
{
public:
    static GameInterface* getInstance();
    ~GameInterface();
    void showAlert(const char* text);
    void updateData(char* key, int value);

    void updateInfosDisplay();
    
    bool leavePause(const CEGUI::EventArgs &e);
    void showPauseMenu();
    void hidePauseMenu();

protected:
    void createGameSheet();
    void createPauseSheet();

    CEGUI::OgreRenderer *mRenderer;

    CEGUI::Window *mWndInfo;

    std::map<char*, InfoData> mInfos;

private:
    GameInterface();
    static GameInterface* mInstance;
};

#endif
