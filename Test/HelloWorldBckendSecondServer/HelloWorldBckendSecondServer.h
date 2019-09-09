#ifndef _HelloWorldBckendSecondServer_H_
#define _HelloWorldBckendSecondServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class HelloWorldBckendSecondServer : public Application
{
public:
    /**
     *
     **/
    virtual ~HelloWorldBckendSecondServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern HelloWorldBckendSecondServer g_app;

////////////////////////////////////////////
#endif
