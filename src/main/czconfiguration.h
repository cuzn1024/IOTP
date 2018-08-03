#ifndef CZCONFIGURATION_H
#define CZCONFIGURATION_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

/*!
 * \brief ���ù�����
 */
class CZConfiguration
{    
public:
    CZConfiguration();
    ~CZConfiguration();

public:

    /*!
     * \brief     ��ʼ������
     *
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt initialize();

	/*!
	 * \brief     ��ȡ����ID
	 *
	 * \exception
	 * \return    CZUShort
	 * \remarks  
	 */
	inline CZUShort getNetworkId(){return networkId;}

	/*!
	 * \brief     �����Ƿ񿪷�
	 *
	 * \exception
	 * \return    bool
	 * \remarks  
	 */
	inline bool isNetworkOpen(){return networkStatus !=0 ?true:false;}

	/*!
	 * \brief     �ı�����״̬
	 *
	 * \param     bool bCloseIt
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt changeNetworkStatus(bool bCloseIt);

	/*!
	 * \brief     ��ȡ�ļ�����int��ʽ
	 *
	 * \param     std::string key
	 * \exception
	 * \return    CZUShort
	 * \remarks  
	 */
	CZUShort getShortFromServerConfig(const std::string &key);

	/*!
	 * \brief     ��ȡ�ļ�����string��ʽ
	 *
	 * \param     std::string key
	 * \exception
	 * \return    std::string
	 * \remarks  
	 */
	std::string getStringFromServerConfig(const std::string &key);

private:

	/*!
	 *	�����Ƿ񿪷�
	 */
	CZUShort networkStatus;

	/*!
	 *	����ID
	 */
	CZUShort networkId;
};

#endif
