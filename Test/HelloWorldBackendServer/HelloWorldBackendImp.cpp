#include "HelloWorldBackendImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void HelloWorldBackendImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void HelloWorldBackendImp::destroy()
{
    //destroy servant here:
    //...
}

int HelloWorldBackendImp::backend_test(const Test::backendReq & req,Test::backendResp &resp,tars::TarsCurrentPtr current) { 
	int iret = 0;

	TLOGDEBUG("HelloWorldBackendImp::backend_test, req.a: "<< req.a << ", req.b: " << req.b <<endl);
	resp.code = 400;
	resp.msg = "response 400";

	return iret;
}


