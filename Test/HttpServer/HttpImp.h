#ifndef _HttpImp_H_
#define _HttpImp_H_
#include "HelloWorld.h"


#include "servant/Application.h"
using namespace Test;



using namespace tars;

class HttpImp : public tars::Servant
{
public:
    /**
     *
     */
    virtual ~HttpImp() {}

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
    virtual int doRequest(TarsCurrentPtr current, vector<char> &buffer) override;
public:
	static HelloWorldPrx GetNextClientHandle();
	
private:
	static HelloWorldPrx m_clientHandlePtr;
};
/////////////////////////////////////////////////////
#endif
