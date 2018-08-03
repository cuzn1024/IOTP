#include <system/czupdatemanager.h>

#include <fstream>

#include <boost/filesystem.hpp>

CZUpdateManager::CZUpdateManager(void):update_bin_data(0)
{
}

CZUpdateManager::~CZUpdateManager(void)
{
    delete []update_bin_data;
    update_bin_data = 0;
    bin_length = 0;
}

CZInt CZUpdateManager::initiateUpdate(const std::string &bin_path)
{
    if (!siteManager.empty()) return -1;

    namespace bf = boost::filesystem;

    bf::path full_path(bin_path);
    
    if (!bf::exists(full_path)) return -2;

    std::ifstream fs;
    fs.open(full_path.string().c_str(), std::ios::binary);
    if (!fs) return -3;

    fs.seekg(0, std::ios::end);
    bin_length = (CZInt)(fs.tellg());
    fs.seekg(0, std::ios::beg);

    update_bin_data = new CZUChar[bin_length];
    fs.read((CZChar*)update_bin_data, bin_length);

    fs.close();
    
    return 0;
}

CZInt CZUpdateManager::registerSite(CZSite *site)
{
    if (update_bin_data == 0)
        return -1;

    Progress_Unit unit = {update_bin_data};
    siteManager.insert(SiteNode(site, unit));
    return 0;
}

CZInt CZUpdateManager::updateProgress(CZSite *site)
{
    SiteManager::iterator iter = siteManager.find(site);
    if (iter == siteManager.end())
        return -1;

    if ((CZUInt)(iter->second.current_ptr - update_bin_data + single_packet_length) >= bin_length) //最后一个包已发完，删除该节点
        siteManager.erase(iter);
    
    if (siteManager.empty()) //若所有基站完成，则删除升级包缓存
    {
        delete []update_bin_data;
        update_bin_data = 0;
        bin_length = 0;
    }

    return 0;
}
