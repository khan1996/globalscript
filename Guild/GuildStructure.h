#ifndef _GUILD_STRUCTURE_H_
#define _GUILD_STRUCTURE_H_

namespace Guild
{
	struct MemberListInfo
	{
		unsigned char	m_cRank;		// 서열 (한 길드내에선 유니크한 숫자. 길드마스터를 0으로 하여 순차적으로 구성)
		unsigned char	m_cTitle;		// 직위 (Guild::Title의 상수를 참고)
		unsigned char	m_cLevel;		// 레벨
		unsigned char	m_cClass;		// 클래스

		MemberListInfo()
			:	m_cRank(0), m_cTitle(0), m_cLevel(0), m_cClass(0)
		{ }

		MemberListInfo(unsigned char cRank, unsigned char cTitle, unsigned char cLevel, unsigned char cClass)
			:	m_cRank(cRank), m_cTitle(cTitle), m_cLevel(cLevel), m_cClass(cClass)
		{ }
	};

	struct MemberDetailInfo
	{
		unsigned long	m_dwFame;
		unsigned long	m_dwGold;

		MemberDetailInfo()
			:	m_dwFame(0), m_dwGold(0)
		{ }

		MemberDetailInfo(unsigned long dwFame, unsigned long dwGold)
			:	m_dwFame(dwFame), m_dwGold(dwGold)
		{ }
	};

	struct MemberInfo
	{
		// 변하지 않는 정보
		unsigned long		m_dwCID;
		char				m_strName[MAX_MEMBER_NAME_LEN];

		// 따로 처리되는 정보
		unsigned long		m_dwServerID;
		TIME				m_LeaveGuildTime;
		unsigned char		m_cUpdateCount;

		// 가끔 변하는 정보 (변경될 때마다 업데이트)
		MemberListInfo		m_MemberListInfo;

		// 자주 변하는 정보 (일정 횟수 이상 변경되면 업데이트)
		MemberDetailInfo	m_MemberDetailInfo;

		MemberInfo()
			:	m_dwCID(0), m_dwServerID(0), m_cUpdateCount(0), m_MemberListInfo(), m_MemberDetailInfo()
		{
			std::fill_n(m_strName, int(MAX_MEMBER_NAME_LEN), 0);
		}
	};
};

#endif