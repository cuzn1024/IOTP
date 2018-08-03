#ifndef CZTHREADUTILS_H
#define CZTHREADUTILS_H

#ifndef _THREAD_
#include <thread>
#endif //#ifndef _THREAD_

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZThreadUtils
{
public:
    enum Thread_Priority
    {
        Idle = 0,
        Low,
        BelowNormal,
        Normal,
        AboveNormal,
        High,
        RealTime
    };

private:
    CZThreadUtils(void);

public:
    static CZInt adjustPriority(std::thread &bthread, Thread_Priority priority);
};

#endif //#ifndef CZTHREADUTILS_H
