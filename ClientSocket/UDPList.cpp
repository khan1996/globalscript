/*

///////////////////////////////////////////////////////////////////////////////////////////////
//
// UDPList
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "UDPList.h"

UDPList::UDPList(void)
{
}

UDPList::~UDPList(void)
{
}


DWORD UDPList::InsertIntoAddressList(DWORD CharID_In, const SOCKADDR_IN& PubAddress_In, 
                                     const SOCKADDR_IN& PriAddress_In, PEERTYPE PeerType_In)
{
    LPUDP_LIST lpList = SearchFromAddressList(CharID_In);
    if(lpList != NULL)
    {
        lpList->CharID = CharID_In;
        lpList->PublicAddress	= PubAddress_In;
        lpList->PrivateAddress	= PriAddress_In;
        lpList->PeerType		= PeerType_In;

        return 1;
    }

    lpList = Create();
    if(lpList != NULL)
    {
        lpList->CharID			= CharID_In;
        lpList->PublicAddress	= PubAddress_In;
        lpList->PrivateAddress	= PriAddress_In;
        lpList->PeerType		= PeerType_In;

        InsertToList(lpList);

        return 2;
    }

    return 0;
}

bool UDPList::DeleteFromAddressList(DWORD CharID_In)
{
    LPUDP_LIST lpList = SearchFromAddressList(CharID_In);
    if(lpList != NULL)
    {
        DeleteFromList(lpList);

        return true;
    }

    return false;
}

bool UDPList::DeleteFromAddressList(const SOCKADDR_IN& Address_In)
{
    LPUDP_LIST lpList = SearchFromAddressList(Address_In);
    if(lpList != NULL)
    {
        DeleteFromList(lpList);

        return true;
    }

    return false;
}

LPUDP_LIST UDPList::SearchFromAddressList(DWORD CharID_In)
{
    LPUDP_LIST lpList;

    lpList = m_pListHead;
    while(lpList)
    {
        if(lpList->CharID == CharID_In)
            return lpList;

        lpList = lpList->pNext;
    }

    return NULL;
}

LPUDP_LIST UDPList::SearchFromAddressList(const SOCKADDR_IN& Address_In)
{
    LPUDP_LIST lpList;

    lpList = m_pListHead;
    while(lpList)
    {
        if(lpList->PrivateAddress.sin_addr.S_un.S_addr == Address_In.sin_addr.S_un.S_addr && lpList->PrivateAddress.sin_port == Address_In.sin_port)
            return lpList;

        if(lpList->PublicAddress.sin_addr.S_un.S_addr == Address_In.sin_addr.S_un.S_addr && lpList->PublicAddress.sin_port == Address_In.sin_port)
            return lpList;

        lpList = lpList->pNext;
    }

    return NULL;
}

*/

///////////////////////////////////////////////////////////////////////////////////////////////
//
// UDPList
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "UDPList.h"
#include "GMMemory.h"

UDPList::UDPList(void)
:   m_nMaxGarbageStackLength(0)
{
}

UDPList::~UDPList(void)
{
    DeleteAllFromList();
    ClearGarbageStack();
}

DWORD UDPList::InsertIntoAddressList(DWORD CharID_In, const SOCKADDR_IN& PubAddress_In, 
                                     const SOCKADDR_IN& PriAddress_In, PEERTYPE PeerType_In)
{	
    LPUDP_LIST lpList = SearchFromAddressList(CharID_In);

	if(NULL == lpList)
	{
		if(m_GarbageList.empty())
		{
			lpList = new UDP_LIST;
		}
		else
		{
			lpList = m_GarbageList.front();
			m_GarbageList.pop_front();
		}
	    
		if(lpList != NULL)
		{
			lpList->CharID			= CharID_In;
			lpList->PublicAddress	= PubAddress_In;
			lpList->PrivateAddress	= PriAddress_In;
			lpList->PeerType		= PeerType_In;
			lpList->MoveTick        = 0;

			m_UDP_List.push_back(lpList);
			return 1;
		}
	}
	else
	{
		lpList->PublicAddress	= PubAddress_In;
		lpList->PrivateAddress	= PriAddress_In;
		lpList->PeerType		= PeerType_In;
		lpList->MoveTick        = 0;
		return 2;
	}

    return 0;
}

