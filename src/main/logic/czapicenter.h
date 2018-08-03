#ifdef THRIFT_ENABLED

#ifndef CZAPICENTER_H
#define CZAPICENTER_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#include "logicfunctionserviceimpl.h"

/*!
 * \brief 逻辑处理
 */
class CZApiCenter
{
public:
    /*!
     * \brief     构造函数
     * 
     * \exception
     * \return    
     * \remarks  
     */
    CZApiCenter();

    /*!
     * \brief     析构函数
     * 
     * \exception
     * \return    
     * \remarks  
     */
    ~CZApiCenter();
	
	/*!
	 * \brief     初始化
	 * 
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void init();
	
private:

	/*!
	 *	返回结果缓存
	 */
	CZUChar *responseBuffer;

	/*!
	 *	返回结果长度
	 */
	CZUShort bufferLen;
};

#endif //#ifndef CZApiCenter_H

#endif //#ifdef THRIFT_ENABLED
