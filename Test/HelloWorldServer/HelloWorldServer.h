#ifndef _HelloWorldServer_H_
#define _HelloWorldServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class HelloWorldServer : public Application
{
public:
    /**
     *
     **/
    virtual ~HelloWorldServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern HelloWorldServer g_app;

////////////////////////////////////////////
#endif
