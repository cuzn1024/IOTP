#ifndef CZ_MISCS_H
#define CZ_MISCS_H

#ifndef _STRING_
#include <string>
#endif //#ifndef _STRING_

#ifndef CZ_TYPES_H
#include <types.h>
#endif

class CZMiscs
{
public:
    static void initialize();

    static inline const std::string &getApplicationPath() { return applicationPath; }

    //十六进制转换
	static std::string toHexString(CZLongLong l);
	static std::string toHexString(CZShort s);
	static std::string toHexString(CZChar c);
	static std::string toHexString(CZUChar *bytes, CZInt len);

    template <typename T>
    static T hexStringToAnyNumber(const std::string &s)
    {
        return (T)hexStringToNumber(s);
    }

    static CZChar hexStringToChar(const std::string &s);
	static CZShort hexStringToShort(const std::string &s);
	static CZLongLong hexStringToLongLong(const std::string &s);

	static CZPhysicalAddr stringToPhysicalAddr(const char *);

    //数字、字符串互转
	static std::string i2s(CZInt i);
	static CZInt s2i(const std::string &s);

    //时间函数
	static CZUInt getTimestamp();
	static CZULongLong getMillisecond();

	static CZUShort randNumber(CZUShort lo = 1, CZUShort hi = 65535);

private:
    static CZLongLong hexStringToNumber(const std::string &s);

private:
    static std::string applicationPath;

public:
#pragma message("to do: " __FILE__)
	static bool isDevice(CZUChar type);
	static bool isSite(CZUChar type);
	static bool isNodeTypeRight(CZUChar type);
};

#endif // CZ_MISCS_H
