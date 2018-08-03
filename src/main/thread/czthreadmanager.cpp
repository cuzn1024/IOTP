#include <thread/czthreadmanager.h>

#include <thread/czprocessorthread.h>
#include <thread/czcollectorthread.h>
#include <thread/czassemblerthread.h>
#include <thread/cztimerthread.h>

#include <utils/czthreadutils.h>

using namespace cz_types;

CZThreadManager::CZThreadManager(void) : running(true), assemblerThread(NULL), collectorThread(NULL), processThread(NULL), timerThread(NULL)
{
}

CZThreadManager::~CZThreadManager(void)
{
    delete collectorThread;
    delete processThread;
    delete assemblerThread;
	delete timerThread;
}

void CZThreadManager::adjustPriority()
{
    CZThreadUtils::adjustPriority(assemblerThread->getThread(), CZThreadUtils::Low);
    CZThreadUtils::adjustPriority(collectorThread->getThread(), CZThreadUtils::Low);
    CZThreadUtils::adjustPriority(processThread->getThread(), CZThreadUtils::Low);
	CZThreadUtils::adjustPriority(timerThread->getThread(), CZThreadUtils::Low);
}

void CZThreadManager::joinAllThread()
{
    processThread->join();
}

CZInt CZThreadManager::startThreads()
{
    processThread = new CZProcessorThread;
    collectorThread = new CZCollectorThread;
    assemblerThread = new CZAssemblerThread;
	timerThread = new CZTimerThread;

    processThread->start();
    collectorThread->start();
    assemblerThread->start();
	timerThread->start();

    adjustPriority();
    return 0;
}
