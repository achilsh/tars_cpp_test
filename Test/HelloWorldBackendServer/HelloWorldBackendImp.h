#ifndef _HelloWorldBackendImp_H_
#define _HelloWorldBackendImp_H_

#include "servant/Application.h"
#include "HelloWorldBackend.h"

/**
 *
 *
 */
class HelloWorldBackendImp : public Test::HelloWorldBackend
{
public:
    /**
     *
     */
    virtual ~HelloWorldBackendImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

    /**
     *
     */
    virtual int test(tars::TarsCurrentPtr current) { return 0;};
	virtual int backend_test(const Test::backendReq & req,Test::backendResp &resp,tars::TarsCurrentPtr current);
};
/////////////////////////////////////////////////////
#endif
