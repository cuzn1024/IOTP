#ifndef CZ_ISUITE_H
#define CZ_ISUITE_H

#ifndef _STRING_
#include <string>
#endif //#ifndef _STRING_

class CZISuite
{
public:
    virtual ~CZISuite() {}

public:
    virtual const std::string &getSuiteName() const { return name; }

protected:
    std::string name;
};

#endif // CZ_ISUITE_H
