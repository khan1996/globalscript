///////////////////////////////////////////////////////////////////////////////////////////////
//
// ClientSocket Class
//
// Last Update : 2003-01-10
//
///////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GAMA_CLIENT_SOCKET_H_
#define _GAMA_CLIENT_SOCKET_H_

#include <winsock2.h>
#include <windows.h>

#include <Network/Packet/PacketCommand.h>
#include <DB/DBDefine.h>
#include <vector>
#include <bitset>

namespace ClientNet
{
    // forward decl.
    class CNetworkMsgBlock;
    class CClientEventHandler;
    class CClientEventHandlerMgr;
};


class ClientSocket
{
protected:

    ClientSocket();
    virtual ~ClientSocket();
	static ClientSocket* s_pInstance;

public:

	static ClientSocket* CreateInstance();
	
    static void DestroyInstance() { if(s_pInstance) { delete s_pInstance; s_pInstance = 0; } };
	static ClientSocket* Instance() { return s_pInstance; };

    enum Const
    {
        MAX_CHAT_SERVER_ADDR				= 16,
		MAX_GAME_GUARD_CRYPT_HEADER_SIZE	= 32
    };

    enum ConnectPort
    {
        DEFAULT_AUTH_TCP_PORT   = 10210,
        DEFAULT_CLIENT_UDP_PORT = 20002    
    };

    enum HandlerType
    {
        AuthEventHandler    = 0,
        GameEventHandler    = 1,                
        MAX_HANDLER         = 2
    };

    enum AddressType
    {
        PublicAddr      = 0,
        PrivateAddr     = 1,
        MoveZoneAddr    = 2,
        MAX_ADDRESS     = 3
    };

    // ------------------------------------------------------------------------------------------------
    // 네트워크 상태 플래그
	// by Hades Kang
    void SetStatusFlag(unsigned long dwStatusFlag);
	unsigned long GetStatusFlag();

	static bool IsNSFlagChecked(unsigned char ucCmdFlag);			//해당 플래그가 체크되었는지 여부
	static bool IsNSFlagChainChecked(unsigned char ucCmdFlag);		//해당 플래그 체인이 체크되었는지 여부
	static void SetNSFlagOn(unsigned char ucCmdFlag);				//해당 플래그를 활성화 시킴 - Send 시에 쓰임
	static void SetNSFlagOff(unsigned char ucCmdFlag);				//해당 플래그와 연관 된 것들을 비활성화 시킴 - Receive 시에 쓰임
	static int	CheckFlag( int* arrFlag );          				//디버깅용

    // ------------------------------------------------------------------------------------------------
    // Management Address
    SOCKADDR_IN& GetAddress(AddressType eAddrType) { return m_Address[eAddrType]; }
    const char* GetAddressString(AddressType eAddrType) { return inet_ntoa(m_Address[eAddrType].sin_addr); }
    unsigned short GetAddressPort(AddressType eAddrType) { return ntohs(m_Address[eAddrType].sin_port);    } 
    
    void SetAddress(AddressType eAddrType, SOCKADDR_IN& Address) { m_Address[eAddrType] = Address; }
    void SetAddress(AddressType eAddrType, const char* szAddress, unsigned port);
    void ClearAddress(AddressType eAddrType) { memset(m_Address + eAddrType, 0, sizeof(SOCKADDR_IN)); }

    ClientNet::CClientEventHandler& GetHandler(HandlerType eHandlerType) 
    { return (0 == m_lpEventHandler[eHandlerType]) ? *m_lpNullEventHandler : *m_lpEventHandler[eHandlerType]; }

    void SetHandler(ClientNet::CClientEventHandler* lpHandler, HandlerType eHandlerType);
    void ResetHandler(ClientNet::CClientEventHandler* lpHandler, HandlerType eHandlerType);

    // 세션을 생성하고, 서버로 연결하거나 지정한 주소에 바인딩을 한다.
    bool Open(const char* address, unsigned short usport, 
        ClientNet::CClientEventHandler* lpEventHandler);

    // 연결되어있는지 여부를 살핀다.
    bool IsConnected(HandlerType eHandlerType);

    // MAX_SESSION이 들어오면 전부 접속을 끊는다.
    void Disconnect(HandlerType eHandlerType);

    // 연결 및 패킷을 처리한다.
    void HandleEvents(DWORD dwProcessPerCall);

private:

	void InitNSCheckList();         // Network Status 관련 Check List 초기화 ( Send, Receive )
	void InitSendCheckList();       
	void InitReceiveCheckList();
    
    SOCKADDR_IN                         m_Address[MAX_ADDRESS + 1];
    ClientNet::CClientEventHandler*     m_lpEventHandler[MAX_HANDLER + 1];
    ClientNet::CClientEventHandler*     m_lpNullEventHandler;

    ClientNet::CClientEventHandlerMgr*  m_lpEventHandlerMgr;
        
    unsigned long                       m_dwStatusFlag;
	std::vector<unsigned char>	        m_SendCheckList[CmdFinalPacketNum];		//보낼때 체크 해야하는 커맨드 리스트 ( 하나라도 true 일 경우 보내지 않는다! )
	std::vector<unsigned char>	        m_ReceiveCheckList[CmdFinalPacketNum];  //받을때 체크 해야하는 커맨드 리스트 ( 모두 false로 처리한다. )
    std::bitset<CmdFinalPacketNum>      m_bStatusFlag;
};


#endif