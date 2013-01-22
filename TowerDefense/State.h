#pragma once
#ifndef __State_h_
#define __State_h_

#include <OIS.h>

class State
{
public:
    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void enter() = 0;
    virtual void exit() = 0;

    virtual void changeState(State* state);
    virtual void popState();
    virtual void pushState(State* state);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;

    virtual bool keyPressed(const OIS::KeyEvent &arg) = 0;
    virtual bool keyReleased(const OIS::KeyEvent &arg) = 0;

    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
    virtual bool mouseMoved(const OIS::MouseEvent &arg) = 0;
};

#endif
