#include "stdafx.h"

#include "PlayState.h"

#include "Game.h"
#include "PauseState.h"

PlayState* PlayState::mInstance = 0;

PlayState::PlayState() : mGame(0)
{
}

PlayState* PlayState::getInstance()
{
    if (mInstance == 0) {
        mInstance = new PlayState();
    }
    return mInstance;
}

void PlayState::pause()
{
}

void PlayState::resume()
{
}

void PlayState::enter()
{
    mGame = new Game();
}

void PlayState::exit()
{
    //delete mGame
}

bool PlayState::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    return mGame->update(evt.timeSinceLastFrame);
}

bool PlayState::keyPressed(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_P) {
        pushState(PauseState::getInstance());
    }
    return mGame->keyPressed(arg);
}

bool PlayState::keyReleased(const OIS::KeyEvent &arg)
{
    return mGame->keyReleased(arg);
}

bool PlayState::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return mGame->mousePressed(arg , id);
}

bool PlayState::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return mGame->mouseReleased(arg, id);
}

bool PlayState::mouseMoved(const OIS::MouseEvent &arg)
{
    return mGame->mouseMoved(arg);
}
