#ifndef _GAMA_CLIENT_SEND_PACKET_H_
#define _GAMA_CLIENT_SEND_PACKET_H_

// forward decl.
class  ClientSocket;
struct CHAR_CREATE;

namespace ClientNet
{
    // forward decl.
    class CClientEventHandler;
}

namespace SendPacket
{
    bool AuthAccount(ClientSocket& clientSocket, char* UserID_In, char* UserPW_In, unsigned long ClientVer_In, unsigned long CheckSum_In, unsigned short Flag_In = 0);
    bool JapanAuthAccount(ClientSocket& clientSocket, char* UserID_In, unsigned long dwUID, unsigned long ClientVer_In, unsigned long CheckSum_In, unsigned short Flag_In = 0);

    bool UserLogin(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In, unsigned char cLoginType_In);
    bool CharSelect(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In, unsigned long CharID_In);
    bool CharCreate(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In, unsigned long SlotNum_In, CHAR_CREATE &Create_In);
    bool CharDelete(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In, unsigned long CharID_In, unsigned long SlotNum_In, char* szPassword);

    // WORK_LIST 2.1 계정 국적 추가
	bool SelectAccountNation(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In, unsigned char cType, unsigned char cAccountNation_In);

    // 통합서버 캐릭터 선택 전송
    bool UnifiedCharSelect(ClientNet::CClientEventHandler* lpHandler, const char* szPassword, 
        unsigned long* lpdwCID, unsigned long dwCIDNum, 
        unsigned char cSelectedStoreServerGroup, unsigned char cSelectedNation);
}

#endif