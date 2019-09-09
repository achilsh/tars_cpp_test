#include "HttpImp.h"
#include "servant/Application.h"

using namespace std;
using namespace Test;
using namespace tars;


HelloWorldPrx HttpImp::m_clientHandlePtr = NULL;

HelloWorldPrx HttpImp::GetNextClientHandle() {
	if (m_clientHandlePtr == NULL) {
		m_clientHandlePtr = Application::getCommunicator()->stringToProxy<HelloWorldPrx>("Test.HelloWorldServer.HelloWorldObj");
	}
	return m_clientHandlePtr;
}

class NextClientCB : public HelloWorldPrxCallback{ 
public:
	NextClientCB(tars::TarsCurrentPtr current) : m_currentPtr(current) {}
	virtual ~NextClientCB() {

	}

	void callback_my_test(tars::Int32 ret,  const Test::test_resp& out_ret) {
		std::string resp_str = out_ret.msg + ", async ret http ret";
		
		TC_HttpResponse resp_http;
		resp_http.setResponse(resp_str.c_str(), resp_str.size());
		
		std::vector<char> http_str;
		resp_http.encode(http_str);
		
		m_currentPtr->sendResponse(&http_str[0], http_str.size());
		TLOGDEBUG("async recv callback ret, now send http reponse  to client, ret: " << resp_str << std::endl);
	}

	
	virtual void callback_my_test_exception(tars::Int32 ret) {

	}
	
private:
	tars::TarsCurrentPtr m_currentPtr;

};

//////////////////////////////////////////////////////
void HttpImp::initialize()
{
    //initialize servant here:
    //...
}

//////////////////////////////////////////////////////
void HttpImp::destroy()
{
    //destroy servant here:
    //...
}


int HttpImp::doRequest(tars::TarsCurrentPtr current, vector<char> &reponse) {
	int ret = 0;
	TC_HttpRequest req;
	std::vector<char> vBuf =  current->getRequestBuffer();
	
	std::string sBuf;
	sBuf.assign(&vBuf[0], vBuf.size());
	req.decode(sBuf);
	TLOGDEBUG("recv http data: "  << sBuf << std::endl);
	
	//TC_HttpResponse resp;
	//std::string resp_val = "hello world";

	Test::test_req in_param;
	in_param.a = 23456;
	in_param.str_b = "str_123456";
	current->setResponse(false);
	

	HelloWorldPrxCallbackPtr next_cb_ptr = new NextClientCB(current);
	HttpImp::GetNextClientHandle()->async_my_test(next_cb_ptr, in_param);
	return ret;
}


