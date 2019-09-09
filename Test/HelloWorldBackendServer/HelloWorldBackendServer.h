#ifndef _HelloWorldBackendServer_H_
#define _HelloWorldBackendServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

/**
 *
 **/
class HelloWorldBackendServer : public Application
{
public:
    /**
     *
     **/
    virtual ~HelloWorldBackendServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();
};

extern HelloWorldBackendServer g_app;

////////////////////////////////////////////
#endif
