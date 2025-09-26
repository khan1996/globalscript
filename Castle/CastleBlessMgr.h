#ifndef _CASTLE_BLESS_MANAGER_H_
#define _CASTLE_BLESS_MANAGER_H_

#pragma once

#define g_CastleBlessMgr	Castle::CCastleBlessMgr::GetInstance()

#include <map>

namespace Castle
{
	class CCastleBlessMgr
	{
	private:

		enum Const
		{
			MAX_BLESS_INFO_NUM	= 6
		};

		struct sCastleBlessInfo
		{
			unsigned short	m_wGainTaxCount;	// 세금 획득 횟수(임시 -> 누적) = 총 공성 횟수 / Castle::TEMP_TAX_GAIN_COUNT
			unsigned char	m_cUpgradeStep;		// 필요 업그레이드
			unsigned char	m_cBonusPercent;	// 증가 수확량
			float			m_fArea;			// 영역(반경 m 단위)
			unsigned char	m_cTitle;			// 칭호

			sCastleBlessInfo(unsigned short wGainTaxCount, unsigned char cUpgradeStep, unsigned char cBonusPercent,
							 float fArea, unsigned char cTitle)
			: m_wGainTaxCount(wGainTaxCount), m_cUpgradeStep(cUpgradeStep), m_cBonusPercent(cBonusPercent),
			  m_fArea(fArea), m_cTitle(cTitle)
			{
			}
		};

	public:
		~CCastleBlessMgr();
		static CCastleBlessMgr& GetInstance();

		unsigned char	GetTitle(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const;
		unsigned char	GetBonusPercent(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const;
		float			GetBlessArea(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const;
				
	private:
		CCastleBlessMgr();

		int				FindProperIndex(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const;

		bool			Initialize();
		void			Destroy();

		static sCastleBlessInfo		m_CastleBlessInfo[ MAX_BLESS_INFO_NUM ] ;
	};
}

#endif	// _CASTLE_BLESS_MANAGER_H_