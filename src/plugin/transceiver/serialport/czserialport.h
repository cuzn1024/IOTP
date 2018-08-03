#ifndef CZ_SERIALPORT_H
#define CZ_SERIALPORT_H

#ifndef CZ_ITRANSCEIVERINTERFACE_H
#include <czitransceiverinterface.h>
#endif //#ifndef CZ_ITRANSCEIVERINTERFACE_H

class CZSerialPort_Private;
class CZSerialPort : public CZITransceiverInterface
{
public:
    CZSerialPort(void *param = 0);
    virtual ~CZSerialPort(void);

public:
    static CZISuite *create(void *param = 0) {
        CZSerialPort *sp = new CZSerialPort(param);
        if (sp->initialize() != 0)
        {
            delete sp;
            sp = 0;
        }
        return sp;
    }

protected:
    CZInt initialize();
	CZInt getData(CZUChar receiveBuffer[], CZUInt length);
    CZInt move(CZUInt length);
	CZInt getFrame(CZUChar receiveBuffer[], CZUInt &length);
    CZInt write(CZUChar sendBuffer[], CZUInt length);

    CZInt collectData();

private:
    CZSerialPort_Private *p;
};

#endif //#ifndef CZ_SERIALPORT_H
