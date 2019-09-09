#include "HelloWorldImp.h"
#include "servant/Application.h"

using namespace std;
using namespace tars;
using namespace Test;

HelloWorldBackendPrx HelloWorldImp::m_ptrBackendClientPtr = NULL;
HelloWorldBckendSecondPrx HelloWorldImp::m_ptrBackendClentSend = NULL;

HelloWorldBackendPrx HelloWorldImp::GetFirstClientPtr()  {
	if (m_ptrBackendClientPtr == NULL)  {
		m_ptrBackendClientPtr = Application::getCommunicator()->stringToProxy<HelloWorldBackendPrx>("Test.HelloWorldBackendServer.HelloWorldBackendObj");
	}
	return m_ptrBackendClientPtr;
}

HelloWorldBckendSecondPrx HelloWorldImp::GetSecondPtr() {
	if (m_ptrBackendClentSend == NULL) {
		m_ptrBackendClentSend = Application::getCommunicator()->stringToProxy<HelloWorldBckendSecondPrx>("Test.HelloWorldBckendSecondServer.HelloWorldBckendSecondObj");
	}
	return m_ptrBackendClentSend;
}

//第二次调用异步callback 类型
class TestHellWordSecndCB: public Test::HelloWorldBckendSecondPrxCallback {
public:
	TestHellWordSecndCB(TarsCurrentPtr &current): m_currentPtr(current) {}
	virtual ~TestHellWordSecndCB() {}

	virtual void callback_test_second(tars::Int32 ret,  const Test::SecondResp& resp) {

		Test::test_resp resp_front;
		resp_front.code = resp.code;
		resp_front.msg = resp.msg;

		TLOGDEBUG("recv second backend response, code: " << resp.code << ", msg: " << resp.msg <<std::endl);
		HelloWorld::async_response_my_test(m_currentPtr, ret, resp_front);
	}
	
	virtual void callback_test_second_exception(tars::Int32 ret) {
		Test::test_resp resp_front;
		resp_front.code = -1;
		resp_front.msg = "second rpc tmout or exception";

		TLOGDEBUG("recv send req backend tmout");
		HelloWorld::async_response_my_test(m_currentPtr, ret, resp_front);
	}

private:
	TarsCurrentPtr m_currentPtr;
};

//第一次异步回调callback对象
class TestHelloWorldCB: public Test::HelloWorldBackendPrxCallback {
public:
	TestHelloWorldCB(int x, TarsCurrentPtr &current)
		:m_iX(x), m_current(current) {}
		
	virtual ~TestHelloWorldCB() {}

	virtual void callback_backend_test(tars::Int32 ret,  const Test::backendResp& resp) {

		Test::test_resp resp_front;
		resp_front.code = resp.code;
		resp_front.msg = resp.msg;

		TLOGDEBUG("recv first backend response, code: " << resp.code << ", msg: " << resp.msg <<std::endl);

		Test::HelloWorldBckendSecondPrxCallbackPtr senconde_cb = new TestHellWordSecndCB(m_current);

		Test::SecondReq second_req;
		second_req.i_a = 5656;
		second_req.str_b = "second req";
		
		HelloWorldImp::GetSecondPtr()->async_test_second(senconde_cb, second_req);
		TLOGDEBUG("send second async rpc, sencod a: " << second_req.i_a << ", second str: " << second_req.str_b <<std::endl);
	}
	
	virtual void callback_backend_test_exception(tars::Int32 ret) {
		Test::test_resp resp_front;
		resp_front.code = -1;
		resp_front.msg = "first aysnc timeout or exception";
		
		TLOGDEBUG("recv backend tmout");
		HelloWorld::async_response_my_test(m_current, ret, resp_front);

	}
	///

private:
	int m_iX;
	TarsCurrentPtr m_current;
};

//////////////////////////////////////////////////////
void HelloWorldImp::initialize()
{
	std::cout << "initial" << std::endl;
    //initialize servant here:
    //...

	GetFirstClientPtr();
	GetSecondPtr();
	
	//可以用单例来实现全局的client	
   // m_ptrBackendClientPtr = Application::getCommunicator()->stringToProxy<HelloWorldBackendPrx>("Test.HelloWorldBackendServer.HelloWorldBackendObj");
   // m_ptrBackendClentSend = Application::getCommunicator()->stringToProxy<HelloWorldBackendPrx>("Test.HelloWorldBckendSecondServer.HelloWorldBckendSecondObj")
	
}

//////////////////////////////////////////////////////
void HelloWorldImp::destroy()
{
    //destroy servant here:
    //...
}

int HelloWorldImp::my_test(const Test::test_req & in_param,Test::test_resp &out_ret,tars::TarsCurrentPtr current) {
	int iRet = 0;
	TLOGDEBUG("HelloImp::my_test:"<< in_param.str_b << std::endl);
	if (in_param.a == 0) {
		std::cout << "input a is 0 " << std::endl;
		out_ret.code = 100;
		out_ret.msg = "input is 0 ";
		return iRet;
	}

	//调用一次下游服务
	current->setResponse(false);
	Test::HelloWorldBackendPrxCallbackPtr ptr_cb = new TestHelloWorldCB(in_param.a, current);
	
	Test::backendReq  bkend_req;
	bkend_req.b = in_param.a;
	bkend_req.a = in_param.str_b;

	TLOGDEBUG("HelloImp::my_test, begin to call next server by aysnc" << std::endl);

	//异步调用下游服务
	GetFirstClientPtr()->async_backend_test(ptr_cb, bkend_req);	

	return iRet;
}


