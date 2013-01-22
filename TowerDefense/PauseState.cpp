#include "stdafx.h"

#include "PauseState.h"

#include "GameInterface.h"

PauseState* PauseState::mInstance = 0;

PauseState::PauseState()
{
}

PauseState* PauseState::getInstance()
{
    if (mInstance == 0) {
        mInstance = new PauseState();
    }
    return mInstance;
}

void PauseState::pause()
{
}

void PauseState::resume()
{
}

void PauseState::enter()
{
    GameInterface::getInstance()->showPauseMenu();
}

void PauseState::exit()
{
    GameInterface::getInstance()->hidePauseMenu();
}

bool PauseState::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    return true;
}

bool PauseState::keyPressed(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_P) {
        popState();
    }
    return true;
}

bool PauseState::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}

bool PauseState::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool PauseState::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool PauseState::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}
