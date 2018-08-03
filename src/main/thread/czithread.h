#ifndef CZ_ITHREAD_H
#define CZ_ITHREAD_H

#ifndef _THREAD_
#include <thread>
#endif //#ifndef _THREAD_

class CZIThread
{
public:
    virtual ~CZIThread(void) {}

public:
    virtual void start() { innerThread = std::thread(std::bind(&CZIThread::run, this)); }
    virtual void join() { innerThread.join(); }
    inline std::thread &getThread() { return innerThread; }

protected:
    virtual void run() = 0;

protected:
    std::thread innerThread;
};

#endif //#ifndef CZ_ITHREAD_H
