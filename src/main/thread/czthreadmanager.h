#ifndef CZ_THREADMANAGER_H
#define CZ_THREADMANAGER_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZIThread;
class CZSession;
class CZThreadManager
{
    friend class CZSingletonManager;
private:
    CZThreadManager(void);
    ~CZThreadManager(void);

public:
    CZInt startThreads();
    void joinAllThread();
    inline bool stillRunning() { return running; }

private:
    void adjustPriority();

private:
    bool running;

    CZIThread *processThread, *collectorThread, *assemblerThread, *timerThread;
};

#endif //#ifndef CZ_THREADMANAGER_H
