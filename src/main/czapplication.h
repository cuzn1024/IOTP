#ifndef CZ_APPLICATION
#define CZ_APPLICATION

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZApplication
{
public:
    CZApplication();
    ~CZApplication();

public:
    CZInt start();

protected:
    CZInt run();
    CZInt initialize();
    CZInt initializeMiscs();
    CZInt initializeSingleton();

    CZInt startThreads();
};

#endif //#ifndef CZ_APPLICATION
