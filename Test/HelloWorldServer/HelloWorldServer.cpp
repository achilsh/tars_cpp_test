#include "HelloWorldServer.h"
#include "HelloWorldImp.h"

using namespace std;

HelloWorldServer g_app;

/////////////////////////////////////////////////////////////////
void
HelloWorldServer::initialize()
{
    //initialize application here:
    //...

    addServant<HelloWorldImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".HelloWorldObj");
}
/////////////////////////////////////////////////////////////////
void
HelloWorldServer::destroyApp()
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
