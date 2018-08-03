#ifndef CZ_TIMERTHREAD_H
#define CZ_TIMERTHREAD_H

#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif //#ifndef _UNORDERED_MAP_

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_ITHREAD_H
#include <thread/czithread.h>
#endif //#ifndef CZ_ITHREAD_H

using namespace cz_types;

class CZTimerThread : public CZIThread
{
public:
    CZTimerThread(void);
    ~CZTimerThread(void);

protected:
    void run();

private:

	/*!
	 *	存储task结构数据
	 */
	CZUChar *taskBuffer;
	/*!
	 *	临时存储超时重传任务
	 */
	cz_types::TimerTask* task;

	/*!
	 *	定时任务数据存储
	 */
	CZUChar *reqBuffer;

	/*!
	 *	定时任务模拟请求数据结构
	 */
	cz_types::RequestMessage* req;

	/*!
	 *	时间戳key
	 *	节点网络地址 value 
	 */
	std::unordered_map<CZULongLong,CZUShort> taskTimeMap;

	/*!
	 *	节点网络地址+data_id 为key
	 *	对应任务的时间 为value
	 */
	std::unordered_map<std::string,CZULongLong> taskKeyMap;
};

#endif //#ifndef CZ_TIMERTHREAD_H
