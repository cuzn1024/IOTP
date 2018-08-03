#ifndef CZ_SESSION_H
#define CZ_SESSION_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

class CZITransceiverInterface;

/*!
 * \brief 服务器和基站链路的抽象
 */
class CZSession
{
public:
	/*!
	 * \brief     session构造函数
	 * 
	 * \param     CZITransceiverInterface * transceiverInterface 底层链路抽象
	 * \param     CZUInt session_id 唯一标识
	 */
	CZSession(CZITransceiverInterface *transceiverInterface,CZUInt session_id);

    /*!
     * \brief    session析构函数
     */
    ~CZSession();

public:
	/*!
	 * \brief     收取链路传来的数据(串口&网口)
	 */
    CZInt collectData();

	/*!
	 * \brief     提取指定长度数据
	 *
	 * \param     CZUChar receiveBuffer[]
	 * \param     CZUInt length
     * \param     bool dropData 是否直接丢弃
	 * \exception
	 * \return    CZInt
	 * \remarks   仅提取不删除原数据
	 */
	CZInt getData(CZUChar receiveBuffer[], CZUInt length);
    

	/*!
	 * \brief     移动读指针
	 *
	 * \param     CZUInt length
	 * \exception
	 * \return    CZInt
	 * \remarks   
	 */
    CZInt move(CZUInt length);

	/*!
	 * \brief     发送一帧数据
	 * 
	 * \param     CZUChar dataBuffer[] 数据存储
	 * \param     CZUInt length 数据长度
	 * \return    CZInt 0 成功发送 非0表示异常发送
	 */
	CZInt sendFrame(CZUChar dataBuffer[], CZUInt length);

	/*!
	 * \brief     获取sessionid
	 *
	 * \return    CZUInt
	 */
	inline CZUInt getId(){return id;}

	/*!
	 * \brief     获取状态值
	 * 
	 * \exception
	 * \return    eSessionStatus
	 * \remarks  
	 */
	inline cz_types::eSessionStatus getStatus(){return status;}

	/*!
	 * \brief     设置session状态
	 *
	 * \param     eSessionStatus status
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void setStatus(cz_types::eSessionStatus status){this->status = status;}

	/*!
	 * \brief     刷新接收数据时间
	 *
	 * \param     CZUInt time
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void refreshLastRecvDataTime(CZUInt time){this->lastRecvDataTime = time;}

	/*!
	 * \brief     获取最后刷新时间
	 *
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	inline CZUInt getLastRecvDataTime(){return lastRecvDataTime;}

	/*!
	 * \brief     关闭链路
	 *
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	CZUInt close();

private:    
	/*!
	 *	底层链路抽象类
	 */
    CZITransceiverInterface *transceiverInterface;

	/*!
	 *	session_id
	 */
	CZUInt id;

	/*!
	 *	上一次接收到的数据时间
	 */
	CZUInt lastRecvDataTime;

	/*!
	 *	session状态
	 */
	cz_types::eSessionStatus status;
};

#endif //#ifndef CZ_SESSION_H
