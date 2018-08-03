#include <czapplication.h>
#include <utils/czmiscs.h>

CZInt main(CZInt argc, CZChar *argv[])
{
    CZMiscs::initialize();

    CZApplication *app = new CZApplication();
    
	app->start();

    return 0;
}
