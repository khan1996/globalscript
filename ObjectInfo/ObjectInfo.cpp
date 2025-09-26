/* TODO : REMOVE HERE
#include "stdafx.h"
#include "ObjectInfo.h"

using namespace BroadcastInfo;


CObjectInfo::CObjectInfo()
:   m_dwLastUpdateTick(0) 
{
}

void CObjectInfo::Update(const SObject& object, unsigned long dwLastUpdateTick) 
{
    m_Object = object;
    m_dwLastUpdateTick = dwLastUpdateTick;
}


CAggresiveCreatureInfo::CAggresiveCreatureInfo()
:   m_nNowHP(0)
{ 
}

void CAggresiveCreatureInfo::Update(const SObject& object, unsigned long dwLastUpdateTick)
{
    CObjectInfo::Update(object, dwLastUpdateTick);

    if(IsAggresiveCreatureInfo(object.GetDataType()))
    {
		std::copy(static_cast<const SAggresiveCreature&>(object).m_EnchantInfo.m_dwStatusFlag, 
			static_cast<const SAggresiveCreature&>(object).m_EnchantInfo.m_dwStatusFlag + EnchantInfo::MAX_ARRAY, 
			m_EnchantInfo.m_dwStatusFlag);
		m_nNowHP = static_cast<const SAggresiveCreature&>(object).m_nNowHP;
    }
}

CAggresiveCreatureInfo* CAggresiveCreatureInfo::DowncastToAggresiveCreatureInfo(CObjectInfo* lpObjectInfo)
{
    return (NULL != lpObjectInfo && IsAggresiveCreatureInfo(lpObjectInfo->GetObjectType()))
        ? static_cast<CAggresiveCreatureInfo*>(lpObjectInfo) : NULL;
}


CMonsterInfo::CMonsterInfo()
{
}

void CMonsterInfo::Update(const SObject& object, unsigned long dwLastUpdateTick)
{
	CAggresiveCreatureInfo::Update(object, dwLastUpdateTick);
}

CMonsterInfo* CMonsterInfo::DowncastToMonsterInfo(CObjectInfo* lpObjectInfo)
{
    return (NULL != lpObjectInfo && IsMonsterInfo(lpObjectInfo->GetObjectType()))
        ? static_cast<CMonsterInfo*>(lpObjectInfo) : NULL;
}


CSummonMonsterInfo::CSummonMonsterInfo()
:	m_dwMasterID(0)
{
}

void CSummonMonsterInfo::Update(const SObject& object, unsigned long dwLastUpdateTick)
{
	CMonsterInfo::Update(object, dwLastUpdateTick);

	if(IsSummonMonsterInfo(object.GetDataType()))
	{
		m_dwMasterID = static_cast<const Monster::SSummonMonster&>(object).m_dwMasterID;
	}
}

CSummonMonsterInfo* CSummonMonsterInfo::DowncastToSummonMonsterInfo(CObjectInfo* lpObjectInfo)
{
	return (NULL != lpObjectInfo && IsSummonMonsterInfo(lpObjectInfo->GetObjectType()))
		? static_cast<CSummonMonsterInfo*>(lpObjectInfo) : NULL;
}

*/
