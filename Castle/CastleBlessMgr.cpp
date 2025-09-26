#include "stdafx.h"
#include "CastleBlessMgr.h"

#include <Castle/CastleConstants.h>
#include "GMMemory.h"

using namespace Castle;

CCastleBlessMgr::sCastleBlessInfo	CCastleBlessMgr::m_CastleBlessInfo[ MAX_BLESS_INFO_NUM ] =
{
	sCastleBlessInfo(0, 0,  5, 320.0f, Castle::TITLE_NEW),
	sCastleBlessInfo(1, 1, 10, 320.0f, Castle::TITLE_NEW),
	sCastleBlessInfo(2, 1, 20, 400.0f, Castle::TITLE_SETTLED),
	sCastleBlessInfo(3, 2, 30, 480.0f, Castle::TITLE_FLOURISHED),
	sCastleBlessInfo(4, 2, 40, 560.0f, Castle::TITLE_HONORABLE),
	sCastleBlessInfo(5, 3, 50, 640.0f, Castle::TITLE_GLORIOUS)
};

CCastleBlessMgr::CCastleBlessMgr()
{
	Initialize();
}

CCastleBlessMgr::~CCastleBlessMgr()
{
	Destroy();
}

CCastleBlessMgr& CCastleBlessMgr::GetInstance()
{
	static CCastleBlessMgr ms_this;
	return ms_this;
}

bool	CCastleBlessMgr::Initialize()
{
	return true;
}

void	CCastleBlessMgr::Destroy()
{
}

unsigned char	CCastleBlessMgr::GetTitle(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const
{
	int iIndex = FindProperIndex(wGainTaxCount, cUpgradeStep);
	return m_CastleBlessInfo[ iIndex ].m_cTitle;
}

unsigned char	CCastleBlessMgr::GetBonusPercent(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const
{
	int iIndex = FindProperIndex(wGainTaxCount, cUpgradeStep);
	return m_CastleBlessInfo[ iIndex ].m_cBonusPercent;
}

float	CCastleBlessMgr::GetBlessArea(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const
{
	int iIndex = FindProperIndex(wGainTaxCount, cUpgradeStep);
	return m_CastleBlessInfo[ iIndex ].m_fArea;
}

int	CCastleBlessMgr::FindProperIndex(unsigned short wGainTaxCount, unsigned char cUpgradeStep) const
{
	int iIndex = 0;
    for (; iIndex<MAX_BLESS_INFO_NUM; ++iIndex)
	{
		if (m_CastleBlessInfo[ iIndex ].m_wGainTaxCount > wGainTaxCount ||
			m_CastleBlessInfo[ iIndex ].m_cUpgradeStep > cUpgradeStep)
		{
			return (iIndex - 1);
		}
	}

	return (iIndex - 1);
}
