#ifndef _GAMA_CLIENT_SEND_LOGIN_OUT_H_
#define _GAMA_CLIENT_SEND_LOGIN_OUT_H_

#include <DB/DBDefine.h>

struct _GG_AUTH_DATA;
class ClientSocket;

namespace ClientNet
{
    class CClientEventHandler;
}

namespace SendPacket
{
    bool CharLogin(ClientSocket& clientSocket, unsigned long dwUID, unsigned long dwCID, unsigned long dwSessionID);
    bool CharLogout(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCloseReason);
    bool CharMoveZone(ClientNet::CClientEventHandler* lpHandler, char cZone, char cChannel, POS& NewPos_In);
    bool ServerZone(ClientNet::CClientEventHandler* lpHandler, char cZone, char cChannel);

	// AuthCodeType은 1혹은 2임
	bool CSAuthReturnCode(ClientNet::CClientEventHandler* lpHandler,
		unsigned long dwCID, unsigned long dwAuthCodeType,
		unsigned long dwReturnCode, const _GG_AUTH_DATA* lpAnswerCode);

	bool KeyInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long* pKeyInfo);		
};

#endif