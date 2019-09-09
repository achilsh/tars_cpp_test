#ifndef _HelloWorldBckendSecondImp_H_
#define _HelloWorldBckendSecondImp_H_

#include "servant/Application.h"
#include "HelloWorldBckendSecond.h"

/**
 *
 *
 */
class HelloWorldBckendSecondImp : public Test::HelloWorldBckendSecond
{
public:
    /**
     *
     */
    virtual ~HelloWorldBckendSecondImp() {}

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
	virtual int test_second(const Test::SecondReq & req,Test::SecondResp &resp,tars::TarsCurrentPtr current);
};
/////////////////////////////////////////////////////
#endif
