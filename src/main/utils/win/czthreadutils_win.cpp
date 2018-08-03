#include <utils/czthreadutils.h>

#include <Windows.h>

using namespace std;

CZInt CZThreadUtils::adjustPriority(thread &bthread, Thread_Priority priority)
{
    CZInt threadPriority = THREAD_PRIORITY_NORMAL;

    switch (priority)
    {
    case RealTime:
        threadPriority = THREAD_PRIORITY_TIME_CRITICAL;
        break;
    case BelowNormal:
        threadPriority = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case Low:
        threadPriority = THREAD_PRIORITY_LOWEST;
        break;
    case Idle:
        threadPriority = THREAD_PRIORITY_IDLE;
        break;
    }

    return SetThreadPriority(bthread.native_handle(), threadPriority);
}
