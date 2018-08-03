/*!
*   \brief      �汾����������
*   \details    ���ڹ������л�վ���ն˵İ汾������
*   \file       system/czupdatemanager.h
*   \author     cuzn
*   \version    0.1.0
*
* ������ʷ
*
* �汾��|˵��|�޶���|�޶�����
* ------|----|------|--------
* v0.1.0|����|cuzn|2016/04/05
*
*/
#ifndef CZ_UPDATEMANAGER_H
#define CZ_UPDATEMANAGER_H

#ifndef _STRING_
#include <string>
#endif //#ifndef _STRING_

#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif //#ifndef _UNORDERED_MAP_

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZSite;

/*! \brief �汾����������
*   \details ���ڹ������л�վ���ն˵İ汾����
*/
class CZUpdateManager
{
    enum
    {
        single_packet_length = 200
    };

    struct Progress_Unit
    {
        CZUChar *current_ptr; /*! \internal ��¼����վ��ǰ�������ȡ�
                              *     ÿ���·�ʱ�����Ӹ�ָ�������single_packet_length�ֽڡ�
                              *     �յ���վ�ɹ�������Ӧ�����и������¡�
                              */
    };

    typedef std::unordered_map<CZSite*, Progress_Unit> SiteManager;
    typedef SiteManager::value_type SiteNode;

public:
    CZUpdateManager(void);
    ~CZUpdateManager(void);

public:
    /*! ��������������\a CZSiteManager ���á�
    *   \param[in]  bin_path �������ļ�ȫ·��
    */
    CZInt initiateUpdate(const std::string &bin_path);
    
    /*! ע��һ����������վ��ί��\a CZUpdateManager �����������̡�
    *   \param[in] site ��������վ
    */
    CZInt registerSite(CZSite *site);

    /*! ���»�վ��������
    *   \param[in] site �����»�վ
    */
    CZInt updateProgress(CZSite *site);

    /*! ����Ƿ����л�վ���°��·����
    *   \retval 0 ȫ���·����
    *   \retval ��0 δ����·���վ����
    */
    inline CZInt updateFinished() {return siteManager.size();}

private:
    CZUChar *update_bin_data; /*! \internal �������ļ�ԭʼ���� */
    CZUInt bin_length; /*! \internal �������ļ��ܳ� */

    SiteManager siteManager; /*! \internal �������д�������վ���� */
};

#endif //#ifndef CZ_UPDATEMANAGER_H
