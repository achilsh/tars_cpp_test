#include "HelloWorldBackendServer.h"
#include "HelloWorldBackendImp.h"

using namespace std;

HelloWorldBackendServer g_app;

/////////////////////////////////////////////////////////////////
void
HelloWorldBackendServer::initialize()
{
    //initialize application here:
    //...

    addServant<HelloWorldBackendImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".HelloWorldBackendObj");
}
/////////////////////////////////////////////////////////////////
void
HelloWorldBackendServer::destroyApp()
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
