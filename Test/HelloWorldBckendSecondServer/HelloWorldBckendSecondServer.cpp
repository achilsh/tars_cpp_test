#include "HelloWorldBckendSecondServer.h"
#include "HelloWorldBckendSecondImp.h"

using namespace std;

HelloWorldBckendSecondServer g_app;

/////////////////////////////////////////////////////////////////
void
HelloWorldBckendSecondServer::initialize()
{
    //initialize application here:
    //...

    addServant<HelloWorldBckendSecondImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".HelloWorldBckendSecondObj");
}
/////////////////////////////////////////////////////////////////
void
HelloWorldBckendSecondServer::destroyApp()
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
