#include <iostream>
#include "servant/Communicator.h"

#include "HelloWorld.h"

using namespace std;
using namespace Test;
using namespace tars;

void  SyncTest() {
	
	tars::Communicator comm;
    try
    {
        HelloWorldPrx  prx;
        comm.stringToProxy("Test.HelloWorldServer.HelloWorldObj@tcp -h 192.168.0.110 -p 20000" , prx);

        try
        {
			Test::test_req req;
			req.a = 1000;
			req.str_b = "str_b_inparam";
			Test::test_resp resp;	

            int iRet = prx->my_test(req, resp);
           	std::cout << "ret: " << iRet << ", code: " << resp.code << ", msg: " << resp.msg << std::endl;

        }
        catch(exception &ex)
        {
            cerr << "ex:" << ex.what() << endl;
        }
        catch(...)
        {
            cerr << "unknown exception." << endl;
        }
    }
    catch(exception& e)
    {
        cerr << "exception:" << e.what() << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << endl;
    }

}


void Asyn_test() {

	//每个接口的回调用一个类来封装
	class AsynTestCB: public HelloWorldPrxCallback  {
		public:
			AsynTestCB(int y): m_iY(y){}
			virtual ~AsynTestCB() {}

			void callback_test(tars::Int32 ret) {	
				m_iY += ret;
				std::cout << "call cb_test, test callback , y: " << m_iY << ", ret: " << ret << std::endl;
				
			}
		
			void callback_test_exception(tars::Int32 ret) {
				std::cout << "test exception or time out, ret: " << ret << std::endl;
			}
			int GetY() const {
				return m_iY + 1;
			}
		private:
			int m_iY;
	};
	
	/////

	//每个接口的回调用一个类来封装
	class HWAsynCB: public HelloWorldPrxCallback {
		public:
			HWAsynCB(int x): m_iX(x) {}
			
			virtual ~HWAsynCB() {}
			
			//实现接口回调，给框架调用
			void callback_my_test(tars::Int32 ret,  const Test::test_resp& out_ret) override {
			
				//打算在请求返回的回调接口中更新成员变量 m_iX值。
				std::cout << "ret: " << ret << ", resp code: " << out_ret.code << ", resp msg: " << out_ret.msg << std::endl;
				std::cout << "val + ret: " << m_iX + ret << std::endl;
				m_iX += out_ret.code;
				std::cout << "val + code: " << m_iX << std::endl;
			}
			
			void callback_my_test_exception(tars::Int32 ret) override {
				std::cout << "tm_out or excepton, ret: " << ret << std::endl;
			}

			//
			int GetX() { return m_iX + 1; }
		private:
			int m_iX;
	
	};
    try
    {
    
	HelloWorldPrx   prx;
	CommunicatorPtr c;
	
#ifdef conn_direct 
	c = new Communicator();
	c->stringToProxy("Test.HelloWorldServer.HelloWorldObj@tcp -h 192.168.0.110 -p 20000" , prx);

#else 
	c = new Communicator();
	c->setProperty("locator", "tars.tarsregistry.QueryObj@tcp -h 192.168.0.110 -p 17890");
	c->stringToProxy("Test.HelloWorldServer.HelloWorldObj",  prx);
#endif
 
        try
        {
			Test::test_req req;
			req.a = 2000;
			req.str_b = "str_callback_inparam";
			Test::test_resp resp;

			//异步接口
			int mytest_in = 5000;
            HelloWorldPrxCallbackPtr cb = new HWAsynCB(mytest_in);
            prx->async_my_test(cb, req);

			//异步另外接口
			int test_in = 2000;
			//HelloWorldPrxCallbackPtr test_cb = new AsynTestCB(test_in);
			//prx->async_test(test_cb);
			sleep(3);

			std::cout << "my_test_aync value: " <<  dynamic_cast<HWAsynCB*>(cb.get())->GetX() << std::endl;
			//std::cout << "test_async value: " << dynamic_cast<AsynTestCB*>(test_cb.get())->GetY() <<std::endl;
        }
        catch(const exception &ex)
        {
            cerr<<"ex:"<<ex.what() <<endl;
        }
        catch(...)
        {
            cerr<<"unknown exception."<<endl;
        }
    }
    catch(exception& e)
    {
        cerr<<"exception:"<<e.what() <<endl;
    }
    catch (...)
    {
        cerr<<"unknown exception."<<endl;
    }
			
}

//测试后台服务并行promise 接口
void PromiseTest() {
	tars::Communicator comm;
    try
    {
        HelloWorldPrx  prx;
        comm.stringToProxy("Test.HelloWorldServer.HelloWorldObj@tcp -h 192.168.0.110 -p 20000" , prx);

        try
        {
			Test::test_req req;
			req.a = 8989;
			req.str_b = "str_b_inparam";
			Test::test_resp resp;	


            int iRet = prx->my_promise_test(req, resp);
           	std::cout << "ret: " << iRet << ", code: " << resp.code << ", msg: " << resp.msg << std::endl;

        }
        catch(exception &ex)
        {
            cerr << "ex:" << ex.what() << endl;
        }
        catch(...)
        {
            cerr << "unknown exception." << endl;
        }
    }
    catch(exception& e)
    {
        cerr << "exception:" << e.what() << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << endl;
    }


}

//测试服务内串行promise
void PromiseSializeTest() {
	tars::Communicator comm;
    try
    {
        HelloWorldPrx  prx;
        comm.stringToProxy("Test.HelloWorldServer.HelloWorldObj@tcp -h 192.168.0.110 -p 20000" , prx);

        try
        {
			Test::test_req req;
			req.a = 8989;
			req.str_b = "str_b_inparam";
			Test::test_resp resp;	


            int iRet = prx->my_serial_promise_test(req, resp);
           	std::cout << "ret: " << iRet << ", code: " << resp.code << ", msg: " << resp.msg << std::endl;

        }
        catch(exception &ex)
        {
            cerr << "ex:" << ex.what() << endl;
        }
        catch(...)
        {
            cerr << "unknown exception." << endl;
        }
    }
    catch(exception& e)
    {
        cerr << "exception:" << e.what() << endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << endl;
    }

}

int main() {

	//SyncTest();
	//Asyn_test();
	//PromiseTest();
	PromiseSializeTest();
	return 0;
}
