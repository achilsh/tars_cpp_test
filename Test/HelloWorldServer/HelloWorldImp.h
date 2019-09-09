#ifndef _HelloWorldImp_H_
#define _HelloWorldImp_H_

#include "servant/Application.h"
#include "HelloWorld.h"
#include "HelloWorldBackend.h"
#include "HelloWorldBckendSecond.h"
#include "promise/promise.h"
#include "promise/when_all.h"





using namespace Test;
using namespace tars;

/**
 *
 *
 */


 using namespace promise;

class HelloWorldImp : public Test::HelloWorld
{
public:
    /**
     *
     */
    virtual ~HelloWorldImp() {}

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
	virtual tars::Int32 my_test(const Test::test_req & in_param,Test::test_resp &out_ret,tars::TarsCurrentPtr current);
	virtual tars::Int32 my_promise_test(const Test::test_req & in_param,Test::test_resp &out_ret,tars::TarsCurrentPtr current);
	virtual tars::Int32 my_serial_promise_test(const Test::test_req & in_param,Test::test_resp &out_ret,tars::TarsCurrentPtr current);

public:
	static HelloWorldBackendPrx GetFirstClientPtr();
	static HelloWorldBckendSecondPrx GetSecondPtr();
public:
	static void HandleParallAll(tars::TarsCurrentPtr current, const promise::Future<promise::Tuple<promise::Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr>, 
                    		   promise::Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr> > > &result);
private:
	static HelloWorldBackendPrx m_ptrBackendClientPtr;	
	static HelloWorldBckendSecondPrx m_ptrBackendClentSend;
};
/////////////////////////////////////////////////////
#endif
