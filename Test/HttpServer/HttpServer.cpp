#include "HttpServer.h"
#include "HttpImp.h"

using namespace std;

HttpServer g_app;



struct HttpParser {
	static int ParseHttp(std::string &in, std::string &out){
		try {
			bool bret = TC_HttpRequest::checkRequest(in.c_str(), in.length());
			if (bret) {
				out = in;
				in = "";
				return TC_EpollServer::PACKET_FULL;
			} else {
				return TC_EpollServer::PACKET_LESS;
			}


			}catch(const std::exception& ex) {
				return TC_EpollServer::PACKET_ERR;
			}
			return TC_EpollServer::PACKET_LESS;
	}

};

/////////////////////////////////////////////////////////////////
void
HttpServer::initialize()
{
    //initialize application here:
    //...

    addServant<HttpImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".HttpObj");
	addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".HttpObj", &HttpParser::ParseHttp);
}
/////////////////////////////////////////////////////////////////
void
HttpServer::destroyApp()
{
    //destroy application here:
    //...
}
/////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
