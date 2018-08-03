#ifndef CZ_LOG_H
#define CZ_LOG_H

#ifndef _CZ_LOGGER_H_
#include <system/czlogger.h>
#endif

#ifndef CZ_MISCS_H
#include "utils/czmiscs.h"
#endif

#ifndef _IOMANIP_
#include <iomanip>
#endif

class CZLogHelper
{
public:
    explicit CZLogHelper(std::string &func_name);
    explicit CZLogHelper(char const * const func_name);
    ~CZLogHelper();
private:
    std::string func_name;
};


/*!
 *	整数转换成十六进制字符串
 */

#define HexStrToLongLong(s) CZMiscs::hexStringToAnyNumber<CZLongLong>(s)

#define HexStrToShort(s) CZMiscs::hexStringToAnyNumber<CZShort>(s)

#define HexStrToChar(s) CZMiscs::hexStringToAnyNumber<CZChar>(s)

#define LongLongToHexStr(s) CZMiscs::toHexString(CZLongLong(s))

#define ShortToHexStr(s) CZMiscs::toHexString(CZShort(s))

#define ByteToHexStr(s) CZMiscs::toHexString(CZShort(s))

/*!
 *	字符数组转换成十六进制字符串
 */
#define CharsToHexStr(chars,len) CZMiscs::toHexString(chars, len)

#ifndef LogHelper
#define LogHelper CZLogHelper(__FUNCTION__)
#endif //#ifndef LogHelper

#ifdef DEVICE_TYPE
#undef DEVICE_TYPE
#endif //#ifdef DEVICE_TYPE

#endif // CZ_LOG_H
