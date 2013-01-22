#pragma once
#ifndef __ConfigElement_h_
#define __ConfigElement_h_

#include "tinyxml2.h"

using namespace tinyxml2;

class ConfigElement
{
public:
    ConfigElement(XMLElement* element);

    ConfigElement* getFirstChildElement(const char* name) const;
    ConfigElement* getChildElement(char* path) const;
    ConfigElement* getNextSiblingElement(char* name) const;
    XMLElement* getXmlElement() const;

protected:
    XMLElement* mElement;
};

#endif