#ifndef CZ_SINGLETONMANAGER_H
#define CZ_SINGLETONMANAGER_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef BOOST_SERIALIZATION_SINGLETON_HPP
#include <boost/serialization/singleton.hpp>
#endif //#ifndef BOOST_SERIALIZATION_SINGLETON_HPP

#define GET_SINGLETON(name) \
    public: \
        inline CZ##name &get##name() { return *instance##name; } \
    private: \
        CZ##name *instance##name;

class CZMessagePool;
class CZSuitManager;
class CZThreadManager;
class CZConfiguration;
class CZConnectionSuite;
//thrift管理器
#ifdef THRIFT_ENABLED
class CZThriftManager;
#endif //#ifdef THRIFT_ENABLED
//日志类
class CZFrameLogger;
class CZNodeManager;
class CZSessionManager;
class CZDataHelper;
class CZSingletonManager : public boost::serialization::singleton < CZSingletonManager >
{
public:
    CZSingletonManager();
    ~CZSingletonManager();
public:
    CZInt initialize();

    GET_SINGLETON(ConnectionSuite);
    GET_SINGLETON(ThreadManager);
    GET_SINGLETON(Configuration);
    GET_SINGLETON(SuitManager);
    GET_SINGLETON(MessagePool);
#ifdef THRIFT_ENABLED
	GET_SINGLETON(ThriftManager);
#endif //#ifdef THRIFT_ENABLED
	GET_SINGLETON(FrameLogger);
	GET_SINGLETON(NodeManager);
	GET_SINGLETON(SessionManager);
	GET_SINGLETON(DataHelper);
};

#undef GET_SINGLETON //#define GET_SINGLETON

#ifndef CZSingletonManagerInstance
#define CZSingletonManagerInstance CZSingletonManager::get_mutable_instance()
#endif

#endif // CZ_SINGLETONMANAGER_H
