
#include "stdafx.h"	

#include <Utility/Math/Math.h>
#include <Utility/Compress/MiniLZO/MiniLZOWrapper.h>
#include <Utility/Resource/EnsureCleanup.h>

#include <Network/XORCrypt/XORCrypt.h>

// 클라이언트에서도 쓰이므로 include를 명시한다.
#include <Log/ServerLog.h>

#define ENCODEHEADER(Start_In, Length_In, PageNum_In, PageVer_In)   CXORCrypt::GetInstance().EncodeHeader((Start_In), (Length_In), (PageNum_In), (PageVer_In))
#define DECODEHEADER(Start_In, Length_In, PageNum_In, PageVer_In)   CXORCrypt::GetInstance().DecodeHeader((Start_In), (Length_In), (PageNum_In), (PageVer_In))
#define COMPRESS(In, In_len, Out, Out_len)      CMiniLZO::Compress((In), (In_len), (Out), (Out_len))
#define DECOMPRESS(In, In_len, Out, Out_len)    CMiniLZO::Decompress((In), (In_len), (Out), (Out_len))

#include <vector>
#include <algorithm>
#include <functional>
#include "MonsterMgr.h"
#include "GMMemory.h"

CMonsterMgr CMonsterMgr::ms_this;

const char* CMonsterMgr::m_szMonsterScriptFileName = "./Script/Game/MonsterProtoType.txt";


CMonsterMgr::CMonsterMgr(void)
:   m_nMonsterNum(0), m_ProtoTypeArray(NULL)
{

}

CMonsterMgr::~CMonsterMgr(void)
{
	ClearProtoType();	
}

void CMonsterMgr::ClearProtoType()
{
  	if (NULL != m_ProtoTypeArray) 
    {
        delete [] m_ProtoTypeArray;
    }
}

