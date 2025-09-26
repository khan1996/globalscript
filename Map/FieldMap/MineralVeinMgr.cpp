#include "stdafx.h"
#include "MineralVeinMgr.h"

#include <Utility/DelimitedFile.h>
#include <Utility/Math/Math.h>
#include <Utility/Compress/MiniLZO/MiniLZOWrapper.h>
#include <Utility/Resource/EnsureCleanup.h>

#define ENCODEHEADER(Start_In, Length_In, PageNum_In, PageVer_In)   CXORCrypt::GetInstance().EncodeHeader((Start_In), (Length_In), (PageNum_In), (PageVer_In))
#define DECODEHEADER(Start_In, Length_In, PageNum_In, PageVer_In)   CXORCrypt::GetInstance().DecodeHeader((Start_In), (Length_In), (PageNum_In), (PageVer_In))
#define COMPRESS(In, In_len, Out, Out_len)      CMiniLZO::Compress((In), (In_len), (Out), (Out_len))
#define DECOMPRESS(In, In_len, Out, Out_len)    CMiniLZO::Decompress((In), (In_len), (Out), (Out_len))

#include <Network/XORCrypt/XORCrypt.h>

// 클라이언트에서도 쓰이므로 include를 명시한다.
#include <Log/ServerLog.h>
#include "GMMemory.h"

CMineralVeinMgr&	CMineralVeinMgr::GetInstance()
{
	static CMineralVeinMgr ms_this;
	return ms_this;
}


CMineralVeinMgr::CMineralVeinMgr()
{
	Initialize();
}

CMineralVeinMgr::~CMineralVeinMgr()
{
	Destroy();
}

bool	CMineralVeinMgr::Initialize()
{
	std::fill_n( m_dwColorTable[0], int(COLOR_IMAGE_SIZE * COLOR_IMAGE_SIZE), 0);

	return true;
}


void	CMineralVeinMgr::Destroy()
{
	MineralVeinMap::iterator itr = m_mapMineralVein.begin();
	MineralVeinMap::iterator end = m_mapMineralVein.end();

	while ( itr != end )
	{
		if ( itr->second )
		{
			delete itr->second;
			itr->second = NULL;
		}

		++itr;
	}

	m_mapMineralVein.clear();
}