void HandleParallAllG(tars::TarsCurrentPtr current, const promise::Future<promise::Tuple<promise::Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr>, 
                    		   promise::Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr> > > &result) {
	promise::Tuple<promise::Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr>, promise::Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr> > out = result.get();

    HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr f1 = out.get<0>().get();
 
    HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr f2 =  out.get<1>().get();

	int iret = (f1)->resp.code + (f2)->resp.code;
	std::string msg =(f1)->resp.msg + "_____" + (f2)->resp.msg;

	Test::test_resp resp_test;
	resp_test.code = iret;
	resp_test.msg = msg;
	
	TLOGDEBUG("send response after promise, ret: " << iret << ", msg: " << msg << std::endl);
	HelloWorld::async_response_my_promise_test(current, iret, resp_test);

}

//采用promise并行调用其他两个服务,并行调用 下游两服务，等待两个异步结果返回后再 继续做后续逻辑
tars::Int32 HelloWorldImp::my_promise_test(const Test::test_req & in_param,Test::test_resp &out_ret,tars::TarsCurrentPtr current) {
	int iret = 0;
	current->setResponse(false);
	
	Test::backendReq req_in;
	req_in.b = in_param.a;
	req_in.a = in_param.str_b;
	
	map<std::string, std::string> mp_cntx;
	const promise::Future< HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr > f1 = \
								GetFirstClientPtr()->promise_async_backend_test(req_in, mp_cntx);

	Test::SecondReq second_req;
	second_req.i_a = 8981;
	second_req.str_b = "second promise req 8981";

	map<string, string> second_cntx;
	const promise::Future< HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr > f2 = \
						GetSecondPtr()->promise_async_test_second(second_req, second_cntx);

	Future<Tuple<Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr>,
		        Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr> > > r = promise::whenAll(f1, f2);
        
	r.then(tars::TC_Bind(&HelloWorldImp::HandleParallAll, current));
	return iret;
}

void HelloWorldImp::HandleParallAll(tars::TarsCurrentPtr current, const Future<Tuple<Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr>, 
                    	 			Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr> > > &result) {
                    	 			
	Tuple<Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr>, 
		  Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr> > out = result.get();
	
    HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr f1 = out.get<0>().get();
    HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr f2 =  out.get<1>().get();

	int iret = (f1)->resp.code + (f2)->resp.code;
	std::string msg =(f1)->resp.msg + "_____" + (f2)->resp.msg;

	Test::test_resp resp_test;
	resp_test.code = iret;
	resp_test.msg = msg;
	
	TLOGDEBUG("send response after promise, ret: " << iret << ", msg: " << msg << std::endl);
	HelloWorld::async_response_my_promise_test(current, iret, resp_test);

}

////////////////////
promise::Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr>
handleFirstRspAndSendCReq(TarsCurrentPtr current, const promise::Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr>& future) {

	HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr f1 = future.get();

	TLOGDEBUG("recv first promise ret, code : " << f1->resp.code << ", data: " << f1->resp.msg << std::endl);

	Test::SecondReq second_req;
	second_req.i_a = f1->resp.code +  9000;
	second_req.str_b = f1->resp.msg + "second promise req 8981";

	map<string, string> second_cntx;
	const promise::Future< HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr > f2 = \
						HelloWorldImp::GetSecondPtr()->promise_async_test_second(second_req, second_cntx);
	TLOGDEBUG("handl first node promise ret and send second promise req, sencode req: " <<  second_req.str_b << std::endl);
	return f2;
}

int handleLastAndReturnClient(TarsCurrentPtr current, const promise::Future<HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr>& future)
{
	HelloWorldBckendSecondPrxCallbackPromise::Promisetest_secondPtr f2 = future.get();
	TLOGDEBUG("recv second promise ret, code: " << f2->resp.code << ", msg: " << f2->resp.msg << std::endl);

	int iret = 200 + (f2)->resp.code;
	std::string msg = (f2)->resp.msg + "_____" + ",test sealize data";

	Test::test_resp resp_test;
	resp_test.code = iret;
	resp_test.msg = msg;

	TLOGDEBUG("send response to client, ret: " << iret << ", msg: " << msg << std::endl);
	HelloWorld::async_response_my_promise_test(current, iret, resp_test);
	return 0;
}


////采用promise串行调用其他两个服务,一个服务 返回结果后才做下一个服务调用。最后做剩下的逻辑
tars::Int32 HelloWorldImp::my_serial_promise_test(const Test::test_req & in_param,Test::test_resp &out_ret,tars::TarsCurrentPtr current)  {
	int ret = 0;
	current->setResponse(false);
	TLOGDEBUG("begin to call seralize promise test..."<< std::endl);

	Test::backendReq req_in;
	req_in.b = in_param.a;
	req_in.a = in_param.str_b;
		
	map<std::string, std::string> mp_cntx;
	const promise::Future<HelloWorldBackendPrxCallbackPromise::Promisebackend_testPtr > f1 = \
									HelloWorldImp::GetFirstClientPtr()->promise_async_backend_test(req_in, mp_cntx);
	//
	f1.then(tars::TC_Bind(&handleFirstRspAndSendCReq, current)).then(tars::TC_Bind(&handleLastAndReturnClient, current));

	
	return ret;
}
											




