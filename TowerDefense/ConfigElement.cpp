#include "stdafx.h"

#include "ConfigElement.h"

ConfigElement::ConfigElement(XMLElement* element) : mElement(element)
{
}

ConfigElement* ConfigElement::getFirstChildElement(const char* name) const
{
    if (mElement == 0) {
        std::string msg = std::string("Error getting child ") + name + " for NULL parent";
        throw std::exception(msg.c_str());
    }
    XMLElement* child = mElement->FirstChildElement(name);
    return new ConfigElement(child);
}

ConfigElement* ConfigElement::getChildElement(char* path) const
{
    return 0;
}

ConfigElement* ConfigElement::getNextSiblingElement(char* name) const
{
    if (mElement == 0) {
        std::string msg = std::string("Error getting sibling ") + name + " for NULL element";
        throw std::exception(msg.c_str());
    }
    XMLElement* sibling = mElement->NextSiblingElement(name);
    return new ConfigElement(sibling);
}

XMLElement* ConfigElement::getXmlElement() const
{
    return mElement;
}