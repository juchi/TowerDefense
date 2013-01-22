#include "stdafx.h"

#include "Config.h"
#include "ConfigElement.h"

Config* Config::mInstance = 0;

Config::Config() : mRoot(0)
{
}

Config* Config::getSingleton()
{
    if (mInstance == 0) {
        mInstance = new Config();
    }
    return mInstance;
}

void Config::loadFile(const char* filename)
{
    XMLError result = mDoc.LoadFile(filename);
    if (result != XML_SUCCESS) {
        std::string msg = std::string("Error reading XML file with name ") + filename;
        throw std::exception(msg.c_str());
    }
    XMLElement* root = mDoc.FirstChildElement("towerdefense");

    mRoot = new ConfigElement(root);
}

ConfigElement* Config::getRoot() const
{
    return mRoot;
}

ConfigElement* Config::getElement(char* path) const
{
    return mRoot;
}
