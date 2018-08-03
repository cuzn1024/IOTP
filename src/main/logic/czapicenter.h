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
 * \brief �߼�����
 */
class CZApiCenter
{
public:
    /*!
     * \brief     ���캯��
     * 
     * \exception
     * \return    
     * \remarks  
     */
    CZApiCenter();

    /*!
     * \brief     ��������
     * 
     * \exception
     * \return    
     * \remarks  
     */
    ~CZApiCenter();
	
	/*!
	 * \brief     ��ʼ��
	 * 
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void init();
	
private:

	/*!
	 *	���ؽ������
	 */
	CZUChar *responseBuffer;

	/*!
	 *	���ؽ������
	 */
	CZUShort bufferLen;
};

#endif //#ifndef CZApiCenter_H

#endif //#ifdef THRIFT_ENABLED
