#pragma once
#ifndef __PauseState_h_
#define __PauseState_h_

#include "State.h"

class PauseState : public State
{
public:
    static PauseState* getInstance();

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

private:
    PauseState();
    static PauseState* mInstance;
};

#endif
