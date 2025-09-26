
#include "stdafx.h"

#include <Utility/Math/Math.h>
#include <Utility/Compress/MiniLZO/MiniLZOWrapper.h>
#include <Utility/Resource/EnsureCleanup.h>

#include <Utility/CheckSum/Crc32Static.h>

#include <Utility/DelimitedFile.h>
#include <Network/XORCrypt/XORCrypt.h>
#include <Creature/Character/CharacterClass.h>

// 클라이언트에서도 쓰이므로 include를 명시한다.
#include <Log/ServerLog.h>

#include <algorithm>

#include "SkillMgr.h"
#include "GMMemory.h"

#define DECODE_HEADER(Start_In, Length_In, PageVer_In, PageNum_In)  CXORCrypt::GetInstance().DecodeHeader((Start_In),(Length_In),(PageVer_In),(PageNum_In))
#define ENCODE_HEADER(Start_In, Length_In, PageVer_In, PageNum_In)  CXORCrypt::GetInstance().EncodeHeader((Start_In),(Length_In),(PageVer_In),(PageNum_In))
#define COMPRESS(In, In_len, Out, Out_len)      CMiniLZO::Compress((In), (In_len), (Out), (Out_len))
#define DECOMPRESS(In, In_len, Out, Out_len)    CMiniLZO::Decompress((In), (In_len), (Out), (Out_len))

CSkillMgr CSkillMgr::ms_this;

const char* CSkillMgr::ms_szSkillScriptFileName = "./Script/Game/SkillScript.txt";


CSkillMgr::CSkillMgr()
:   m_ProtoTypeArray(NULL), m_nSkillNum(0), m_dwCRC32(0)
{

}

CSkillMgr::~CSkillMgr()
{
	ClearProtoType();
}

void CSkillMgr::ClearProtoType(void)
{
    if (NULL != m_ProtoTypeArray)
    {
        delete [] m_ProtoTypeArray;
        m_ProtoTypeArray = NULL;
    }
}


