#ifndef CZ_TYPES_H
#define CZ_TYPES_H

#ifndef _STRING_
#include <string>
#endif //#ifndef _STRING_

#ifndef _IOMANIP_
#include <iomanip>
#endif //#ifndef _IOMANIP_

#ifndef _SSTREAM_
#include <sstream>
#endif //#ifndef _SSTREAM_

#ifndef _MEMORY_
#include <memory>
#endif //#ifndef _MEMORY_

static const unsigned PhysicalAddrLength = 8;

typedef unsigned long long CZULongLong;
typedef unsigned short CZUShort;
typedef unsigned char CZUChar;
typedef unsigned CZUInt;
typedef unsigned long CZULong;
typedef long long CZLongLong;
typedef short CZShort;
typedef char CZChar;
typedef int CZInt;

class CZNode;
typedef std::shared_ptr<CZNode> CZNodePtr;

struct CZPhysicalAddr
{
    CZPhysicalAddr():initialized(false)
    {
    }

    CZPhysicalAddr(const CZUChar mem[]):initialized(true)
    {
        if (mem != 0)
            memcpy(addr, mem, PhysicalAddrLength);
    }

    inline bool empty() const
    {
        if (!initialized) return true;

        CZUChar zero[PhysicalAddrLength] = {0};
        return memcmp(addr, zero, PhysicalAddrLength) == 0;
    }

    friend std::ostream &operator<<(std::ostream &out, const CZPhysicalAddr &addr)
    {
        for (CZInt i = 0; i < PhysicalAddrLength; i++)
        {
            out<<std::hex<<std::uppercase<<std::setfill('0')<<std::setw(2)<<(CZUShort)addr.addr[i];
        }

        return out;
    }

    inline std::string str() const
    {
        std::stringstream ss;
        ss.setf(std::ios::hex, std::ios::basefield);
        ss.setf(std::ios::uppercase);
		for (CZInt i = 0; i < PhysicalAddrLength; i++)
        {
            ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) <<(CZUShort)addr[i];
        }

        return ss.str();
    }

	inline bool operator == (const CZPhysicalAddr &a) const _NOEXCEPT
	{
		return memcmp(addr, a.addr, PhysicalAddrLength) == 0;
	}

    CZUChar addr[PhysicalAddrLength];

    bool initialized;
};

template<typename T>
struct CZPhysicalAddrHash
{
	size_t operator()(const T &addr) const
	{
		return boost::hash_value<CZUChar, PhysicalAddrLength>(addr.addr);
	}
};

class CZSite;
namespace cz_types
{
	/********************new *********************/

	/*!
	 *	节点类型
	 */
	enum eNodeType
	{
		No_Care = 0,
		Server  = 1,//服务器
		M_AP    = 10,//M基站
		R_AP    = 11,//R基站
		C_AP    = 12,//C基站
		B_AP    = 13,//备份基站
		L_AP    = 14,//定位基站
		T_AP    = 15,//授时基站
		Multiplex_AP = 16,//复合基站
		N_ED    = 30,//普通终端
		LP_ED   = 31,//休眠终端
		M_ED    = 32,//移动终端
		MLP_ED  = 33//休眠移动终端
	};

	enum eNodeStatus
	{
		/*!
		 *	就绪态
		 */
		E_Node_Status_Ready = 0,

		/*!
		 *	运行态
		 */
		E_Node_Status_Running = 2,
        
		/*!
		 *	故障态
		 */
        E_Node_status_Fault = 4,
        
		/*!
		 *	信道干扰
		 */
        E_Node_status_Disturb = 8
	};

    enum eFaultType
    {
        ED_WChnDisturb = 0x01, //信道干扰(ED)
        ED_WHardFault = 0x02, //无线硬件故障(ED)
        ED_WLinkFault = 0x03, //无线链路故障(ED)
        AP_WChnDisturb = 0x07, //信道干扰(AP)
        AP_WHardFault = 0x08, //无线硬件故障(AP)
        AP_WLinkFault = 0x09, //无线链路故障(AP)
        AP_LHardFault = 0x0A, //有线硬件故障(AP)
        AP_LLinkFault = 0x0B, //有线链路故障(AP)
        AP_EdLost = 0x0C      //终端丢失(AP)
    };

	/*!
	 *	状态枚举
	 */
	enum eSessionStatus
	{
		/*!
		*	session未知
		*/
		E_Session_Status_Unknown,

		/*!
		 *	session在线
		 */
		E_Session_Status_Online,

		/*!
		 *	session离线
		 */
		E_Session_Status_Offline
	};

	/********************end *********************/

	enum eMessageType
	{
		/*!
		 *	请求处理消息
		 */
		E_Msg_Type_Client_Request,

		/*!
		 *	定时任务
		 */
		E_Msg_Type_TimerTask
	};

    struct RequestMessage
    {
		eMessageType msg_type;
		CZUShort session_id;
        CZUShort src_addr;
		CZUShort dst_addr;
		CZUChar data_id;
		CZUShort data_len;
		CZUChar data[1];
    };

	struct ResponseMessage
	{
		CZUShort session_id;
		CZUShort dst_addr;
		CZUChar data_id;
		CZUShort data_len;
		CZUChar data[1];
	};

	struct RequestFrame
	{
		CZUInt session_id;
		CZUInt message_type;
		CZUShort message_len;
		CZUChar data[1];
	};

	struct ResponseFrame
	{
		CZInt session_id;
		CZUShort message_len;
		CZUChar data[1];
	};

#pragma pack(push)
#pragma pack(1)

	enum eTaskType
	{
		/*!
		 *	普通定时任务
		 */
		E_Task_Normal_Timer  = 1 ,

		/*!
		 *	超时重传任务
		 */
		E_Task_Timeout_Retry
	};

	struct TimerTask
	{
		bool isAddTask;
		eTaskType type;		
		CZUShort session_id;
		CZUShort dst_addr;
		CZULongLong millisecond;
		CZUChar data_id;
		CZUInt retryTimes;
		CZUShort data_len;
		CZUChar data[1];
	};

	struct QtyInfo
	{
		CZUShort freId;
		CZUChar rssi;
		CZUChar snr;
	};
#pragma pack(pop)
}

#endif //#ifndef CZ_TYPES_H
