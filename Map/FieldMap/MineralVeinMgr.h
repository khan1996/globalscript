#ifndef __MINERAL_VEIN_MANAGER_H__
#define __MINERAL_VEIN_MANAGER_H__

#pragma once

#define g_MinveralVeinMgr CMineralVeinMgr::GetInstance()

#include <list>
#include <map>

// 광물 정보 구조체
struct MineralInfo
{
	unsigned short	m_dwMineralID;	// 광물 Item ID
	unsigned char	m_cMin;			// 광물 추출 Min
	unsigned char	m_cMax;			// 광물 추출 Max
};
typedef std::list<MineralInfo>	MineralInfoList;


// 광맥 정보 구조체
struct MineralVeinInfo
{
	enum Length
	{
		MAX_VEIN_NAME_LEN	= 32
	};

	char				m_szVeinName[ MAX_VEIN_NAME_LEN ];	// 광맥 이름
	unsigned long		m_dwVeinColor;						// 광맥의 색상 ID
	unsigned long		m_dwMaxFertility;					// 광맥의 최대 지력
	unsigned long		m_dwNowFertility;					// 광맥의 현재 지력	(DB 에서 얻어오는 값)
	MineralInfoList		m_lstMineralInfo;					// 광물 리스트
};
typedef std::map<unsigned long, MineralVeinInfo*>	MineralVeinMap;


// TGA 파일 헤더
#pragma pack (1)
struct sTargaHeader {
	unsigned char	IDLength, ColorMapType, ImageType ;
	unsigned short	ColorMapOrigin, ColorMapSize ;
	unsigned char	ColorMapEntrySize ;
	unsigned short	XOrigin, YOrigin, Width, Height ;
	unsigned char	PixelSize ;
	unsigned char	ImageDescriptor ;
};
#pragma pack (8)

// 광맥 관리 클래스
class CMineralVeinMgr
{
public:
	enum Const
	{
		MAX_TYPE_STRING_LEN	= 32,
		MAX_HEXA_COLOR_LEN	= 10,

		COLOR_IMAGE_SIZE	= 512
	};

	static CMineralVeinMgr&		GetInstance();

	bool	Initialize();
	void	Destroy();

	bool	LoadMineralVeinsFromFiles(const char* szTextFile, const char* szImageFile = 0);
	bool	SaveMineralVeinsToBinary(const char* szBinaryFile, const char* szTrashFile = 0);
	bool	LoadMineralVeinsFromBinary(const char* szBinaryFile);

	MineralVeinInfo*	GetMineralVein(unsigned long dwVeinColor) const;
	MineralVeinInfo*	GetMineralVein(float fX, float fZ) const;
	unsigned long		GetVeinColor(float fX, float fZ) const;
	const char*			GetVeinName(float fX, float fZ) const;
	int					GetVeinNum() const;

	// 지력 프로세스 함수
	void	InitializeMiningCampNum();						// 지력당 채굴기 수를 0 으로 리셋
    void	CalculateMiningCampNum(float fX, float fZ);		// 채굴기 위치에 해당하는 지력의 요새 수를 증가해주는 함수
	int		GetMiningCampNum(unsigned long dwVeinColor);	// 현재 지역의 채굴중인 채굴기 수를 리턴
	void	CalculateFertility();							// 지력 업데이트
	void	SerializeOutFertility(unsigned long* lpData_Out, unsigned short& wSize);


	// DB 에서 얻어온 현재 지력값을 설정하는 함수
	bool	SetNowFertility(unsigned short wNum, unsigned long* szDataFromDB);
	bool	SetNowFertility(unsigned long dwVeinColor, unsigned long dwFertility);

private:

	typedef std::map<unsigned long, int>	VeinToCampNumMap;	// < VeinColor, MiningCampNum >

	CMineralVeinMgr();
	~CMineralVeinMgr();

	MineralVeinMap		m_mapMineralVein;
	VeinToCampNumMap	m_mapCampNum;
	unsigned long		m_dwColorTable[ COLOR_IMAGE_SIZE ][ COLOR_IMAGE_SIZE ];
};

#endif	// __MINERAL_VEIN_MANAGER_H__