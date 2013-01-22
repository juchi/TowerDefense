#include "stdafx.h"

#include "State.h"
#include "StateManager.h"

void State::changeState(State* state)
{
    StateManager::getInstance()->changeState(state);
}

void State::popState()
{
    StateManager::getInstance()->popState();
}

void State::pushState(State* state)
{
    StateManager::getInstance()->pushState(state);
}