bool	CMineralVeinMgr::LoadMineralVeinsFromFiles(const char* szTextFile, const char* szImageFile)
{
	Destroy() ;

	// 텍스트 파일 체크
	if ( !szTextFile )
	{
		ERRLOG0(g_Log, "광물 배치 스크립트 읽기 실패 : txt 파일이 존재하지 않습니다.");
		return false;
	}

	// 이미지 파일명 체크
    char szTempImageFile[ MAX_PATH ];
	if ( !szImageFile )
	{
		strcpy(szTempImageFile, szTextFile);
		if ( strtok(szTempImageFile, ".") )
		{
			strcat(szTempImageFile, ".tga");

			FILE* fp = fopen(szTempImageFile, "rb");
			if ( !fp )
			{
				ERRLOG1(g_Log, "광물 배치 스크립트 읽기 실패 : 이미지 파일 %s 가 존재하지 않습니다.", szTempImageFile);
				return false;
			}

			fclose(fp);
		}
		else
		{
			ERRLOG1(g_Log, "광물 배치 스크립트 읽기 실패 : 파일명이 이상합니다. - %s", szTextFile);
			return false;
		}
	}
	else
	{
		strcpy(szTempImageFile, szImageFile);
	}

	char szTempBuf[ MAX_PATH ];
	unsigned long dwTempValue;
	int iLineCount = 0;
    CDelimitedFile	DelimitedFile;	// 객체 소멸시, 자동 Close.

	// 매크로에 로그 코드 삽입을 잊지 말 것.
	// 매크로에서 \뒤에 공백이나 문자 삽입되지 않도록 주의할 것.
	// ( '이스케이프 시퀀스가 잘못되었습니다' 에러 발생 )
	#define READ_DATA(ColumnName, Argument) \
		if (!DelimitedFile.ReadData(Argument)) { \
			ERRLOG2(g_Log, "몬스터 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", iLineCount, #ColumnName); \
			return false; \
		}

	#define READ_STRING(ColumnName, Buffer, BufferSize) \
		if (!DelimitedFile.ReadString(Buffer, BufferSize)) { \
			ERRLOG2(g_Log, "몬스터 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", iLineCount, #ColumnName); \
			return false; \
		}

    // 텍스트 파일 로드
	if (!DelimitedFile.Open(szTextFile)) 
	{
		ERRLOG1(g_Log, "광물 배치 스크립트 읽기 실패 : %s 파일이 존재하지 않습니다.", szTextFile);
		return false;
	}

	MineralVeinInfo* pCurrentVeinInfo = NULL;

	while ( DelimitedFile.ReadLine2nd() )
	{
		++iLineCount;

		READ_STRING("TYPE", szTempBuf, MAX_TYPE_STRING_LEN);

		if (!stricmp(szTempBuf, "VEIN_COLOR"))
		{
			READ_STRING("VEIN_COLOR_VALUE", szTempBuf, MAX_HEXA_COLOR_LEN);
			dwTempValue = Math::Convert::Atoi(szTempBuf);

			MineralVeinMap::iterator itr = m_mapMineralVein.find(dwTempValue);
			if (itr != m_mapMineralVein.end())
			{
				ERRLOG1(g_Log, "VeinColor : 0x%08x 중복되는 광맥 ID 가 존재합니다.", dwTempValue);
				return false;
			}
			else
			{
				MineralVeinInfo* pVeinInfo = new MineralVeinInfo;
				pVeinInfo->m_dwVeinColor = dwTempValue;
				m_mapMineralVein.insert(std::make_pair(dwTempValue, pVeinInfo)).second ;

				pCurrentVeinInfo = pVeinInfo;
			}
		}
		else if (!stricmp(szTempBuf, "VEIN_NAME"))
		{
			if ( pCurrentVeinInfo )
			{
				READ_STRING("VEIN_NAME_VALUE", szTempBuf, MineralVeinInfo::MAX_VEIN_NAME_LEN);
				strcpy(pCurrentVeinInfo->m_szVeinName, szTempBuf);
				pCurrentVeinInfo->m_szVeinName[MineralVeinInfo::MAX_VEIN_NAME_LEN - 1] = '\0';
			}
		}
		else if (!stricmp(szTempBuf, "FERTILITY"))
		{
			if ( pCurrentVeinInfo )
			{
				READ_DATA("FERTILITY_VALUE", pCurrentVeinInfo->m_dwMaxFertility);
			}
		}
		else if (!stricmp(szTempBuf, "MINERAL"))
		{
			if ( pCurrentVeinInfo )
			{
				MineralInfo mineralInfo;

				READ_DATA("MINERAL_ID", mineralInfo.m_dwMineralID);
				READ_DATA("MIN_VALUE", mineralInfo.m_cMin);
				READ_DATA("MAX_VALUE", mineralInfo.m_cMax);

				pCurrentVeinInfo->m_lstMineralInfo.push_back(mineralInfo);
			}
		}
		else
		{
			ERRLOG1(g_Log, "광물 배치 스크립트 로드 실패 : 존재하지 않는 타입이 있습니다. Type - %s", szTempBuf);
			return false;
		}
	}

	// 이미지 파일 로드
	FILE* pTgaFile = fopen(szTempImageFile, "rb");
	if ( !pTgaFile )
	{
		ERRLOG1(g_Log, "광물 배치 스크립트 읽기 실패 : 이미지 파일 %s 가 존재하지 않습니다.", szTempImageFile);
		return false;
	}

	sTargaHeader	tgaHeader;
	bool			bFlipVertical;
	int				iWidth, iHeight, iScanLineSize;

	if (!fread(&tgaHeader, sizeof(sTargaHeader), 1, pTgaFile))
	{
		ERRLOG1(g_Log, "광물 배치 스크립트 읽기 실패 : TGA 파일(%s) 헤더 읽기 실패", szTempImageFile);
		return false;
	}

	bFlipVertical = ((tgaHeader.ImageDescriptor & 0x20) != 0 );
	tgaHeader.ImageDescriptor = tgaHeader.ImageDescriptor & 0xF;
	iWidth = tgaHeader.Width;
	iHeight = tgaHeader.Height;
	iScanLineSize = iWidth * (tgaHeader.PixelSize / 8);

	if ( bFlipVertical )
	{
		for (int i=0; i<iHeight; ++i)
		{
			fread(m_dwColorTable[i], iScanLineSize, 1, pTgaFile);
		}
	}
	else
	{
		for (int i=iHeight-1; i>=0; --i)
		{
			fread(m_dwColorTable[i], iScanLineSize, 1, pTgaFile);
		}
	}

	fclose( pTgaFile );

	return true;
}

bool	CMineralVeinMgr::SaveMineralVeinsToBinary(const char* szBinaryFile, const char* szTrashFile)
{
	if ( m_mapMineralVein.empty() )
	{
		return false;        
	}

	HANDLE hFile = CreateFile(szBinaryFile,	GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	CEnsureCloseHandle writeFile(hFile);

	size_t tempSize = 0;
	unsigned long dwVeinCount = static_cast<unsigned long>(m_mapMineralVein.size());
	MineralVeinMap::iterator itr = m_mapMineralVein.begin();
	MineralVeinMap::iterator end = m_mapMineralVein.end();

	while (itr != end)
	{
		MineralVeinInfo* pVeinInfo = itr->second;
		if ( pVeinInfo )
		{
			tempSize += pVeinInfo->m_lstMineralInfo.size() * sizeof(MineralInfo) + sizeof(unsigned long);
		}

		++itr;
	}
	tempSize += m_mapMineralVein.size() * sizeof(MineralVeinInfo) + sizeof(unsigned long);
	tempSize += COLOR_IMAGE_SIZE * COLOR_IMAGE_SIZE * sizeof(unsigned long);
	const size_t MAX_SCRIPT_FILE_SIZE = tempSize;

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

	// MineralVeinInfo 갯수 저장
	memcpy(pInputBuffer, &dwVeinCount, sizeof(unsigned long));
	dwInputBufferSize += sizeof(unsigned long);
	pInputBuffer += sizeof(unsigned long);

	// Image 저장
	for (int i=0; i<COLOR_IMAGE_SIZE; ++i)
	{
		memcpy(pInputBuffer, m_dwColorTable[i], sizeof(unsigned long) * COLOR_IMAGE_SIZE);
		dwInputBufferSize += sizeof(unsigned long) * COLOR_IMAGE_SIZE;
		pInputBuffer += sizeof(unsigned long) * COLOR_IMAGE_SIZE;
	}

	unsigned long dwMineralCount;
	itr = m_mapMineralVein.begin();
	end = m_mapMineralVein.end();

	while ( itr != end )
	{
		MineralVeinInfo* pVeinInfo = itr->second;
		if ( pVeinInfo )
		{
			// 광물수 저장
			dwMineralCount = static_cast<unsigned long>( pVeinInfo->m_lstMineralInfo.size() );
			memcpy(pInputBuffer, &dwMineralCount, sizeof(unsigned long));
			dwInputBufferSize += sizeof(unsigned long);
			pInputBuffer += sizeof(unsigned long);

			// Vein 정보 저장
			memcpy(pInputBuffer, pVeinInfo, 56); //sizeof(MineralVeinInfo));
			dwInputBufferSize += 56; //sizeof(MineralVeinInfo);
			pInputBuffer += 56; //sizeof(MineralVeinInfo);

			// 광물 정보 저장
			MineralInfoList::iterator listItr = pVeinInfo->m_lstMineralInfo.begin();
			MineralInfoList::iterator listEnd = pVeinInfo->m_lstMineralInfo.end();

			while ( listItr != listEnd )
			{
				const MineralInfo& mineralInfo = (*listItr);
                memcpy(pInputBuffer, &mineralInfo, sizeof(MineralInfo));
				dwInputBufferSize += sizeof(MineralInfo);
				pInputBuffer += sizeof(MineralInfo);

				++listItr;
			}
		}

		++itr;
	}

	ENCODEHEADER(InputStartPointer, dwInputBufferSize, 0, 3);
	COMPRESS(InputStartPointer, dwInputBufferSize, pOutputBuffer, &dwOutputBufferSize);

	// 파일에 쓰기
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

bool	CMineralVeinMgr::LoadMineralVeinsFromBinary(const char* szBinaryFile)
{
	Destroy() ;

	HANDLE hFile = CreateFile(szBinaryFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
	DECODEHEADER(lpBuffer, dwBufferSize, 0, 3);

	// MineralVein 갯수 얻어오기
	unsigned long dwVeinCount = *reinterpret_cast<unsigned long*>(lpBuffer);
	lpBuffer += sizeof(unsigned long);

	// Image 읽기
	for (int h=0; h<COLOR_IMAGE_SIZE; ++h)
	{
		memcpy(m_dwColorTable[h], lpBuffer, sizeof(unsigned long) * COLOR_IMAGE_SIZE);
		lpBuffer += sizeof(unsigned long) * COLOR_IMAGE_SIZE;
	}

	for (int i=0; i<(int)dwVeinCount; ++i)
	{
		// 광물 갯수 읽기
		unsigned long dwMineralCount = *reinterpret_cast<unsigned long*>(lpBuffer);
		lpBuffer += sizeof(unsigned long);

		MineralVeinInfo* lpVeinInfo = new MineralVeinInfo;

		// 광맥 정보 얻기
        MineralVeinInfo* lpTempVeinInfo = reinterpret_cast<MineralVeinInfo*>(lpBuffer);
		lpBuffer += 56; // std::list doesnt have a static size between VS versions

		if ( lpVeinInfo && lpTempVeinInfo )
		{
			// 광맥 정보 셋팅
			lpVeinInfo->m_dwVeinColor = lpTempVeinInfo->m_dwVeinColor;
			strcpy(lpVeinInfo->m_szVeinName, lpTempVeinInfo->m_szVeinName);
			lpVeinInfo->m_dwMaxFertility = lpTempVeinInfo->m_dwMaxFertility;
			lpVeinInfo->m_dwNowFertility = 0;

			for (int j=0; j<(int)dwMineralCount; ++j)
			{
				// 광물 정보 얻어와서 셋팅
				MineralInfo* lpMineralInfo = reinterpret_cast<MineralInfo*>(lpBuffer);
				lpBuffer += sizeof(MineralInfo);

				lpVeinInfo->m_lstMineralInfo.push_back(*lpMineralInfo);
			}

			// 광맥 추가
			m_mapMineralVein.insert(std::make_pair(lpVeinInfo->m_dwVeinColor, lpVeinInfo)).second;
		}
		else
		{
			ERRLOG0(g_Log, "광물 배치 스크립트 로드 실패 : 읽어온 정보 혹은 new 로 생성한 포인터가 NULL 입니다.");
			return false;
		}
	}

	return true;
}


MineralVeinInfo*	CMineralVeinMgr::GetMineralVein(unsigned long dwVeinColor) const
{
	if ( 0 != dwVeinColor )
	{
		MineralVeinMap::const_iterator itr = m_mapMineralVein.find(dwVeinColor);
		if ( itr != m_mapMineralVein.end() )
		{
			return itr->second;
		}
	}

	return NULL;
}

MineralVeinInfo*	CMineralVeinMgr::GetMineralVein(float fX, float fZ) const
{
	int iIdxX = static_cast<int>( (fX - 315) * 512 / 3465.f ) ;
	int iIdxZ = static_cast<int>( 512 - (fZ - 315) * 512 / 3465.f ) ;

	unsigned long dwVeinColor = m_dwColorTable[ iIdxZ ][ iIdxX ];

	return GetMineralVein(dwVeinColor);
}

unsigned long		CMineralVeinMgr::GetVeinColor(float fX, float fZ) const
{
	int iIdxX = static_cast<int>( (fX - 315) * 512 / 3465.f ) ;
	int iIdxZ = static_cast<int>( 512 - (fZ - 315) * 512 / 3465.f ) ;

	return m_dwColorTable[ iIdxZ ][ iIdxX ];
}

const char*			CMineralVeinMgr::GetVeinName(float fX, float fZ) const
{
	MineralVeinInfo* pVeinInfo = GetMineralVein(fX, fZ);
	if (pVeinInfo)
	{
		return pVeinInfo->m_szVeinName;
	}

	return NULL;
}

int	CMineralVeinMgr::GetVeinNum() const
{
	return static_cast<int>( m_mapMineralVein.size() );
}

void	CMineralVeinMgr::InitializeMiningCampNum()
{
	m_mapCampNum.clear();
}

void	CMineralVeinMgr::CalculateMiningCampNum(float fX, float fZ)
{
	MineralVeinInfo* lpVeinInfo = GetMineralVein(fX, fZ);
	if ( lpVeinInfo )
	{
		VeinToCampNumMap::iterator itr = m_mapCampNum.find( lpVeinInfo->m_dwVeinColor );
		if ( itr != m_mapCampNum.end() )
		{
			++(itr->second);
		}
		else
		{
			m_mapCampNum.insert( std::make_pair(lpVeinInfo->m_dwVeinColor, 1) ).second;
		}
	}
}

int		CMineralVeinMgr::GetMiningCampNum(unsigned long dwVeinColor)
{
	VeinToCampNumMap::iterator itr = m_mapCampNum.find( dwVeinColor );
	if ( itr != m_mapCampNum.end() )
	{
		return itr->second;
	}

	return 0;
}

void	CMineralVeinMgr::CalculateFertility()
{
	MineralVeinMap::iterator itr = m_mapMineralVein.begin();
	MineralVeinMap::iterator end = m_mapMineralVein.end();

	while ( itr != end )
	{
		MineralVeinInfo* lpVeinInfo = itr->second;
		if ( lpVeinInfo )
		{
			VeinToCampNumMap::iterator pos = m_mapCampNum.find( itr->first );
			if ( pos != m_mapCampNum.end() )
			{
				if ( (unsigned long)pos->second > lpVeinInfo->m_dwNowFertility )
				{
					lpVeinInfo->m_dwNowFertility = 0;
				}
				else
				{
					lpVeinInfo->m_dwNowFertility -= pos->second;
				}
			}
			else if (lpVeinInfo->m_dwNowFertility < lpVeinInfo->m_dwMaxFertility)
			{
				unsigned long dwAddValue = static_cast<unsigned long>(ceil(lpVeinInfo->m_dwMaxFertility * 0.1f));
				if (dwAddValue + lpVeinInfo->m_dwNowFertility > lpVeinInfo->m_dwMaxFertility)
				{
					lpVeinInfo->m_dwNowFertility = lpVeinInfo->m_dwMaxFertility;
				}
				else
				{
					lpVeinInfo->m_dwNowFertility += dwAddValue;
				}
			}
		}

		++itr;
	}
}

void	CMineralVeinMgr::SerializeOutFertility(unsigned long* lpData_Out, unsigned short& wSize)
{
	MineralVeinMap::iterator itr = m_mapMineralVein.begin();
	MineralVeinMap::iterator end = m_mapMineralVein.end();

	while ( itr != end )
	{
		MineralVeinInfo* lpVeinInfo = itr->second;
		if ( lpVeinInfo )
		{
			*lpData_Out = lpVeinInfo->m_dwVeinColor;
			++lpData_Out;
			wSize += sizeof(unsigned long);

			*lpData_Out = lpVeinInfo->m_dwNowFertility;
			++lpData_Out;
			wSize += sizeof(unsigned long);
		}

		++itr;
	}
}


// DB 에서 얻어온 현재 지력값을 설정하는 함수
bool	CMineralVeinMgr::SetNowFertility(unsigned short wNum, unsigned long* szDataFromDB)
{
	unsigned long dwVeinColor, dwNowFertility;

	for (int i=0; i<wNum; ++i)
	{
		dwVeinColor = *szDataFromDB;
		++szDataFromDB;

		dwNowFertility = *szDataFromDB;
		++szDataFromDB;

		SetNowFertility(dwVeinColor, dwNowFertility);
	}

	return true;
}

bool	CMineralVeinMgr::SetNowFertility(unsigned long dwVeinColor, unsigned long dwFertility)
{
	MineralVeinMap::iterator pos = m_mapMineralVein.find(dwVeinColor);
	if (pos != m_mapMineralVein.end())
	{
		MineralVeinInfo* lpVeinInfo = pos->second;
		lpVeinInfo->m_dwNowFertility = dwFertility;

		return true;
	}

	return false;
}
