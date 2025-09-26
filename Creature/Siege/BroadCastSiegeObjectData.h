#ifndef __BROADCAST_SIEGEOBJECT_DATA_H__
#define __BROADCAST_SIEGEOBJECT_DATA_H__

#include <winsock2.h>
#include <windows.h>

#include <Network/Packet/PacketStruct/CharMovePacket.h>
#include <Creature/Siege/SiegeConstants.h>

//! 본 파일은 서버/클라이언트가 전부 사용하는 파일이다.
//! 공성 오브젝트 데이터 중 클라이언트가 꼭 알아야 할 내용들을 담고 있다.

namespace BroadCastSiege
{
	inline void COPY_AND_ADVANCE_SRC( void* dst, const char* src, size_t size )
	{
		memcpy( dst, src, size ); src += size;
	}

	inline void COPY_AND_ADVANCE_DST( char* dst, const void* src, size_t size )
	{
		memcpy( dst, src, size ); dst += size;
	}


	#define FLOAT_EPSILON	0.001

	// Update Data Flag
	enum SiegeDeltaInfo
	{
		DELTA_OWNERSHIP		= 1 << 0,	// dwCampOrCastleID, OwnerID, GID, cNation
		DELTA_STATE			= 1 << 1,	// wObjectType, cState, cSubState, cUpgradeStep, cUpgradeType
		DELTA_HP			= 1 << 2,	// HP 정보
		DELTA_POS			= 1 << 3,	// fDir, Pos
		DELTA_MATERIAL		= 1 << 4,	// cMaterial
		DELTA_RIDER			= 1 << 5,	// dwRiderCID[]
	};

	enum Const
	{
		// UpdateFlag ( 4 byte )
		// OwnerShipInfo ( 13 byte )
		// StateInfo( 6 byte )
		// HPInfo ( 8 byte )
		// PosInfo ( 12 byte )
		// MaterialInfo ( 1 byte )
		// RiderInfo ( 40 byte )
		// Total : 84 byte
		MAX_SIEGEOBJECT_DATA_SIZE		= 120,
		MAX_SIEGEBROADCAST_BUFFER_SIZE	= 160	// sizeof(PktNewSiegeBroadCast) + MAX_SIEGEOBJECT_DATA_SIZE
	};

	enum BroadCastType
	{
		BROADCAST_ALL_DATA		= 1,	// 모든 데이터 전송
		BROADCAST_DELTA_DATA	= 2,	// 변경된 데이터 전송
		BROADCAST_DELETE_DATA	= 3		// 해당 크리쳐 삭제 데이터 전송
	};

    // 소유 정보 (성ID, 길드 요새ID, 소유자CID), 길드ID
	struct sOwnerShipInfo
	{
		unsigned long	m_dwCampOrCastleID;
		unsigned long	m_dwOwnerCID;
		unsigned long	m_dwGID;
		unsigned char	m_cNation;
	};

	// 공성 오브젝트 상태 정보
	struct sStateInfo
	{
		unsigned short	m_wObjectType;
		unsigned char	m_cState;
		unsigned char	m_cSubState;
		unsigned char	m_cUpgradeStep;
		unsigned char	m_cUpgradeType;
	};

	// HP 정보
	struct sHPInfo
	{
		unsigned long	m_dwNowHP;
		unsigned long	m_dwMaxHP;
	};

	// 위치 정보
	struct sPosInfo
	{
		float			m_fDefaultDir;
		CNetworkPos		m_NetworkPos;
	};

	// 자재 정보
	struct sMaterialInfo
	{
		unsigned char	m_cMaterial;
	};

	// 탑승자 정보
	struct sRiderInfo
	{
		unsigned long	m_dwRiderID[Siege::AIRSHIP_RIDER_NUM];
	};


	// =============================================================================================
	// SiegeObject Data
	class CSiegeObjectData
	{
	public:
		CSiegeObjectData();
		~CSiegeObjectData();

		// ===================================================================================
		// Get 함수
		const unsigned long		GetCID() const				{ return m_dwCID;	}

		const sOwnerShipInfo&	GetOwnerShipInfo() const	{ return m_OwershipInfo;					}
		const unsigned long		GetCastleID() const			{ return m_OwershipInfo.m_dwCampOrCastleID;	}
		const unsigned long		GetCampID() const			{ return m_OwershipInfo.m_dwCampOrCastleID;	}
		const unsigned long		GetOwnerCID() const			{ return m_OwershipInfo.m_dwOwnerCID;		}
		const unsigned long		GetGID() const				{ return m_OwershipInfo.m_dwGID;			}
		const unsigned char		GetNation() const			{ return m_OwershipInfo.m_cNation;			}

		const sStateInfo&		GetStateInfo() const	{ return m_StateInfo;					}
		const unsigned short	GetObjectType() const	{ return m_StateInfo.m_wObjectType;		}
		const unsigned char		GetState() const		{ return m_StateInfo.m_cState;			}
		const unsigned char		GetSubState() const		{ return m_StateInfo.m_cSubState;		}
		const unsigned char		GetUpgradeType() const	{ return m_StateInfo.m_cUpgradeType;	}
		const unsigned char		GetUpgradeStep() const	{ return m_StateInfo.m_cUpgradeStep;	}

