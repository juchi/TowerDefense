#include "stdafx.h"

#include "Singleton.h"

Singleton* Singleton::mInstance = 0;

Singleton::Singleton()
{
}

Singleton* Singleton::getSingleton()
{
    if (mInstance == 0) {
        mInstance = new Singleton();
    }
    return mInstance;
}