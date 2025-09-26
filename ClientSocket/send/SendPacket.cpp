#include "stdafx.h"
#include "../ClientSocket.h"
#include "SendPacket.h"
#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/ClientNetwork/NetworkMsgBlock.h>
#include "GMMemory.h"

bool SendPacket::SendPacket(ClientNet::CClientEventHandler* lpHandler,
                            ClientNet::CNetworkMsgBlock* lpNetworkMsgBlock, 
                            unsigned short Len_In, unsigned char Cmd_In, 
                            unsigned short State_In, unsigned short Error_In)
{
    if (0 != lpNetworkMsgBlock && 
        lpNetworkMsgBlock->WrapCrypt(Len_In, Cmd_In, State_In, Error_In) &&
        !ClientSocket::IsNSFlagChainChecked(Cmd_In))
    {
		ClientSocket::SetNSFlagOn( Cmd_In );
        lpHandler->SendPacket(lpNetworkMsgBlock);
        return true;
    }

	ClientNet::CNetworkMsgPool::GetInstance().FreeNetworkMsgBlock(lpNetworkMsgBlock);
    return false;
}

bool SendPacket::SendPacket(ClientNet::CClientEventHandler* lpHandler, 
                            ClientNet::CNetworkMsgBlock* lpNetworkMsgBlock,
                            unsigned short Len_In, unsigned char Cmd_In, unsigned long Tick_In)
{
    if (0 != lpNetworkMsgBlock && 
        lpNetworkMsgBlock->WrapCrypt(Len_In, Cmd_In, Tick_In) &&
        !ClientSocket::IsNSFlagChainChecked(Cmd_In))
    {
		ClientSocket::SetNSFlagOn( Cmd_In );
        lpHandler->SendPacket(lpNetworkMsgBlock);
        return true;
    }

	ClientNet::CNetworkMsgPool::GetInstance().FreeNetworkMsgBlock(lpNetworkMsgBlock);
    return false;
}