bool CSkillMgr::LoadSkillsFromFile(const char* szFileName)
{
	using namespace Skill;

	// 임시변수들
	const int	        SKILL_ID_LEN	= 32;
	const int			HEADER_LINE_NUM	= 1;

	char		        szBuffer[SKILL_ID_LEN];
    unsigned char       chTempValue;

    int                 nLineCount  = 0;
    int                 nSkillCount = 0;
	CDelimitedFile      DelimitedFile;      // 객체 소멸시, 자동 Close.

    ProtoTypeArray      tempProtoType;

	// 매크로에 로그 코드 삽입을 잊지 말 것.
	// 매크로에서 \뒤에 공백이나 문자 삽입되지 않도록 주의할 것.
	// ( '이스케이프 시퀀스가 잘못되었습니다' 에러 발생 )
	#define READ_DATA(ColumnName, Argument) \
		if (false == DelimitedFile.ReadData(ColumnName, Argument)) \
        { \
            ERRLOG2(g_Log, "스킬 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount + HEADER_LINE_NUM, #ColumnName); \
			return false; \
		}
	
	#define READ_STRING(ColumnName, Buffer, BufferSize) \
		if (false == DelimitedFile.ReadString(ColumnName, Buffer, BufferSize)) \
        {\
            ERRLOG2(g_Log, "스킬 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount + HEADER_LINE_NUM, #ColumnName);\
            return false;\
		}

	#define READ_STRING_TO_TYPE_VALUE(saveType, saveValue, szColumn, TypeArray, nMaxType)\
        saveValue = static_cast<saveType>(ReadStringToTypeValue(DelimitedFile, szColumn, TypeArray, nMaxType));\
		if (nMaxType == saveValue)\
        {\
            ERRLOG2(g_Log, "스킬 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount + HEADER_LINE_NUM, #szColumn); \
            return false;\
		}

	if (false == DelimitedFile.Open(szFileName ? szFileName : ms_szSkillScriptFileName, HEADER_LINE_NUM)) 
    {
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szFileName ? szFileName : ms_szSkillScriptFileName);
		return false;
	}
	
    std::vector<ProtoTypeArray>     ProtoTypeVector;
    

	while (DelimitedFile.ReadLine()) 
    {
        ++nLineCount;

        READ_STRING("ID", szBuffer, SKILL_ID_LEN);
        tempProtoType.m_ProtoTypes[nSkillCount].m_usSkill_ID = Math::Convert::Atos(szBuffer);

        READ_STRING_TO_TYPE_VALUE(Skill::Type::SkillType, tempProtoType.m_ProtoTypes[nSkillCount].m_eSkillType, 
            "Type", Type::SkillTypes, Type::MAX_SKILL_TYPE);

		READ_DATA("ClassSkill", chTempValue); tempProtoType.m_ProtoTypes[nSkillCount].m_bIsClassSkill = chTempValue != 0;
		READ_STRING("Name[5]", tempProtoType.m_ProtoTypes[nSkillCount].m_SpriteInfo.m_szName, ProtoType::MAX_SKILL_NAME);
        READ_STRING_TO_TYPE_VALUE(Skill::UseLimit::Type, tempProtoType.m_ProtoTypes[nSkillCount].m_eUseLimit, 
            "Limit", UseLimit::UseLimits, UseLimit::MAX_USE_LIMIT);
		
		READ_STRING_TO_TYPE_VALUE(unsigned char, tempProtoType.m_ProtoTypes[nSkillCount].m_StatusLimitType[0], 
            "Stat[0]", StatusLimit::StatusLimits, StatusLimit::MAX_STATUS_LIMIT);
		READ_DATA("Value[0]", tempProtoType.m_ProtoTypes[nSkillCount].m_StatusLimitValue[0]);
		
		READ_STRING_TO_TYPE_VALUE(unsigned char, tempProtoType.m_ProtoTypes[nSkillCount].m_StatusLimitType[1], 
            "Stat[1]", StatusLimit::StatusLimits, StatusLimit::MAX_STATUS_LIMIT);
		READ_DATA("Value[1]", tempProtoType.m_ProtoTypes[nSkillCount].m_StatusLimitValue[1]);

        READ_STRING_TO_TYPE_VALUE(Skill::Target::Type, tempProtoType.m_ProtoTypes[nSkillCount].m_eTargetType, 
            "Target", Target::TargetTypes, Target::MAX_TARGET_TYPE);

		READ_DATA("mRange", tempProtoType.m_ProtoTypes[nSkillCount].m_fMinRange);
		READ_DATA("Range", tempProtoType.m_ProtoTypes[nSkillCount].m_fMaxRange);
		READ_DATA("Region", tempProtoType.m_ProtoTypes[nSkillCount].m_fEffectExtent);
		READ_DATA("StartMP", tempProtoType.m_ProtoTypes[nSkillCount].m_StartMP);
		READ_DATA("LevelMP", tempProtoType.m_ProtoTypes[nSkillCount].m_LevelMP);
		READ_DATA("LockAdd", tempProtoType.m_ProtoTypes[nSkillCount].m_LockMP);
		READ_DATA("Starttick", tempProtoType.m_ProtoTypes[nSkillCount].m_StartTick);
		READ_DATA("LevelTick", tempProtoType.m_ProtoTypes[nSkillCount].m_LevelTick);
		READ_STRING("CastingFlag", tempProtoType.m_ProtoTypes[nSkillCount].m_szCastingFileName, ProtoType::MAX_FILE_NAME);
		READ_STRING("EffectFlag", tempProtoType.m_ProtoTypes[nSkillCount].m_szEffectFileName, ProtoType::MAX_FILE_NAME);
		READ_STRING("HitFlag", tempProtoType.m_ProtoTypes[nSkillCount].m_szHitFileName, ProtoType::MAX_FILE_NAME);
		READ_DATA("CoolDownTime", tempProtoType.m_ProtoTypes[nSkillCount].m_dwCoolDownTime);
		READ_DATA("EndCoolDown", tempProtoType.m_ProtoTypes[nSkillCount].m_cEndCoolDown);
		READ_STRING("dds", tempProtoType.m_ProtoTypes[nSkillCount].m_SpriteInfo.m_szSpriteName, ProtoType::MAX_FILE_NAME);
		READ_DATA("MinX", tempProtoType.m_ProtoTypes[nSkillCount].m_SpriteInfo.m_nSpriteMinX);
		READ_DATA("MinY", tempProtoType.m_ProtoTypes[nSkillCount].m_SpriteInfo.m_nSpriteMinY);
		READ_DATA("MaxX", tempProtoType.m_ProtoTypes[nSkillCount].m_SpriteInfo.m_nSpriteMaxX);
		READ_DATA("MaxY", tempProtoType.m_ProtoTypes[nSkillCount].m_SpriteInfo.m_nSpriteMaxY);
		READ_DATA("Hit", tempProtoType.m_ProtoTypes[nSkillCount].m_cStrikeNum);
		READ_DATA("EndScript", tempProtoType.m_ProtoTypes[nSkillCount].m_cEndScript);
		READ_DATA("Protection", chTempValue); tempProtoType.m_ProtoTypes[nSkillCount].m_bProtection = chTempValue != 0;
        READ_DATA("Interrupt", chTempValue); tempProtoType.m_ProtoTypes[nSkillCount].m_bInterrupt = chTempValue != 0;
        READ_DATA("Counter", chTempValue); tempProtoType.m_ProtoTypes[nSkillCount].m_bCounter = chTempValue != 0;
		READ_DATA("Gauge", chTempValue); tempProtoType.m_ProtoTypes[nSkillCount].m_bGauge = chTempValue != 0;

		READ_STRING("Parent", szBuffer, SKILL_ID_LEN);
        tempProtoType.m_ProtoTypes[nSkillCount].m_usParentSkill = Math::Convert::Atos(szBuffer);
		READ_STRING("Child", szBuffer, SKILL_ID_LEN);
		tempProtoType.m_ProtoTypes[nSkillCount].m_usChildSkill = Math::Convert::Atos(szBuffer);

		READ_STRING("Text", tempProtoType.m_ProtoTypes[nSkillCount].m_szSkillDescribe, ProtoType::MAX_SKILL_DESCRIBE);

		if (tempProtoType.m_ProtoTypes[0].m_usSkill_ID != tempProtoType.m_ProtoTypes[nSkillCount].m_usSkill_ID)
        {
            ERRLOG2(g_Log, "스킬 숫자가 맞지 않습니다. 클래스 스킬을 제외하고는, 각 스킬은 반드시 같은 ID가"
                " 5개가 연속해서 나와야 합니다. 현재 스킬 번호는 0x%04x, 개수는 %d입니다.", 
                tempProtoType.m_ProtoTypes[nSkillCount].m_usSkill_ID, nSkillCount);
			return false;
		}

		++nSkillCount;
        
		if (MAX_SKILL_LOCKCOUNT == nSkillCount || tempProtoType.m_ProtoTypes[0].m_bIsClassSkill) 
		{
            ProtoTypeVector.push_back(tempProtoType);
			
            for (int nIndex = 0; nIndex < nSkillCount; ++nIndex)
            {
                tempProtoType.m_ProtoTypes[nIndex].Initialize();
            }

            nSkillCount = 0;
		}
	}

    std::sort(ProtoTypeVector.begin(), ProtoTypeVector.end());

    for (std::vector<ProtoTypeArray>::iterator itr = ProtoTypeVector.begin();
        itr != ProtoTypeVector.end() - 1; ++itr)
    {
        if (itr->m_ProtoTypes[0].m_usSkill_ID == (itr+1)->m_ProtoTypes[0].m_usSkill_ID)
        {
            ERRLOG1(g_Log, "겹치는 스킬 종류 ID가 있습니다. 종류ID:0x%04x", itr->m_ProtoTypes[0].m_usSkill_ID);
            return false;
        }
    }

    m_nSkillNum = ProtoTypeVector.size();
    m_ProtoTypeArray = new ProtoTypeArray[m_nSkillNum];
    if (NULL == m_ProtoTypeArray)
    {
        ERRLOG0(g_Log, "메모리 할당에 실패했습니다.");
        return false;
    }

    std::copy(ProtoTypeVector.begin(), ProtoTypeVector.end(), m_ProtoTypeArray);

	if (false == CheckParentChildRule())
	{
		ERRLOG0(g_Log, "스킬 스크립트의 잘못된 부모/자식 관계를 수정해주세요.");
		return false;
	}

    // 스킬 체크섬을 빌드하겠소.
    if(NO_ERROR != CCrc32Static::BufferCrc32(reinterpret_cast<const char*>(m_ProtoTypeArray),
        sizeof(ProtoTypeArray) * m_nSkillNum, m_dwCRC32))
    {
        ERRLOG0(g_Log, "스킬 스크립트 체크섬 빌드에 실패했습니다.");
        return false;
    }

	return true;
}

bool CSkillMgr::LoadSkillsFromBinary(const char* szFileNameBinary)
{
	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? ms_szSkillScriptFileName : szFileNameBinary,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; } 

	CEnsureCloseHandle hfile(hFile);

	unsigned long dwRead            = 0;
	unsigned long dwFileHighSize    = 0;
	unsigned long dwFileSize        = GetFileSize(hFile, &dwFileHighSize);

	char* lpAllocated = new char[dwFileSize];
	CEnsureDeleteArray<char> allocated(lpAllocated);

	if (NULL == lpAllocated)
	{
		ERRLOG0(g_Log, "메모리가 부족합니다.");
		return false;
	}

	if (false == ReadFile(hFile, lpAllocated, dwFileSize, &dwRead, NULL)) 
	{
		ERRLOG1(g_Log, "읽기 실패 : %d에러", GetLastError());
		return false;
	}

	unsigned long dwHeaderSize          = sizeof(unsigned long) + *reinterpret_cast<unsigned long*>(lpAllocated) + sizeof(unsigned long);
	unsigned long dwDecompressedSize    = *reinterpret_cast<unsigned long*>(lpAllocated + dwHeaderSize - sizeof(unsigned long));

	ClearProtoType();

	m_nSkillNum = dwDecompressedSize/sizeof(ProtoTypeArray);
	m_ProtoTypeArray = new ProtoTypeArray[m_nSkillNum];
	if (NULL == m_ProtoTypeArray)
	{
		ERRLOG0(g_Log, "스킬 스크립트 내용을 불러올 수 없습니다. 메모리가 부족합니다.");
		return false;
	}

	DECOMPRESS(lpAllocated + dwHeaderSize, dwFileSize - dwHeaderSize, 
		reinterpret_cast<char*>(m_ProtoTypeArray), &dwDecompressedSize);

	DECODE_HEADER(reinterpret_cast<char*>(m_ProtoTypeArray), dwDecompressedSize, 3, 3);	

    // 스킬 체크섬을 빌드하겠소.
    if(NO_ERROR != CCrc32Static::BufferCrc32(reinterpret_cast<const char*>(m_ProtoTypeArray),
        sizeof(ProtoTypeArray) * m_nSkillNum, m_dwCRC32))
    {
        ERRLOG0(g_Log, "스킬 스크립트 체크섬 빌드에 실패했습니다.");
        return false;
    }

    return true;
}

bool CSkillMgr::SaveSkillsToBinary(const char* szFileNameBinary, const char* szTrashFile)
{
	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? ms_szSkillScriptFileName : szFileNameBinary,
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	CEnsureCloseHandle file(hFile);

	unsigned long   dwSkillSize     = static_cast<unsigned long>(sizeof(ProtoTypeArray) * m_nSkillNum);
	unsigned long   dwCompressSize  = dwSkillSize;

	char*   lpSkill_Info        = new char[dwSkillSize];
	char*   lpCompressedInfo    = new char[dwSkillSize];

	if (NULL == lpSkill_Info || NULL == lpCompressedInfo)
	{
		ERRLOG0(g_Log, "메모리가 부족합니다.");
		return false;
	}

	CEnsureDeleteArray<char> skill_info(lpSkill_Info);
	CEnsureDeleteArray<char> compress_info(lpCompressedInfo);

	memcpy(lpSkill_Info, m_ProtoTypeArray, dwSkillSize);
	ENCODE_HEADER(lpSkill_Info, dwSkillSize, 3, 3);
	COMPRESS(lpSkill_Info, dwSkillSize, lpCompressedInfo, &dwCompressSize);

	unsigned long dwWritten = 0;

	// 쓰레기(더미) 자료
	HANDLE hTrashFile = CreateFile(szTrashFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hTrashFile == INVALID_HANDLE_VALUE) 
	{ 
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szTrashFile);
		return false; 
	} 

	CEnsureCloseHandle trashFile(hTrashFile);

	unsigned long dwRead            = 0;
	unsigned long dwFileHighSize    = 0;
	unsigned long dwFileSize        = GetFileSize(hTrashFile, &dwFileHighSize);

	char* lpAllocated = new char[dwFileSize];
	CEnsureDeleteArray<char> allocated(lpAllocated);

	if (false == ReadFile(hTrashFile, lpAllocated, dwFileSize, &dwRead, NULL))
	{
		ERRLOG0(g_Log, "쓰레기 파일을 읽을 수 없습니다.");
		return false;
	}

	WriteFile(hFile, &dwFileSize, sizeof(unsigned long), &dwWritten, 0);
	WriteFile(hFile, lpAllocated, dwFileSize, &dwWritten, 0);

	// 올바른 자료
	WriteFile(hFile, &dwSkillSize, sizeof(unsigned long), &dwWritten, 0);       // 원본 데이터 크기
	WriteFile(hFile, lpCompressedInfo, dwCompressSize, &dwWritten, 0);  // 압축된 데이터 크기
	return true;
}


const Skill::ProtoType* CSkillMgr::GetSkillProtoType(unsigned short usSkill_ID)
{
    ProtoTypeArray* lpFirst = m_ProtoTypeArray;
    ProtoTypeArray* lpLast  = m_ProtoTypeArray + m_nSkillNum;
    ProtoTypeArray* lpMid   = NULL;

    size_t nCount   = m_nSkillNum;
    size_t nCount2  = 0;

    for (; 0 < nCount; )
    {
        nCount2 = nCount / 2;
        lpMid = lpFirst + nCount2;

        if (lpMid->m_ProtoTypes[0].m_usSkill_ID < usSkill_ID)
        {
            lpFirst = ++lpMid, nCount -= nCount2 + 1;
        }
        else
        {
            nCount = nCount2;
        }
    }

    return (lpFirst != lpLast && !(usSkill_ID < lpFirst->m_ProtoTypes[0].m_usSkill_ID)) ? 
        lpFirst->m_ProtoTypes : NULL;
}

void CSkillMgr::GetClassSkill(unsigned char cClass, unsigned short *arySkillID, unsigned char *cSkillNum)
{
	*cSkillNum = 0;

	for (size_t nIndex = 0; nIndex < m_nSkillNum; nIndex++)
	{
		Skill::ProtoType protoType = (m_ProtoTypeArray + nIndex)->m_ProtoTypes[0];
		if (true == protoType.m_bIsClassSkill)
		{
			unsigned char cSkillClass = static_cast<unsigned char>((((protoType.m_usSkill_ID - Skill::SKILL_MASK) & 0xFF00) >> 8) & 0x00FF);
			if (cClass == cSkillClass || CClass::GetPreviousJob(cClass) == cSkillClass)
			{
				arySkillID[*cSkillNum] = protoType.m_usSkill_ID;
				(*cSkillNum)++;
			}
		}
	}
}


unsigned char CSkillMgr::ReadStringToTypeValue(CDelimitedFile& DelimitedFile, const char* szColumn, 
											   const CTypeName* TypeArray, const unsigned char nMaxType)
{
	char szReadData[MAX_PATH];
	unsigned char nCount = 0;

	// 등급 구분 읽기.
	if (false == DelimitedFile.ReadString(szColumn, szReadData, MAX_PATH)) 
	{
		ERRLOG0(g_Log, "등급 구분 읽기에 실패하였습니다.");
		return nMaxType;	// 실패시 nMaxType을 리턴.
	}

	for (nCount = 0; nCount < nMaxType; ++nCount) 
	{
		if (0 == strcmp(szReadData, TypeArray[nCount].GetTypeName())) 
		{ 
			break; 
		}
	}

	if (nCount == nMaxType) 
	{
		ERRLOG0(g_Log, "읽어들인 정보가 타입 배열에 존재하지 않습니다.");
		return nMaxType;
	}

	return TypeArray[nCount].GetTypeValue();
}


bool CSkillMgr::CheckParentChildRule(void)
{
	for (ProtoTypeArray* lpPointer = m_ProtoTypeArray; lpPointer != m_ProtoTypeArray + m_nSkillNum; ++lpPointer)
	{
		Skill::ProtoType ProtoType = lpPointer->m_ProtoTypes[0];

		const Skill::ProtoType* lpParent = GetSkillProtoType(ProtoType.m_usParentSkill);
		const Skill::ProtoType* lpChild = GetSkillProtoType(ProtoType.m_usChildSkill);

		if (0 != ProtoType.m_usParentSkill && NULL == lpParent)
		{
			ERRLOG1(g_Log, "앨터너티브 스킬 오류 : 존재하지 않는 스킬이 부모로 설정되었습니다. SkillID:0x%04x", 
				ProtoType.m_usSkill_ID);
			return false;
		}

		if (0 != ProtoType.m_usChildSkill && NULL == lpChild)
		{
			ERRLOG1(g_Log, "앨터너티브 스킬 오류 : 존재하지 않는 스킬이 자식으로 설정되었습니다. SkillID:0x%04x", 
				ProtoType.m_usSkill_ID);
			return false;
		}

		if (NULL == lpParent && NULL == lpChild) { continue; }

		switch (ProtoType.m_eSkillType)
		{
			case Skill::Type::PASSIVE:
			{
				if (0 != ProtoType.m_usChildSkill)
				{
					ERRLOG1(g_Log, "앨터너티브 스킬 오류 : 패시브 스킬이 부모로 설정되었습니다. SkillID:0x%04x", 
						ProtoType.m_usSkill_ID);
					return false;
				}

			} break;

			case Skill::Type::CHANT:
			{		
				if (0 != ProtoType.m_usParentSkill)
				{
					ERRLOG1(g_Log, "앨터너티브 스킬 오류 : 챈트 스킬이 자식으로 설정되었습니다. SkillID:0x%04x", 
						ProtoType.m_usSkill_ID);
					return false;
				}				

				if (0 != ProtoType.m_usChildSkill)
				{
					if (Skill::Type::CHANT == lpChild->m_eSkillType)
					{
						ERRLOG1(g_Log, "앨터너티브 스킬 오류 : 챈트 스킬의 자식으로 챈트 스킬이 설정되었습니다. SkillID:0x%04x", 
							ProtoType.m_usSkill_ID);
						return false;
					}
				}

			} break;
		}
	}

	return true;
}

bool CSkillMgr::CheckData()
{
    unsigned long dwCheckSum = 0;

    // 스킬 체크섬을 빌드하겠소.
    if(NO_ERROR != CCrc32Static::BufferCrc32(reinterpret_cast<const char*>(m_ProtoTypeArray),
        sizeof(ProtoTypeArray) * m_nSkillNum, dwCheckSum))
    {
        ERRLOG0(g_Log, "스킬 스크립트 체크섬 빌드에 실패했습니다.");
        return false;
    }

    return (m_dwCRC32 == dwCheckSum);
}