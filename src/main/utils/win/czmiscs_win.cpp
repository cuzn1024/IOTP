#include <utils/czmiscs.h>

#include <boost/dll/runtime_symbol_info.hpp>

std::string CZMiscs::applicationPath = "";

void CZMiscs::initialize()
{
#ifdef NDEBUG
	applicationPath = "E:/work/IOT_SVN/project/delivery/Platform/branches/iot_lamp/server/gen/bin/Release/Win32/";
#else
	applicationPath = "E:/work/IOT_SVN/project/delivery/Platform/branches/iot_lamp/server/gen/bin/Debug/Win32/";
#endif
    //char *value = nullptr;
    //if (_get_pgmptr(&value) == 0)
    //{
    //    applicationPath = value;
    //    applicationPath.resize(applicationPath.rfind('\\') + 1);
    //}

	//applicationPath == boost::dll::program_location().parent_path();
}

