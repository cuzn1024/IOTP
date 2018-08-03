/*!
*   \brief      版本升级管理器
*   \details    用于管理所有基站及终端的版本升级。
*   \file       system/czupdatemanager.h
*   \author     cuzn
*   \version    0.1.0
*
* 更新历史
*
* 版本号|说明|修订者|修订日期
* ------|----|------|--------
* v0.1.0|创建|cuzn|2016/04/05
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

/*! \brief 版本升级管理器
*   \details 用于管理所有基站及终端的版本升级
*/
class CZUpdateManager
{
    enum
    {
        single_packet_length = 200
    };

    struct Progress_Unit
    {
        CZUChar *current_ptr; /*! \internal 记录单基站当前升级进度。
                              *     每次下发时，均从该指针往后读single_packet_length字节。
                              *     收到基站成功接收响应后，自行更新至新。
                              */
    };

    typedef std::unordered_map<CZSite*, Progress_Unit> SiteManager;
    typedef SiteManager::value_type SiteNode;

public:
    CZUpdateManager(void);
    ~CZUpdateManager(void);

public:
    /*! 发起升级请求，由\a CZSiteManager 调用。
    *   \param[in]  bin_path 升级包文件全路径
    */
    CZInt initiateUpdate(const std::string &bin_path);
    
    /*! 注册一个待升级基站，委托\a CZUpdateManager 管理升级进程。
    *   \param[in] site 待升级基站
    */
    CZInt registerSite(CZSite *site);

    /*! 更新基站升级进度
    *   \param[in] site 待更新基站
    */
    CZInt updateProgress(CZSite *site);

    /*! 检查是否所有基站更新包下发完毕
    *   \retval 0 全部下发完毕
    *   \retval 非0 未完成下发基站总数
    */
    inline CZInt updateFinished() {return siteManager.size();}

private:
    CZUChar *update_bin_data; /*! \internal 升级包文件原始数据 */
    CZUInt bin_length; /*! \internal 升级包文件总长 */

    SiteManager siteManager; /*! \internal 包含所有待升级基站容器 */
};

#endif //#ifndef CZ_UPDATEMANAGER_H
