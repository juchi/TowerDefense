#pragma once
#ifndef __Config_h_
#define __Config_h_

#include "tinyxml2.h"

using namespace tinyxml2;

class ConfigElement;

class Config
{
public:
    static Config* getSingleton();

    void loadFile(const char* filename);

    ConfigElement* getElement(char* path) const;
    ConfigElement* getRoot() const;

protected:
    XMLDocument mDoc;
    ConfigElement *mRoot;

private:
    Config();
    static Config* mInstance;
};

#endif