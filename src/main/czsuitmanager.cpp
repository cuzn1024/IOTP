#include "czsuitmanager.h"

#include <algorithm>
#include <functional>

#include <boost/dll/import.hpp>
#include <boost/filesystem.hpp>

#include <suite/czisuite.h>
#include <system/czlog.h>
#include <utils/czmiscs.h>

using namespace boost;
namespace fs = filesystem;

using namespace std;

CZSuitManager::CZSuitManager(void)
{
}

CZSuitManager::~CZSuitManager(void)
{
}

CZInt CZSuitManager::lazyLoadSuit()
{
    lazyLoadThread = std::thread(std::bind(&CZSuitManager::loadAllSuits, this));
    lazyLoadThread.join();

    return 0;
}

CZInt CZSuitManager::registerSuit(const std::string &suitPath, AddSuitCallback addSuitFunc)
{
    registerCallbackContainer.insert(RegisterCallbackContainer::value_type(suitPath, addSuitFunc));

    return 0;
}

void CZSuitManager::loadAllSuits()
{
    string modulePath = CZMiscs::getApplicationPath();
    modulePath += "plugin/";
    fs::path suitPath;
    system::error_code ec;

    for_each (registerCallbackContainer.begin(), registerCallbackContainer.end(), [&suitPath, &modulePath](const RegisterCallbackContainer::value_type &node) {
        suitPath = modulePath;
        suitPath += node.first;
        for (fs::recursive_directory_iterator iter(suitPath); iter != fs::recursive_directory_iterator(); ++iter)
        {
            const fs::directory_entry &de = *iter;

            if (!is_regular_file(de.status()) || de.path().extension().string().compare(".dll") != 0) continue;

            try
            {
                PluginCreator creator = boost::dll::import<CZISuite* (void*)>(
                    de.path(),
                    "create_plugin",
                    dll::load_mode::append_decorations
                    );

                std::function<const string& ()> get_plugin_name = boost::dll::import<const string& ()>(
                    de.path(),
                    "get_plugin_name",
                    dll::load_mode::append_decorations
                    );

                node.second(get_plugin_name(), creator);
            }
            catch (...)
            {
            }
        }
    });
}
