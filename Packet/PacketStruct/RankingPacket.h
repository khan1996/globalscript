
#ifndef _RANKING_PACKET_H_
#define _RANKING_PACKET_H_

#include <algorithm>
#include <Network/Packet/PacketBase.h>


#pragma pack(1)

// ---------------------------------------------------------------------------
// ·©Å· °ü·Ã

struct RankingNode
{
	enum { MAX_NAME = 16 };

	char			m_szCharName[MAX_NAME];
	unsigned long	m_dwPoint;
	unsigned char	m_cLevel;
	unsigned char	m_cClass;

	RankingNode()
	:	m_dwPoint(0), m_cLevel(0), m_cClass(0)
	{
		std::fill_n(m_szCharName, int(MAX_NAME), 0);
	}

	RankingNode(const char* szCharName, unsigned long dwPoint, unsigned char cLevel, unsigned char cClass)
	:	m_dwPoint(dwPoint), m_cLevel(cLevel), m_cClass(cClass)
	{
		strncpy(m_szCharName, szCharName, MAX_NAME);
        m_szCharName[MAX_NAME - 1] = 0;
	}

	bool operator < (const RankingNode& rhs)
	{ 
		return (m_dwPoint < rhs.m_dwPoint); 
	}

	bool operator > (const RankingNode& rhs)
	{ 
		return (m_dwPoint > rhs.m_dwPoint); 
	}

	struct ComparePoint
	{
		int operator() (const RankingNode& node1, const RankingNode& node2) 
		{ 
			return (node1.m_dwPoint > node2.m_dwPoint); 
		}
	};
};

typedef struct PktRankingInfo* LPPktRankingInfo;
struct PktRankingInfo : public PktBase
{
	enum Const
	{ 
		MAX_NUM_PER_PAGE	= 18 
	};

	enum PktRankingInfoErr
	{
		FAIL_INVALID_PAGE	= 2
	};

	unsigned long	m_dwCharID;

	unsigned char	m_cClass;
	unsigned char	m_cPage;

	unsigned short	m_wNum;
};

#pragma pack()


#endif