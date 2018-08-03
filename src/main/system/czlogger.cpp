#include <system/czlogger.h>

#include <iostream>
#include <string>
#include <memory>

#include <boost/log/support/date_time.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>

#include <boost/filesystem.hpp>

#include <czsingletonmanager.h>
#include <czconfiguration.h>

#include <utils/czmiscs.h>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

BOOST_LOG_ATTRIBUTE_KEYWORD(_severity, "Severity", MyLogLevel);

CZFrameLogger::CZFrameLogger()
{
}

CZFrameLogger::~CZFrameLogger()
{
}

bool CZFrameLogger::init()
{
	std::string workPath = CZMiscs::getApplicationPath();

	int iFileSize = CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("log_file_size");

	iFileSize = iFileSize <= 0 ? 10:iFileSize;

	//<!--1 控制台输出 2 文件输出 3 混合输出-->
	int iLevel = CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("log_show_type");

	int iShowDebug = CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("log_show_debug");

	if (!boost::filesystem::exists(workPath+"log"))
	{
		boost::filesystem::create_directories(workPath+"log");
	}

	if( iLevel == 2 || iLevel == 3  )
	{
		// init sink RunLog
		boost::shared_ptr<sinks::text_file_backend> backendRun = boost::make_shared<sinks::text_file_backend>
			(
			keywords::open_mode = std::ios::app,
			keywords::file_name = workPath + "/log/RunLog_"+LOGName+".log",
			keywords::rotation_size = iFileSize * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::min_free_space = 3 * iFileSize * 1024 * 1024
			);

		//backendRun->auto_flush(true);

		sinkRun = boost::shared_ptr<TextSink>(new TextSink(backendRun));

		sinkRun->set_formatter
			(
			expr::format("[%1% %2%]: %3%")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
			//% expr::attr<attrs::current_thread_id::value_type >("ThreadID")
			% expr::smessage
			);
		sinkRun->set_filter(expr::attr<MyLogLevel>("Severity") == Run_Log);
		//logging::core::get()->add_sink(sinkRun);


		// init sink Debug log
		boost::shared_ptr<sinks::text_file_backend> backendDebug = boost::make_shared<sinks::text_file_backend>
			(
			keywords::open_mode = std::ios::app,
			keywords::file_name = workPath + "/log/DebugLog_"+LOGName+".log",
			keywords::rotation_size = iFileSize * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::min_free_space = 3 * iFileSize * 1024 * 1024
			);
		//backendDebug->auto_flush(true);

		sinkDebug = boost::shared_ptr<TextSink>(new TextSink(backendDebug));

		sinkDebug->set_formatter
			(
			expr::format("[%1% %2%]: %3%")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
			//% expr::attr<attrs::current_thread_id::value_type >("ThreadID")
			% expr::smessage
			);
		sinkDebug->set_filter(expr::attr<MyLogLevel>("Severity") == Debug_Log || expr::attr<MyLogLevel>("Severity") == Trace_Log);

		// init sink Error log
		boost::shared_ptr<sinks::text_file_backend> backendError = boost::make_shared<sinks::text_file_backend>
			(
			keywords::open_mode = std::ios::app,
			keywords::file_name = workPath + "/log/ErrLog_"+LOGName+".log",
			keywords::rotation_size = iFileSize * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::min_free_space = 3 * iFileSize * 1024 * 1024
			);
		//backendError->auto_flush(true);

		sinkError = boost::shared_ptr<TextSink>(new TextSink(backendError));

		sinkError->set_formatter
			(
			expr::format("[%1% %2%]: %3%")
			% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
			//% expr::attr<attrs::current_thread_id::value_type >("ThreadID")
			% expr::smessage
			);
		sinkError->set_filter(expr::attr<MyLogLevel>("Severity") == Err_Log);

		//logging::core::get()->add_sink(sinkError);

		logging::core::get()->add_sink(sinkRun);
		logging::core::get()->add_sink(sinkError);

		if( iShowDebug != 0 )
		{
			logging::core::get()->add_sink(sinkDebug);
		}
	}


	//logging::core::get()->add_sink(sinkConsole);

	if( iLevel == 1 || iLevel == 3)
	{
		// init sink console log
		sinkConsole = logging::add_console_log
			(
			std::clog,
			keywords::format = expr::stream
			<<"["
			<<expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%H:%M:%S.%f")
			//<<" "
			//<<expr::attr<attrs::current_thread_id::value_type >("ThreadID")
            <<" "
            <<_severity
			<< "]："
			<< expr::message
			,keywords::auto_flush = true);

		if( iShowDebug != 0 )
		{
			sinkConsole->set_filter(expr::attr<MyLogLevel>("Severity") <= Debug_Log);
		}
		else
		{
			sinkConsole->set_filter(expr::attr<MyLogLevel>("Severity") != Debug_Log);
		}

		logging::core::get()->add_sink(sinkConsole);
	}

	logging::add_common_attributes();

	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());

	return true;
}

void CZFrameLogger::addSinks()
{
}

void CZFrameLogger::setLevel(MyLogLevel level)
{
	logging::core::get()->set_filter(expr::attr<MyLogLevel>("Severity") <= level);
}