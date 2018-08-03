/*!
 * \file   czlogger.h
 * \author guxingxin
 * \date   2017/05/22
 * \brief  日志打印管理类
 * 日志规则：配置文件中加载基础（大小、级别）设置
 * 按照deubg、release环境打印到文件、或者控制台中
 * 正式下文件是按天打印、命名为日期，文件大小为配置值
 */
#ifndef _CZ_LOGGER_H_
#define _CZ_LOGGER_H_

#ifndef BOOST_LOG_COMMON_HPP_INCLUDED_
#include <boost/log/common.hpp>
#endif

#ifndef BOOST_LOG_SOURCES_LOGGER_HPP_INCLUDED_
#include <boost/log/sources/logger.hpp>
#endif 

#ifndef BOOST_LOG_SINKS_HPP_INCLUDED_
#include <boost/log/sinks.hpp>
#endif

#ifndef BOOST_LOG_SINKS_SYNC_FRONTEND_HPP_INCLUDED_
#include <boost/log/sinks/sync_frontend.hpp>
#endif

#ifndef BOOST_LOG_SINKS_TEXT_OSTREAM_BACKEND_HPP_INCLUDED_
#include <boost/log/sinks/text_ostream_backend.hpp>
#endif

namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;

#ifdef _DEBUG
/*!
 *  开发环境
 */
#define LOGName std::string("Debug")

#else
/*!
 *  线上环境
 */
#define LOGName std::string("%Y-%m-%d_%N")

#endif

/**
 *  日志类型枚举
 */
enum MyLogLevel
{
    Err_Log,    /*!<错误日志*/
    Trace_Log,  /*!<跟踪日志*/
	Run_Log,    /*!<运行日志*/
	Debug_Log   /*!<调试日志*/
};

/*!
 *  重载<<操作符 用于枚举对应文字
 */
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
    std::basic_ostream< CharT, TraitsT >& strm, MyLogLevel lvl)
{
    static const char* const str[] =
    {
        "Err_Log",
        "Trace_Log",
		"Run_Log",
		"Debug_Log"
    };
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
        strm << str[lvl];
    else
        strm << static_cast< int >(lvl);
    return strm;
}

/*!
 *	运行日志
 */
#define RunLog BOOST_LOG_SEV(my_logger::get(), Run_Log)

/*!
 *	调试日志
 */
//#define DebugLog BOOST_LOG_SEV(my_logger::get(),Debug_Log)<<__FILE__<<" "<<__FUNCTION__<<" "<<__LINE__<<" "

//#define DebugLog if(_DEBUG) BOOST_LOG_SEV(my_logger::get(),Debug_Log)<<__FILE__<<" "<<__FUNCTION__<<" "<<__LINE__<<" "

//#define DebugLog if(_DEBUG) BOOST_LOG_SEV(my_logger::get(),Debug_Log)<<" Function:"<<__FUNCTION__<<" Line:"<<__LINE__<<" Msg:"

#ifdef _DEBUG_
#define DebugLog BOOST_LOG_SEV(my_logger::get(), Debug_Log)
#else
#define DebugLog std::ostringstream()
#endif

/*!
 * 错误日志
 */
#define ErrLog BOOST_LOG_SEV(my_logger::get(),Err_Log)

/*!
 * 跟踪日志
 */
#define TraceLog BOOST_LOG_SEV(my_logger::get(), Trace_Log)


/*!
 * 全局唯一日志打印类my_logger
 */
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt<MyLogLevel>);

/*!
 * \brief 日志打印、管理类
 */
class CZFrameLogger
{
public:
	
	/*!
	 * \brief     打印类构造函数
	 */
	CZFrameLogger();
	/*!
	 * \brief     打印类析构函数
	 */
	~CZFrameLogger();

	/*!
	 * \brief     日志类初始化
	 *
	 * 主要完成run sink 、debug sink、error sink、console sink初始化
	 * 根据文件配置对日志文件配置及日志打印级别设置
	 */
	bool init();

	/*!
	 * \brief     预留加载日志打印sink
	 *
	 *  加载不通sink，可以设置打印什么日志，打印到什么地方
	 */
	void addSinks();

	/*!
	 * \brief     预留设置日志打印级别
	 * 
	 * \param     MyLogLevel level 日志级别
	 * \enum      MyLogLevel
	 */
	void setLevel(MyLogLevel level);

public:
	/*!
	 *	文件打印sink
	 */
	typedef sinks::synchronous_sink<sinks::text_file_backend> TextSink;

	/*!
	 *	控制台打印sink
	 */
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> ConsoleSink;

	/*!
	 * 发布运行日志打印	
	 */
	boost::shared_ptr<TextSink> sinkRun;

	/*!
	 *	调试日志打印
	 */
	boost::shared_ptr<TextSink> sinkDebug;
	
	/*!
	 *	错误日志打印
	 */
	boost::shared_ptr<TextSink> sinkError;
	
	/*!
	 *	全部日志控制台打印
	 */
	boost::shared_ptr<ConsoleSink> sinkConsole;
};

#endif