#ifndef _SKILL_MGR_H_
#define _SKILL_MGR_H_

#define g_SkillMgr CSkillMgr::GetInstance()

#include <limits>

#include <Pattern/Singleton.h>
#include "SkillStructure.h"

// 전방 참조
class CDelimitedFile;


class CSkillMgr : public CSingleton<CSkillMgr>
{
public:

	enum _Const
	{ 
		MAX_SKILL_LOCKCOUNT     = 5,	    // 스킬의 최대 개수 (이 개수만큼 ProtoType이 존재한다)
		MAX_SKILL_LEVEL         = 6,		// 스킬 레벨의 한계

		MAX_SKILL_PER_STATUS    = 16,	    // 챈트와 인챈트의 가짓수
	};

	~CSkillMgr();

	void ClearProtoType(void);

	bool LoadSkillsFromFile(const char* szFileName = 0);
	bool LoadSkillsFromBinary(const char* szFileNameBinary = 0);
	bool SaveSkillsToBinary(const char* szFileNameBinary = 0, const char* szTrashFile = 0);

	const Skill::ProtoType* GetSkillProtoType(unsigned short usSkill_ID);
	void GetClassSkill(unsigned char cClass, unsigned short *arySkillID, unsigned char *cSkillNum);

    // 로드 당시의 체크섬과, 현재 데이터로 만든 체크섬을 비교한다.
    // 데이터가 바뀌면 false를 리턴한다.
    bool CheckData();

private:

	CSkillMgr();
	
    struct ProtoTypeArray 
    {
        Skill::ProtoType m_ProtoTypes[MAX_SKILL_LOCKCOUNT];
        inline bool operator < (ProtoTypeArray& rhs)
        { return m_ProtoTypes[0].m_usSkill_ID < rhs.m_ProtoTypes[0].m_usSkill_ID; }
    };

	static const char*	ms_szSkillScriptFileName;
    static CSkillMgr    ms_this;

	ProtoTypeArray*     m_ProtoTypeArray;
    size_t              m_nSkillNum;

    unsigned long       m_dwCRC32;

	// 문자열과 비교해서, 알맞은 타입 값을 리턴한다. 실패시 nMaxType을 리턴한다.
	unsigned char ReadStringToTypeValue(CDelimitedFile& DelimitedFile, const char* szColumn, 
        const CTypeName* TypeArray, const unsigned char nMaxType);

	bool CheckParentChildRule(void);
};

#endif