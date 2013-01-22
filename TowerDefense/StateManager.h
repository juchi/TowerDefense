
#pragma once
#ifndef __StateManager_h_
#define __StateManager_h_

#include <CEGUI.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <Ogre.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

class State;

class StateManager : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    static StateManager* getInstance();

    void run(State* state);

    void changeState(State* state);
    void pushState(State* state);
    void popState();

    void createFrameListener();
    void setupResources();
    void startInputManager();

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved(const OIS::MouseEvent &arg);
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

    void windowClosed(Ogre::RenderWindow* rw);
    void windowResized(Ogre::RenderWindow* rw);

protected:
    std::vector<State*> mStates;

private:
    StateManager();
    static StateManager* mInstance;

    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;

    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;       // basic camera controller
    OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;

    //OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
};

#endif
