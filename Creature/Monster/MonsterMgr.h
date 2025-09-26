#ifndef _MONSTER_MGR_H_
#define _MONSTER_MGR_H_

#pragma once

#define g_MonsterMgr CMonsterMgr::GetInstance()

#include <Pattern/Singleton.h>
#include <Utility/DelimitedFile.h>
#include <Creature/Character/CharacterStructure.h>

#include "MonsterStructure.h"


class CMonsterMgr : public CSingleton<CMonsterMgr>
{
public:

	struct MonsterProtoType 
    {
		MonsterInfo     m_MonsterInfo;
		CreatureStatus  m_CreatureStatus;

        inline bool operator < (MonsterProtoType& rhs)
        { return m_MonsterInfo.m_dwKID < rhs.m_MonsterInfo.m_dwKID; }
	};		

    ~CMonsterMgr();

	bool LoadMonstersFromFile(const char* szFileName = 0);
	bool LoadMonstersFromBinary(const char* szFileNameBinary = 0);
	bool SaveMonstersToBinary(const char* szFileNameBinary = 0, const char* szTrashFile = 0);

	void ClearProtoType();

	const MonsterProtoType* GetMonsterProtoType(unsigned long dwKID);
	const MonsterProtoType* GetMonsterProtoType(char* szName);

	size_t GetMonsterKindNum(void) { return m_nMonsterNum; }

private:

    CMonsterMgr();

	static const char*  m_szMonsterScriptFileName;
    static CMonsterMgr  ms_this;

	MonsterProtoType*   m_ProtoTypeArray;
    size_t              m_nMonsterNum;
};


#endif