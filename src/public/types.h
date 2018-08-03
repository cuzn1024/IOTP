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
	 *	�ڵ�����
	 */
	enum eNodeType
	{
		No_Care = 0,
		Server  = 1,//������
		M_AP    = 10,//M��վ
		R_AP    = 11,//R��վ
		C_AP    = 12,//C��վ
		B_AP    = 13,//���ݻ�վ
		L_AP    = 14,//��λ��վ
		T_AP    = 15,//��ʱ��վ
		Multiplex_AP = 16,//���ϻ�վ
		N_ED    = 30,//��ͨ�ն�
		LP_ED   = 31,//�����ն�
		M_ED    = 32,//�ƶ��ն�
		MLP_ED  = 33//�����ƶ��ն�
	};

	enum eNodeStatus
	{
		/*!
		 *	����̬
		 */
		E_Node_Status_Ready = 0,

		/*!
		 *	����̬
		 */
		E_Node_Status_Running = 2,
        
		/*!
		 *	����̬
		 */
        E_Node_status_Fault = 4,
        
		/*!
		 *	�ŵ�����
		 */
        E_Node_status_Disturb = 8
	};

    enum eFaultType
    {
        ED_WChnDisturb = 0x01, //�ŵ�����(ED)
        ED_WHardFault = 0x02, //����Ӳ������(ED)
        ED_WLinkFault = 0x03, //������·����(ED)
        AP_WChnDisturb = 0x07, //�ŵ�����(AP)
        AP_WHardFault = 0x08, //����Ӳ������(AP)
        AP_WLinkFault = 0x09, //������·����(AP)
        AP_LHardFault = 0x0A, //����Ӳ������(AP)
        AP_LLinkFault = 0x0B, //������·����(AP)
        AP_EdLost = 0x0C      //�ն˶�ʧ(AP)
    };

	/*!
	 *	״̬ö��
	 */
	enum eSessionStatus
	{
		/*!
		*	sessionδ֪
		*/
		E_Session_Status_Unknown,

		/*!
		 *	session����
		 */
		E_Session_Status_Online,

		/*!
		 *	session����
		 */
		E_Session_Status_Offline
	};

	/********************end *********************/

	enum eMessageType
	{
		/*!
		 *	��������Ϣ
		 */
		E_Msg_Type_Client_Request,

		/*!
		 *	��ʱ����
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
		 *	��ͨ��ʱ����
		 */
		E_Task_Normal_Timer  = 1 ,

		/*!
		 *	��ʱ�ش�����
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