		const sHPInfo&			GetHPInfo() const		{ return m_HPInfo;						}
		const unsigned long		GetNowHP() const		{ return m_HPInfo.m_dwNowHP;			}
		const unsigned long		GetMaxHP() const		{ return m_HPInfo.m_dwMaxHP;			}

		const sPosInfo&			GetPosInfo() const		{ return m_PosInfo;						}
		const float				GetDefaultDir() const	{ return m_PosInfo.m_fDefaultDir;		}
		const CNetworkPos&		GetNetworkPos() const	{ return m_PosInfo.m_NetworkPos;		}

		const sMaterialInfo&	GetMaterialInfo() const	{ return m_MaterialInfo;				}
		const unsigned char		GetMaterialNum() const	{ return m_MaterialInfo.m_cMaterial;	}

		const sRiderInfo&		GetRiderInfo() const	{ return m_RiderInfo;					}
		const void				GetRiders( unsigned long* pPassenger ) const;
		const bool				IsSameRiders( unsigned long * pPassenger ) const;

		// ===================================================================================
		// Set 함수
		void	SetOwnerShipInfo( const sOwnerShipInfo& info )	{ m_OwershipInfo = info;							}
		void	SetCastleID( unsigned long dwCastleID )			{ m_OwershipInfo.m_dwCampOrCastleID = dwCastleID;	}
		void	SetCampID( unsigned long dwCampID )				{ m_OwershipInfo.m_dwCampOrCastleID = dwCampID;		}
		void	SetOwnerCID( unsigned long dwOwnerCID )			{ m_OwershipInfo.m_dwOwnerCID = dwOwnerCID;			}
		void	SetGID( unsigned long dwGID )					{ m_OwershipInfo.m_dwGID = dwGID;					}
		void	SetNation( unsigned char cNation )				{ m_OwershipInfo.m_cNation = cNation;				}

		void	SetStateInfo( const sStateInfo& info )			{ m_StateInfo = info;						}
		void	SetObjectType( unsigned short wType )			{ m_StateInfo.m_wObjectType = wType;		}
		void	SetState( unsigned char cState )				{ m_StateInfo.m_cState = cState;			}
		void	SetSubState( unsigned char cSubState )			{ m_StateInfo.m_cSubState = cSubState;		}
		void	SetUpgradeType( unsigned char cType )			{ m_StateInfo.m_cUpgradeType = cType;		}
		void	SetUpgradeStep( unsigned char cStep )			{ m_StateInfo.m_cUpgradeStep = cStep;		}

		void	SetHPInfo( const sHPInfo& info )				{ m_HPInfo = info;							}
		void	SetNowHP( unsigned long dwNowHP )				{ m_HPInfo.m_dwNowHP = dwNowHP;				}
		void	SetMaxHP( unsigned long dwMaxHP )				{ m_HPInfo.m_dwMaxHP = dwMaxHP;				}

		void	SetPosInfo( const sPosInfo& info )				{ m_PosInfo = info;							}
		void	SetDefaultDir( float fDir )						{ m_PosInfo.m_fDefaultDir = fDir;			}
		void	SetNetworkPos( const CNetworkPos& pos)			{ m_PosInfo.m_NetworkPos = pos;				}

		void	SetMaterialInfo( const sMaterialInfo& info )	{ m_MaterialInfo = info;					}
		void	SetMaterialNum( unsigned char cMaterial )		{ m_MaterialInfo.m_cMaterial = cMaterial;	}

		void	SetRiderInfo( const sRiderInfo& info )			{ m_RiderInfo = info;						}
		void	SetRiders( unsigned long* pPassenger );
		void	ClearRiders();

		// ===================================================================================
		// Data 처리 함수
		void		ClearData();

		//! 버퍼와 버퍼 길이를 인자로 받아서 데이터를 업데이트한다.
		//! 리턴시에 버퍼 길이에, 사용한 버퍼 길이를 넣어 준다.
		//! 리턴값은 성공/실패 여부를 리턴한다.
		bool		UpdateData( unsigned long dwCID, const char* szData, int& iBufferSize_InOut );
		static int	EstimateBufferSize( unsigned long dwUpdateFlag );

		// ===================================================================================
		// Data Flag 관련 함수
		void	SetUpdateDataFlag( unsigned long dwFlag )		{ m_dwUpdateDataFlag |= dwFlag;					}
		bool	HasUpdateDataFlag( unsigned long dwFlag )		{ return 0 != (m_dwUpdateDataFlag & dwFlag);	}
		void	ResetUpdateDataFlag( unsigned long dwFlag )		{ m_dwUpdateDataFlag &= ~dwFlag;				}
		void	ClearUpdateDataFlag()							{ m_dwUpdateDataFlag = 0;						} 

		unsigned long GetDataFlag() const						{ return m_dwUpdateDataFlag;					}

	private:

		unsigned long	m_dwCID;

		sOwnerShipInfo	m_OwershipInfo;
		sStateInfo		m_StateInfo;
		sHPInfo			m_HPInfo;
		sPosInfo		m_PosInfo;
		sMaterialInfo	m_MaterialInfo;
		sRiderInfo		m_RiderInfo;
		
		unsigned long   m_dwUpdateDataFlag;
	};

}

#endif	__BROADCAST_SIEGEOBJECT_DATA_H__