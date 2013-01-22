#pragma once
#ifndef __Singleton_h_
#define __Singleton_h_

class Singleton
{
public:
    static Singleton* getSingleton();

protected:
    Singleton();
private:
    static Singleton* mInstance;
};

#endif