#include <utils/czmiscs.h>

#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>

#include <boost/date_time.hpp>

using namespace cz_types;

std::string CZMiscs::toHexString(CZLongLong l)
{
    std::stringstream strstream;
    std::string str;
    strstream.setf(std::ios::uppercase);

    strstream<<std::hex<<std::setw(16)<<std::setfill('0')<<l;
    str = strstream.str();

	return str;
}

std::string CZMiscs::toHexString(CZShort s)
{
    std::stringstream strstream;
    std::string str;
    strstream.setf(std::ios::uppercase);

    strstream<<std::hex<<std::setw(4)<<std::setfill('0')<<s;
    str = strstream.str();

	return str;
}

std::string CZMiscs::toHexString(CZChar c)
{
    std::stringstream strstream;
    std::string str;
    strstream.setf(std::ios::uppercase);

    strstream<<std::hex<<std::setw(2)<<std::setfill('0')<<c;
    str = strstream.str();

	return str;
}

std::string CZMiscs::toHexString(CZUChar *bytes, CZInt len)
{
    std::stringstream strstream;
    std::string str;
    strstream.setf(std::ios::uppercase);

	for (CZInt i = 0; i < len; i++)
    {
        strstream<<std::hex<<std::setw(2)<<std::setfill('0')<<(CZInt)bytes[i];
	}

    str = strstream.str();

	return str;
}

CZLongLong CZMiscs::hexStringToNumber(const std::string &s)
{
    std::stringstream strstream;
    strstream.str(s.c_str());
	CZLongLong result = 0;

    strstream>>std::hex>>result;

	return result;
}

CZUInt CZMiscs::getTimestamp()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
	
    return (CZUInt)std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

CZULongLong CZMiscs::getMillisecond()
{
	boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970, 1, 1));
	boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration diff = now - time_t_epoch + boost::posix_time::hours(8);
	return diff.total_milliseconds();
}

std::string CZMiscs::i2s(CZInt i)
{
	std::stringstream stream;  
	stream<<i;
	return stream.str();
}

CZInt CZMiscs::s2i(const std::string &s)
{
	if (s.length() == 0)
	{
		return 0;
	}

	int i = 0;

	std::stringstream stream(s);
	stream>>i;

	return i;
}

CZUShort CZMiscs::randNumber(CZUShort lo, CZUShort hi)
{
	if (hi < lo)
		return 0;
    else if (hi == lo)
        return lo;

    std::mt19937 g(CZUInt(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<CZUShort> dist(lo, hi);

    return dist(g);
}

CZPhysicalAddr CZMiscs::stringToPhysicalAddr(const char *pAddr)
{
	std::stringstream ss;
	ss.setf(std::ios::hex, std::ios::basefield);
	ss.str(pAddr);

	CZULongLong addr;
	ss >> addr;

	for (unsigned i = 0; i < 4; i++)
	{
		std::swap(*((CZUChar*)&addr + i), *((CZUChar*)&addr + 7 - i));
	}

	return CZPhysicalAddr((CZUChar*)&addr);
}
bool CZMiscs::isDevice(CZUChar type)
{
	bool bRet = false;
	switch (type)
	{
		case N_ED:
		case LP_ED:
		case M_ED:
		case MLP_ED:
			bRet = true;
		default:
			break;
	}

	return bRet;
}

bool CZMiscs::isSite(CZUChar type)
{
	bool bRet = false;
	switch (type)
	{
		case M_AP:
		case R_AP:
		case C_AP:
		case B_AP:
		case L_AP:
		case T_AP:
		case Multiplex_AP:
			bRet = true;
		default:
			break;
	}

	return bRet;
}

bool CZMiscs::isNodeTypeRight(CZUChar type)
{
	bool bRet = false;
	switch (type)
	{
		case M_AP:
		case R_AP:
		case C_AP:
		case B_AP:
		case L_AP:
		case T_AP:
		case Multiplex_AP:
		case N_ED:
		case LP_ED:
		case M_ED:
		case MLP_ED:
			bRet = true;
		default:
			break;
	}

	return bRet;
}