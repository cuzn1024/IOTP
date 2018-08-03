#ifndef CZ_SUITMANAGER_H
#define CZ_SUITMANAGER_H

#ifndef _STRING_
#include <string>
#endif //#ifndef _STRING_

#ifndef _THREAD_
#include <thread>
#endif //#ifndef _THREAD_

#ifndef _FUNCTIONAL_
#include <functional>
#endif //#ifndef _FUNCTIONAL_

#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif //#ifndef _UNORDERED_MAP_

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZISuite;
class CZSuitManager
{
public:
    typedef std::function<CZISuite*(void *)> PluginCreator;

private:
    typedef std::function<void(const std::string&, PluginCreator)> AddSuitCallback;
    typedef std::unordered_map<std::string, AddSuitCallback> RegisterCallbackContainer;

    typedef std::unordered_map<CZISuite*, std::function<CZISuite*()>> PluginContainer;

public:
    CZSuitManager(void);
    ~CZSuitManager(void);

public:
    CZInt lazyLoadSuit();
    CZInt registerSuit(const std::string &suitPath, AddSuitCallback addSuitFunc);

private:
    void loadAllSuits();

private:
    RegisterCallbackContainer registerCallbackContainer;
    PluginContainer pluginContainer;
    std::thread lazyLoadThread;
};

#endif //#ifndef CZ_SUITMANAGER_H
