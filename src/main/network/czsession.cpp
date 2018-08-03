#include <network/czsession.h>

#include <system/czlog.h>
#include <suite/transceiver/czitransceiverinterface.h>

using namespace cz_types;
using namespace cz_defines;

CZSession::CZSession(CZITransceiverInterface *transceiverInterface,CZUInt session_id) :transceiverInterface(transceiverInterface),
	id(session_id), status(E_Session_Status_Unknown)
{
	lastRecvDataTime = CZMiscs::getTimestamp();
}

CZSession::~CZSession()
{
}

CZInt CZSession::collectData()
{
    return transceiverInterface->collectData();
}

CZInt CZSession::getData(CZUChar receiveBuffer[], CZUInt length)
{
	return transceiverInterface->getData(receiveBuffer, length);
}

CZInt CZSession::move(CZUInt length)
{
    return transceiverInterface->move(length);
}

CZInt CZSession::sendFrame(CZUChar dataBuffer[], CZUInt length)
{
	return transceiverInterface->write(dataBuffer,length);
}

CZUInt CZSession::close()
{
	return transceiverInterface->close();
}