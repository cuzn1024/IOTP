#ifndef CZCONFIGURATION_H
#define CZCONFIGURATION_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

/*!
 * \brief 配置管理类
 */
class CZConfiguration
{    
public:
    CZConfiguration();
    ~CZConfiguration();

public:

    /*!
     * \brief     初始化函数
     *
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt initialize();

	/*!
	 * \brief     获取网络ID
	 *
	 * \exception
	 * \return    CZUShort
	 * \remarks  
	 */
	inline CZUShort getNetworkId(){return networkId;}

	/*!
	 * \brief     网络是否开放
	 *
	 * \exception
	 * \return    bool
	 * \remarks  
	 */
	inline bool isNetworkOpen(){return networkStatus !=0 ?true:false;}

	/*!
	 * \brief     改变网络状态
	 *
	 * \param     bool bCloseIt
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt changeNetworkStatus(bool bCloseIt);

	/*!
	 * \brief     获取文件配置int形式
	 *
	 * \param     std::string key
	 * \exception
	 * \return    CZUShort
	 * \remarks  
	 */
	CZUShort getShortFromServerConfig(const std::string &key);

	/*!
	 * \brief     获取文件配置string形式
	 *
	 * \param     std::string key
	 * \exception
	 * \return    std::string
	 * \remarks  
	 */
	std::string getStringFromServerConfig(const std::string &key);

private:

	/*!
	 *	网络是否开放
	 */
	CZUShort networkStatus;

	/*!
	 *	网络ID
	 */
	CZUShort networkId;
};

#endif