bool CMonsterMgr::LoadMonstersFromFile(const char* szFileName)
{
	int nIndex = 0;
    int nLineCount = 0;
    char strTemp[MAX_PATH];
	float fTemp = 0;

	CDelimitedFile                  DelimitedFile;	// 객체 소멸시, 자동 Close.
    std::vector<MonsterProtoType>   monsterProtoTypeVector;

    monsterProtoTypeVector.reserve(1000);
    MonsterProtoType    tempProtoType;

    // 매크로에 로그 코드 삽입을 잊지 말 것.
	// 매크로에서 \뒤에 공백이나 문자 삽입되지 않도록 주의할 것.
	// ( '이스케이프 시퀀스가 잘못되었습니다' 에러 발생 )
	#define READ_DATA(ColumnName, Argument) \
		if (!DelimitedFile.ReadData(Argument)) { \
            ERRLOG2(g_Log, "몬스터 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount, #ColumnName); \
			return false; \
		}
	
	#define READ_STRING(ColumnName, Buffer, BufferSize) \
		if (!DelimitedFile.ReadString(Buffer, BufferSize)) { \
            ERRLOG2(g_Log, "몬스터 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount, #ColumnName); \
			return false; \
		}

	#define READ_DATA_ARRAY(ColumnName, Argument, ArgumentNum) \
		for (nIndex=0; nIndex < ArgumentNum; ++nIndex) { \
			READ_DATA(ColumnName, Argument[nIndex]); \
		}

	#define READ_SKILLID_ARRAY(ColumnName, Argument, ArgumentNum) \
		for (nIndex=0; nIndex < ArgumentNum; ++nIndex) { \
			if (!DelimitedFile.ReadString(strTemp, MAX_PATH)) { \
				ERRLOG2(g_Log, "몬스터 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount, #ColumnName); \
				return false; \
			} \
			char strString[MAX_PATH] = "0x0000"; \
			strcat(strString, (strTemp+2)); \
			Argument[nIndex] = static_cast<unsigned short>(Math::Convert::Atoi(strString)); \
		}

	#define READ_DATA_BOOL(ColumnName, Argument) \
		if (!DelimitedFile.ReadString(strTemp, MAX_PATH)) { \
            ERRLOG2(g_Log, "몬스터 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount, #ColumnName); \
			return false; \
		} \
        Argument = (!strcmp(strTemp, "O")) ? true : false;


	if (!DelimitedFile.Open(szFileName ? szFileName : m_szMonsterScriptFileName)) 
	{
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szFileName ? szFileName : m_szMonsterScriptFileName);
		return false;
	}

	while (DelimitedFile.ReadLine()) 
	{
        ++nLineCount;

		// 순서가 바뀌면 곤란하다니깐~!!! (버럭!)
		READ_DATA("KID", tempProtoType.m_MonsterInfo.m_dwKID);
		
		READ_STRING("이름", tempProtoType.m_MonsterInfo.m_strName, MonsterInfo::MAX_NAME_LEN);
		READ_STRING("모델링 형태 플래그", tempProtoType.m_MonsterInfo.m_strModelingFlag, MonsterInfo::MAX_MODELING_FLAG_LENGTH);

		READ_DATA_BOOL("LOD 사용 유무", tempProtoType.m_MonsterInfo.m_bUseLOD);

		READ_DATA("국적", tempProtoType.m_MonsterInfo.m_cNation);

		READ_STRING("스킬패턴", strTemp, MAX_PATH);
        tempProtoType.m_MonsterInfo.m_cSkillPattern = MonsterInfo::GetMonsterPattern(strTemp);

		READ_DATA_BOOL("인챈트무시여부", tempProtoType.m_MonsterInfo.m_bIgnoreEnchant);

		// 모션 정보
		tempProtoType.m_MonsterInfo.m_MonsterMotions[0].m_wAction = MonsterInfo::Z3D_CA_WALK;												
		READ_DATA("걷기프레임", tempProtoType.m_MonsterInfo.m_MonsterMotions[0].m_dwFrame);

		READ_DATA("걷기거리", tempProtoType.m_MonsterInfo.m_MonsterMotions[0].m_fVelocity);
		tempProtoType.m_MonsterInfo.m_MonsterMotions[0].m_fVelocity /= 100.0f;  // 스크립트 값은cm 임. 미터로 변환함.

		tempProtoType.m_MonsterInfo.m_MonsterMotions[1].m_wAction = MonsterInfo::Z3D_CA_RUN;											
		READ_DATA("달리기프레임", tempProtoType.m_MonsterInfo.m_MonsterMotions[1].m_dwFrame);

		READ_DATA("달리기거리", tempProtoType.m_MonsterInfo.m_MonsterMotions[1].m_fVelocity);
		tempProtoType.m_MonsterInfo.m_MonsterMotions[1].m_fVelocity /= 100.0f;  // 스크립트 값은cm 임. 미터로 변환함.

		tempProtoType.m_MonsterInfo.m_MonsterMotions[2].m_wAction = MonsterInfo::Z3D_CA_ATTACK;							
		READ_DATA("공격프레임", tempProtoType.m_MonsterInfo.m_MonsterMotions[2].m_dwFrame);

		tempProtoType.m_MonsterInfo.m_MonsterMotions[3].m_wAction = MonsterInfo::Z3D_CA_CASTING;						
		READ_DATA("스킬프레임", tempProtoType.m_MonsterInfo.m_MonsterMotions[3].m_dwFrame);

		READ_DATA_ARRAY("타격박스", tempProtoType.m_MonsterInfo.m_fHitBox, MonsterInfo::MAX_HITBOX_NUM);
		READ_DATA("공격거리", tempProtoType.m_MonsterInfo.m_wAttackRange);
		READ_DATA("공격각도", tempProtoType.m_MonsterInfo.m_fAttackAngle);

        // 기본 정보
		READ_DATA("기본 경험점", tempProtoType.m_CreatureStatus.m_nExp);						
		READ_DATA("레벨", tempProtoType.m_CreatureStatus.m_nLevel);							
		READ_DATA("최소데미지", tempProtoType.m_CreatureStatus.m_StatusInfo.m_lMinDamage);	
        READ_DATA("최대데미지", tempProtoType.m_CreatureStatus.m_StatusInfo.m_lMaxDamage);	
		READ_DATA("명중", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wHitRate);
		READ_DATA("방어력", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wArmor);			
		READ_DATA("회피", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wEvade);
		READ_DATA("블록", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wBlock);

		READ_DATA("DRC", fTemp);	// 기획팀의 편의를 위해 스크립트에서만 존재

		READ_DATA("크리티컬", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wCritical);	
		READ_DATA("크리티컬타입", tempProtoType.m_CreatureStatus.m_StatusInfo.m_cCriticalType);	
		READ_DATA("마법력", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wMagicPower);			
		READ_DATA("저항력", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wMagicResist);			
		READ_DATA("속도", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wSpeed);			
		READ_DATA("HP Max", tempProtoType.m_CreatureStatus.m_StatusInfo.m_nMaxHP);				
		READ_DATA("MP Max", tempProtoType.m_CreatureStatus.m_StatusInfo.m_nMaxMP);				
		READ_DATA("HP 회복량", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wHPRegen);		
		READ_DATA("MP 회복량", tempProtoType.m_CreatureStatus.m_StatusInfo.m_wMPRegen);		

        // 기타
		READ_DATA("사이즈", tempProtoType.m_MonsterInfo.m_fSize);
		READ_DATA("스킬레벨", tempProtoType.m_MonsterInfo.m_cSkillLevel);

		READ_DATA_BOOL("충돌타입여부", tempProtoType.m_MonsterInfo.m_bCollision);
		READ_DATA_BOOL("선제공격여부", tempProtoType.m_MonsterInfo.m_bFirstAttack);
		READ_DATA_BOOL("귀환여부", tempProtoType.m_MonsterInfo.m_bReturnPosition);

		READ_DATA_BOOL("고정레벨갭적용여부", tempProtoType.m_MonsterInfo.m_bFixLevelGap);
		READ_DATA("고정레벨갭", tempProtoType.m_MonsterInfo.m_cFixLevelGap);

		READ_DATA("스킬사용 확률", tempProtoType.m_MonsterInfo.m_wSkillUseRate);		

 		READ_SKILLID_ARRAY("몬스터 스킬ID", tempProtoType.m_MonsterInfo.m_wSkillID, MonsterInfo::MAX_SKILL_PATTERN);
		READ_DATA("부여스킬 스펠타입", tempProtoType.m_MonsterInfo.m_cEnchantSpellType);
		READ_DATA("챈트스킬 스펠타입", tempProtoType.m_MonsterInfo.m_cChantSpellType);
		READ_DATA("스킬이펙트 사이즈", tempProtoType.m_MonsterInfo.m_fSkillEffectSize);
		
		READ_DATA("리스폰타임", tempProtoType.m_MonsterInfo.m_dwRespawnTime);
		READ_DATA_ARRAY("오리지날아이템", tempProtoType.m_MonsterInfo.m_aryAwardItem, MonsterInfo::MAX_ORIGINAL_ITEM_NUM);
		READ_DATA_ARRAY("아이템드랍률", tempProtoType.m_MonsterInfo.m_aryDropRate, MonsterInfo::MAX_AWARD_KIND);
		READ_DATA("Standard-Option Item Base", tempProtoType.m_MonsterInfo.m_cStandardBaseNum);
		READ_DATA("Over-Option Item Base", tempProtoType.m_MonsterInfo.m_cOverBaseNum);

		READ_STRING("석상 효과 타입", strTemp, MAX_PATH);
		tempProtoType.m_MonsterInfo.m_cStatueEffectType = MonsterInfo::GetStatueEffectType(strTemp);
		READ_DATA("석상 효과 퍼센트", tempProtoType.m_MonsterInfo.m_cStatueEffectPercent);
		READ_DATA_BOOL("석상 리스폰 가능 여부", tempProtoType.m_MonsterInfo.m_bStatueRespawnEnable);

		// 공성 오브젝트 정보 읽어들이기
		READ_DATA("개발 비용", tempProtoType.m_MonsterInfo.m_dwDevelopGold);
		READ_DATA("개발 속도", tempProtoType.m_MonsterInfo.m_cDevelopSpeed);
		READ_DATA("업그레이드 비용", tempProtoType.m_MonsterInfo.m_dwUpgradeGold);
		READ_DATA("업그레이드 속도", tempProtoType.m_MonsterInfo.m_cUpgradeSpeed);

		READ_DATA("개발 비용 절감", tempProtoType.m_MonsterInfo.m_fDevelopGoldDown);
		READ_DATA("개발 속도 향상", tempProtoType.m_MonsterInfo.m_fDevelopSpeedUp);
		READ_DATA("업그레이드 비용 절감", tempProtoType.m_MonsterInfo.m_fUpgradeGoldDown);
		READ_DATA("업그레이드 속도 향상", tempProtoType.m_MonsterInfo.m_fUpgradeSpeedUp);
		READ_DATA("방어력 향상", tempProtoType.m_MonsterInfo.m_fDefenseUp);
		READ_DATA("공경력 향상", tempProtoType.m_MonsterInfo.m_fOffenseUp);
		READ_DATA("HP 향상", tempProtoType.m_MonsterInfo.m_fHPUp);
		READ_DATA("총 수입 보너스", tempProtoType.m_MonsterInfo.m_fBonusRate);
		READ_DATA("PC 리스폰 속도 향상", tempProtoType.m_MonsterInfo.m_fRespawnSpeedUp);

        monsterProtoTypeVector.push_back(tempProtoType);
	}

    std::sort(monsterProtoTypeVector.begin(), monsterProtoTypeVector.end());

    for (std::vector<MonsterProtoType>::iterator itr = monsterProtoTypeVector.begin();
        itr != monsterProtoTypeVector.end() - 1; ++itr)
    {
        if (itr->m_MonsterInfo.m_dwKID == (itr+1)->m_MonsterInfo.m_dwKID)
        {
            ERRLOG1(g_Log, "겹치는 몬스터 종류 ID가 있습니다. 종류ID:%d", itr->m_MonsterInfo.m_dwKID);
            return false;
        }
    }

    m_nMonsterNum = monsterProtoTypeVector.size();
    m_ProtoTypeArray = new MonsterProtoType[m_nMonsterNum];
    if (NULL == m_ProtoTypeArray) 
    {
        ERRLOG0(g_Log, "몬스터 스크립트 초기화 실패 : 메모리 부족");
        return false;
    }
        
    std::copy(monsterProtoTypeVector.begin(), monsterProtoTypeVector.end(), m_ProtoTypeArray);
	return true;
}


class CFindProtoTypeFromKID : public std::unary_function<CMonsterMgr::MonsterProtoType, bool>
{
public:

	explicit CFindProtoTypeFromKID(unsigned long dwKID) 
	:	m_dwKID(dwKID) 
	{ }		

	bool operator() (CMonsterMgr::MonsterProtoType& protoType) 
	{ 
		return (m_dwKID == protoType.m_MonsterInfo.m_dwKID); 
	}


private:

	const unsigned long m_dwKID;
};

const CMonsterMgr::MonsterProtoType* CMonsterMgr::GetMonsterProtoType(unsigned long dwKID)
{
	MonsterProtoType* lpFirst = m_ProtoTypeArray;
    MonsterProtoType* lpLast  = m_ProtoTypeArray + m_nMonsterNum;
    MonsterProtoType* lpMid   = NULL;

    size_t nCount   = m_nMonsterNum;
    size_t nCount2  = 0;

    for (; 0 < nCount; )
    {
        nCount2 = nCount / 2;
        lpMid = lpFirst + nCount2;

        if (lpMid->m_MonsterInfo.m_dwKID < dwKID)
        {
            lpFirst = ++lpMid, nCount -= nCount2 + 1;
        }
        else
        {
            nCount = nCount2;
        }
    }

    return (lpFirst != lpLast && !(dwKID < lpFirst->m_MonsterInfo.m_dwKID)) ? lpFirst : NULL;
}


const CMonsterMgr::MonsterProtoType* CMonsterMgr::GetMonsterProtoType(char* szName)
{
	for (size_t nIndex = 0; nIndex < m_nMonsterNum; nIndex++)
	{
		if (0 == strncmp(szName, m_ProtoTypeArray[nIndex].m_MonsterInfo.m_strName, MonsterInfo::MAX_NAME_LEN))
		{
			return m_ProtoTypeArray + nIndex;
		}
	}

	return NULL;
}


bool CMonsterMgr::LoadMonstersFromBinary(const char* szFileNameBinary)
{
	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? m_szMonsterScriptFileName : szFileNameBinary,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 
	if (hFile == INVALID_HANDLE_VALUE) { return false; } 

    CEnsureCloseHandle readFile(hFile);

	unsigned long dwRead = 0;
	unsigned long dwFileHighSize = 0;
	unsigned long dwFileSize = GetFileSize(hFile, &dwFileHighSize);

	char* lpAllocated = new char[dwFileSize];    
    if (NULL == lpAllocated)
    {
        return false;
    }
    
    CEnsureDeleteArray<char> allocated(lpAllocated);
    
	if (!ReadFile(hFile, lpAllocated, dwFileSize, &dwRead, NULL)) 
    {
		return false;
	}

	MonsterProtoType*   lpProtoType = 0;
	MonsterInfo*        lpMonsterInfo = 0;
	CreatureStatus*     lpCreatureStatus = 0;

	unsigned long dwStructSize = 0;
	unsigned long dwHeaderSize = sizeof(unsigned long) + *reinterpret_cast<unsigned long*>(lpAllocated) + sizeof(unsigned long);
    unsigned long dwBufferSize = *reinterpret_cast<unsigned long*>(lpAllocated + dwHeaderSize - sizeof(unsigned long));

    char* lpBuffer = new char[dwBufferSize];
	if (NULL == lpBuffer)
    { 
        return false;
    }

    CEnsureDeleteArray<char> buffer(lpBuffer);

	char* lpBufferStartPointer = lpBuffer;
	
	DECOMPRESS(lpAllocated + dwHeaderSize, dwFileSize - dwHeaderSize, lpBuffer, &dwBufferSize);
    DECODEHEADER(lpBuffer, dwBufferSize, 3, 2);

    m_nMonsterNum = dwBufferSize / (sizeof(MonsterInfo) + sizeof(CreatureStatus));
	m_ProtoTypeArray = new MonsterProtoType[m_nMonsterNum];
	if (NULL == m_ProtoTypeArray)
    {
        return false;
    }

    for (size_t nIndex = 0; nIndex < m_nMonsterNum; ++nIndex)
    {
		lpMonsterInfo       = reinterpret_cast<MonsterInfo*>(lpBuffer);
		lpCreatureStatus    = reinterpret_cast<CreatureStatus*>(lpBuffer + sizeof(MonsterInfo));

		m_ProtoTypeArray[nIndex].m_MonsterInfo = MonsterInfo(*lpMonsterInfo);
		dwBufferSize -= sizeof(MonsterInfo);
        lpBuffer += sizeof(MonsterInfo);

		m_ProtoTypeArray[nIndex].m_CreatureStatus = CreatureStatus(*lpCreatureStatus);
		dwBufferSize -= sizeof(CreatureStatus);
		lpBuffer += sizeof(CreatureStatus);		
    }

	return true;
}


bool CMonsterMgr::SaveMonstersToBinary(const char* szFileNameBinary, const char* szTrashFile)
{
	if (0 == m_ProtoTypeArray)
    {
        return false;        
    }

	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? m_szMonsterScriptFileName : szFileNameBinary,
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) { return false; }

    CEnsureCloseHandle writeFile(hFile);

    const size_t MAX_SCRIPT_FILE_SIZE = m_nMonsterNum * (sizeof(MonsterInfo) + sizeof(CreatureStatus));

	char *pInputBuffer  = new char[MAX_SCRIPT_FILE_SIZE];
	char *pOutputBuffer = new char[MAX_SCRIPT_FILE_SIZE];

    CEnsureDeleteArray<char> input(pInputBuffer);
    CEnsureDeleteArray<char> output(pOutputBuffer);
    
    if (0 == pInputBuffer || 0 == pOutputBuffer)
    {
		return false;
	}

	char *InputStartPointer = pInputBuffer;
	char *OutputStartPointer = pOutputBuffer;

	unsigned long dwInputBufferSize = 0;
	unsigned long dwOutputBufferSize = 0;

	for (size_t nCount = 0; nCount < m_nMonsterNum; ++nCount)
    {
		memcpy(pInputBuffer, &m_ProtoTypeArray[nCount].m_MonsterInfo, sizeof(MonsterInfo));
		dwInputBufferSize += sizeof(MonsterInfo);		
		pInputBuffer += sizeof(MonsterInfo);

		memcpy(pInputBuffer, &m_ProtoTypeArray[nCount].m_CreatureStatus, sizeof(CreatureStatus));
		dwInputBufferSize += sizeof(CreatureStatus);	
		pInputBuffer += sizeof(CreatureStatus);
	}

    ENCODEHEADER(InputStartPointer, dwInputBufferSize, 3, 2);
    COMPRESS(InputStartPointer, dwInputBufferSize, pOutputBuffer, &dwOutputBufferSize);

	unsigned long dwWritten = 0;

	// 쓰레기(더미) 자료
	HANDLE hTrashFile = CreateFile(szTrashFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
    if (NULL == lpAllocated)
    {
		ERRLOG0(g_Log, "메모리 할당에 실패하였습니다.");
        return false;
    }

    CEnsureDeleteArray<char> allocated(lpAllocated);

	if (false == ReadFile(hTrashFile, lpAllocated, dwFileSize, &dwRead, NULL)) 
    {
		ERRLOG0(g_Log, "쓰레기 파일을 읽을 수 없습니다.");
		return false;
	}

	WriteFile(hFile, &dwFileSize, sizeof(unsigned long), &dwWritten, 0);
	WriteFile(hFile, lpAllocated, dwFileSize, &dwWritten, 0);

	// 올바른 자료
	WriteFile(hFile, &dwInputBufferSize, sizeof(unsigned long), &dwWritten, 0);
	WriteFile(hFile, pOutputBuffer, dwOutputBufferSize, &dwWritten, 0);
	return true;
}
