#pragma once
#ifndef __PlayState_h_
#define __PlayState_h_

#include "State.h"

class Game;

class PlayState : public State
{
public:
    static PlayState* getInstance();

    virtual void pause();
    virtual void resume();

    virtual void enter();
    virtual void exit();

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);

protected:
    Game* mGame;

private:
    PlayState();
    static PlayState* mInstance;
};

#endif
