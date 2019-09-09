#include "HelloWorldBckendSecondImp.h"
#include "servant/Application.h"

using namespace std;

//////////////////////////////////////////////////////
void HelloWorldBckendSecondImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void HelloWorldBckendSecondImp::destroy()
{
    //destroy servant here:
    //...
}


int HelloWorldBckendSecondImp::test_second(const Test::SecondReq & req,Test::SecondResp &resp,tars::TarsCurrentPtr current) {
	int iret = 0;
	TLOGDEBUG("test_second(), req: " << req.i_a << ", str: " << req.str_b << std::endl);
	resp.code = 11111;
	resp.msg = "11111 response";
	TLOGDEBUG("test_second(), response: " << resp.code << ", " << resp.msg  << std::endl);
	return iret;
}

