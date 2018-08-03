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
	 *	�洢task�ṹ����
	 */
	CZUChar *taskBuffer;
	/*!
	 *	��ʱ�洢��ʱ�ش�����
	 */
	cz_types::TimerTask* task;

	/*!
	 *	��ʱ�������ݴ洢
	 */
	CZUChar *reqBuffer;

	/*!
	 *	��ʱ����ģ���������ݽṹ
	 */
	cz_types::RequestMessage* req;

	/*!
	 *	ʱ���key
	 *	�ڵ������ַ value 
	 */
	std::unordered_map<CZULongLong,CZUShort> taskTimeMap;

	/*!
	 *	�ڵ������ַ+data_id Ϊkey
	 *	��Ӧ�����ʱ�� Ϊvalue
	 */
	std::unordered_map<std::string,CZULongLong> taskKeyMap;
};

#endif //#ifndef CZ_TIMERTHREAD_H
