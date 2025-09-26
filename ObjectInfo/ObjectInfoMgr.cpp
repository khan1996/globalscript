/* TODO : REMOVE HERE
#include "stdafx.h"
#include "ObjectInfo.h"
#include "ObjectInfoMgr.h"
#include <algorithm>
#include <mmsystem.h>


using namespace BroadcastInfo;


CObjectInfo* CObjectInfoMgr::Create(const SObject& object)
{
    CObjectInfo* lpNewObjectInfo = NULL;

    switch(object.GetObjectType())
    {
		case ObjectType::OBJECT:                lpNewObjectInfo = new CObjectInfo;              break;
		case ObjectType::AGGRESIVE_CREATURE:    lpNewObjectInfo = new CAggresiveCreatureInfo;   break;
		case ObjectType::MONSTER:               lpNewObjectInfo = new CMonsterInfo;             break;
		case ObjectType::SUMMON_MONSTER:		lpNewObjectInfo = new CSummonMonsterInfo;		break;
//		case ObjectType::CHARACTER:             lpNewObjectInfo = new CCharacterInfo;           break;
    }

    return lpNewObjectInfo;
}


CObjectInfoMgr::~CObjectInfoMgr()
{
    clear();
}

void CObjectInfoMgr::clear()
{
    ObjectIterator first = m_ObjectArray.begin(), last = m_ObjectArray.end();

    for(;first != last; ++first)
    {
        delete (*first);
    }

    m_ObjectArray.clear();

}

bool CObjectInfoMgr::Update(const char* szBuffer_In, unsigned long dwBufferSize, bool bRemove)
{
    const char*     szObjectInfo    = szBuffer_In;
    const char*     szPastEnd       = szBuffer_In + dwBufferSize;
    
    unsigned long   dwCurrentTime   = timeGetTime();

    for(; szObjectInfo + sizeof(SObject) <= szPastEnd; )
    {
        const SObject*  lpSObject           = reinterpret_cast<const SObject*>(szObjectInfo);
        CObjectInfo*    lpUpdateObjectInfo  = NULL;
        CObjectInfo*    lpNewObjectInfo     = NULL;
        
        ObjectIterator finditr = std::lower_bound(m_ObjectArray.begin(), m_ObjectArray.end(), 
            lpSObject->m_dwCID, CObjectInfo::LessCID());

        if(finditr != m_ObjectArray.end() && lpSObject->m_dwCID == (*finditr)->GetCID())
        {
            // 업데이트할 것.
            lpUpdateObjectInfo = *finditr;
        }

        if(NULL != lpUpdateObjectInfo)
        {
            lpUpdateObjectInfo->Update(*lpSObject, dwCurrentTime);
        }
        else
        {
            lpNewObjectInfo = Create(*lpSObject);

            if(NULL != lpNewObjectInfo) 
            {
                lpNewObjectInfo->Update(*lpSObject, dwCurrentTime);
                m_ObjectArray.insert(finditr, lpNewObjectInfo);
            }
        }

        unsigned long dwObjectSize = GetObjectSize(*lpSObject);
        if(0 == dwObjectSize)
        {
            // 잘못된 데이터가 들어왔다.
            break;
        }

        szObjectInfo += dwObjectSize;
    }

    if(bRemove)
    {
        Eliminate(dwCurrentTime);
    }

    return true;
}




void CObjectInfoMgr::Process(IProcessBroadcastInfo& processBroadcastInfo)
{
    ObjectIterator first = m_ObjectArray.begin(), last = m_ObjectArray.end();
    for(;first != last; ++first) 
    {
        (*first)->Process(processBroadcastInfo);
    }
}



CObjectInfo* CObjectInfoMgr::GetObjectInfo(unsigned long dwCID)
{
    ObjectIterator finditr = std::lower_bound(m_ObjectArray.begin(), m_ObjectArray.end(), 
        dwCID, CObjectInfo::LessCID());

    if(finditr != m_ObjectArray.end() && dwCID == (*finditr)->GetCID())
    {        
        return *finditr;
    }

    return NULL;
}


CObjectInfo* CObjectInfoMgr::CreateOrUpdate(const SObject& object, unsigned long dwCurrentTime)
{
    CObjectInfo* lpObjectInfo = NULL;

    ObjectIterator finditr = std::lower_bound(m_ObjectArray.begin(), m_ObjectArray.end(), 
        object.m_dwCID, CObjectInfo::LessCID());

    if(finditr != m_ObjectArray.end() && object.m_dwCID == (*finditr)->GetCID())
    {
        lpObjectInfo = *finditr;
        lpObjectInfo->Update(object, dwCurrentTime);        
    }
    else
    {
        lpObjectInfo = Create(object);
        if(NULL != lpObjectInfo)
        {
            lpObjectInfo->Update(object, dwCurrentTime);
            m_ObjectArray.insert(finditr, lpObjectInfo);
        }
    }

    return lpObjectInfo;
}



void CObjectInfoMgr::Remove(unsigned long dwCID)
{
    ObjectIterator finditr = std::lower_bound(m_ObjectArray.begin(), m_ObjectArray.end(), 
        dwCID, CObjectInfo::LessCID());

    if(finditr != m_ObjectArray.end() && dwCID == (*finditr)->GetCID())
    {
        CObjectInfo* lpObjectInfo = *finditr;
        delete lpObjectInfo;

        m_ObjectArray.erase(finditr);
    }
}


void CObjectInfoMgr::Eliminate(unsigned long dwExceptTime)
{
    // 이번에 갱신된 데이터가 아니면 전부 제거한다.
    ObjectArray objectArray;
    objectArray.reserve(m_ObjectArray.size());

    ObjectIterator first    = m_ObjectArray.begin();
    ObjectIterator last     = m_ObjectArray.end();

    for(;first != last; ++first)
    {
        CObjectInfo* lpObjectInfo = *first;

        if(dwExceptTime == lpObjectInfo->GetLastUpdateTick())
        {
            objectArray.push_back(lpObjectInfo);
            *first = NULL;
        }
    }

    m_ObjectArray.swap(objectArray);

    for(first = objectArray.begin(), last = objectArray.end(); first != last; ++first)
    {
        CObjectInfo* lpObjectInfo = *first;
        if(NULL != lpObjectInfo)
        {
            delete lpObjectInfo;
        }
    }
}

*/