DWORD UDPList::InsertIntoAddressList(UDP_LIST* lpUDP_List)
{
    LPUDP_LIST lpList = SearchFromAddressList(lpUDP_List->CharID);

	if(NULL == lpList)
	{
		if(m_GarbageList.empty())
		{
			lpList = new UDP_LIST;
		}
		else
		{
			lpList = m_GarbageList.front();
			m_GarbageList.pop_front();
		}

		if(lpList != NULL)
		{
			*lpList = *lpUDP_List;
			m_UDP_List.push_back(lpList);
			return 1;
		}
	}
	else
	{
		*lpList = *lpUDP_List;
		return 2;
	}

    return 0;
}

bool UDPList::DeleteFromAddressList(DWORD CharID_In)
{
    STDLIST::iterator pos = m_UDP_List.begin();
    STDLIST::iterator end = m_UDP_List.end();

    for(;pos != end; ++pos)
    {
        LPUDP_LIST lpList = (*pos);
        if(lpList->CharID == CharID_In)
        {
            m_UDP_List.erase(pos);

            if(m_GarbageList.size() < m_nMaxGarbageStackLength)
            {
                m_GarbageList.push_back(lpList);
            }
            else
            {
                delete lpList;
            }

            return true;
        }
    }

    return false;
}

bool UDPList::DeleteFromAddressList(const SOCKADDR_IN& Address_In)
{
    STDLIST::iterator pos = m_UDP_List.begin();
    STDLIST::iterator end = m_UDP_List.end();

    for(;pos != end; ++pos)
    {
        LPUDP_LIST lpList = (*pos);
        bool bDelete = false;

        if(lpList->PrivateAddress.sin_addr.S_un.S_addr == Address_In.sin_addr.S_un.S_addr && 
            lpList->PrivateAddress.sin_port == Address_In.sin_port)
        {
            bDelete = true;            
        }

        if(lpList->PublicAddress.sin_addr.S_un.S_addr == Address_In.sin_addr.S_un.S_addr && 
            lpList->PublicAddress.sin_port == Address_In.sin_port)
        {
            bDelete = true;
        }

        if(bDelete)
        {
            m_UDP_List.erase(pos);

            if(m_GarbageList.size() < m_nMaxGarbageStackLength)
            {
                m_GarbageList.push_back(lpList);
            }
            else
            {
                delete lpList;
            }
        }
    }

    return false;
}

LPUDP_LIST UDPList::SearchFromAddressList(DWORD CharID_In)
{
    STDLIST::iterator pos = m_UDP_List.begin();
    STDLIST::iterator end = m_UDP_List.end();

    for(;pos != end; ++pos)
    {
        LPUDP_LIST lpList = (*pos);
        if(lpList->CharID == CharID_In)
        {
            return lpList;
        }
    }

    return NULL;
}

LPUDP_LIST UDPList::SearchFromAddressList(const SOCKADDR_IN& Address_In)
{
    STDLIST::iterator pos = m_UDP_List.begin();
    STDLIST::iterator end = m_UDP_List.end();

    for(;pos != end; ++pos)
    {
        LPUDP_LIST lpList = (*pos);

        if(lpList->PrivateAddress.sin_addr.S_un.S_addr == Address_In.sin_addr.S_un.S_addr && 
            lpList->PrivateAddress.sin_port == Address_In.sin_port)
        {
            return lpList;
        }

        if(lpList->PublicAddress.sin_addr.S_un.S_addr == Address_In.sin_addr.S_un.S_addr && 
            lpList->PublicAddress.sin_port == Address_In.sin_port)
        {
            return lpList;
        }
    }

    return NULL;
}


void UDPList::DeleteAllFromList()
{
    STDLIST::iterator pos = m_UDP_List.begin();
    STDLIST::iterator end = m_UDP_List.end();

    for(;pos != end; ++pos)
    {
        LPUDP_LIST lpList = (*pos);

        if(m_GarbageList.size() < m_nMaxGarbageStackLength)
        {
            m_GarbageList.push_back(lpList);
        }
        else
        {
            delete lpList;
        }
    }

    m_UDP_List.clear();
}


void UDPList::SetGarbageStackLength(size_t StackLength_In)
{
    m_nMaxGarbageStackLength = StackLength_In;
}

void UDPList::ClearGarbageStack(void)
{
    STDLIST::iterator pos = m_GarbageList.begin();
    STDLIST::iterator end = m_GarbageList.end();

    for(;pos != end; ++pos)
    {        
        delete (*pos);
    }

    m_GarbageList.clear();
}

