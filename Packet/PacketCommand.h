////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 패킷 커맨드
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _PACKET_COMMAND_H_
#define _PACKET_COMMAND_H_

#include <Network/Packet/PacketBase.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 커맨드
//
////////////////////////////////////////////////////////////////////////////////////////////////////
const PktBase::CMDType	CmdNull						= 0x00;	

const PktBase::CMDType	CmdAuthAccount				= 0x01;			// 계정 인증

const PktBase::CMDType	CmdServerGroup				= 0x02;			// 서버군 선택

const PktBase::CMDType	CmdUserLogin				= 0x03;			// 유저 로그인
const PktBase::CMDType	CmdUserKill					= 0x04;			// 유저 제거

const PktBase::CMDType	CmdCharSelect				= 0x05;			// 캐릭터 선택
const PktBase::CMDType	CmdCharCreate				= 0x06;			// 캐릭터 생성
const PktBase::CMDType	CmdCharDelete				= 0x07;			// 캐릭터 삭제

const PktBase::CMDType	CmdCharLogin				= 0x08;			// 캐릭터 로그인
const PktBase::CMDType	CmdCharSuicide				= 0x09;			// 캐릭터 자살
const PktBase::CMDType	CmdCharRespawn				= 0x0A;			// 캐릭터 부활
const PktBase::CMDType	CmdCharMove					= 0x0B;			// 캐릭터 이동 (P2P)
const PktBase::CMDType	CmdCharMoveUpdate			= 0x0C;			// 캐릭터 이동 업데이트
const PktBase::CMDType	CmdCharChat					= 0x0D;			// 캐릭터 채팅
const PktBase::CMDType	CmdCharAttack				= 0x0E;			// 캐릭터 공격
const PktBase::CMDType	CmdCharCastObject			= 0x0F;			// 캐릭터 오브젝트 던지기
const PktBase::CMDType	CmdCharUseSkill				= 0x10;			// 캐릭터 스킬 사용
const PktBase::CMDType	CmdCharTakeItem				= 0x11;			// 캐릭터 아이템 집기
const PktBase::CMDType	CmdCharSwapItem				= 0x12;			// 캐릭터 아이템 스왑
const PktBase::CMDType	CmdCharRepairItem			= 0x13;			// 캐릭터 아이템 수리
const PktBase::CMDType	CmdCharUseItem				= 0x14;			// 캐릭터 아이템 사용
const PktBase::CMDType	CmdCharTradeItem			= 0x15;			// 캐릭터 아이템 거래
const PktBase::CMDType	CmdCharSkillLock			= 0x16;			// 캐릭터 스킬 락
const PktBase::CMDType	CmdCharSkillCreate			= 0x17;			// 캐릭터 스킬 생성
const PktBase::CMDType	CmdCharSkillErase			= 0x18;			// 캐릭터 스킬 지우기
const PktBase::CMDType	CmdCharClassUpgrade			= 0x19;			// 캐릭터 클래스 업그레이드
const PktBase::CMDType	CmdCharShapeInfo			= 0x1A;			// 캐릭터 정보 (P2P)
const PktBase::CMDType	CmdCharIncreasePoint		= 0x1B;			// 캐릭터 능력 포인트 증가
const PktBase::CMDType	CmdCharBindPosition			= 0x1C;			// 캐릭터 바인드 포지션
const PktBase::CMDType	CmdCharRequireInfo			= 0x1D;			// 캐릭터 해당 정보 요청
const PktBase::CMDType	CmdCharUpdateAddress		= 0x1E;			// 캐릭터 UDP 주소 업데이트
const PktBase::CMDType	CmdCharPartyCmd				= 0x1F;			// 캐릭터 파티 명령
const PktBase::CMDType	CmdCharPartyMemInfo			= 0x20;			// 캐릭터 파티 맴버 정보 전달
const PktBase::CMDType	CmdCharExchangeCmd			= 0x21;			// 캐릭터 아이템 교환 명령
const PktBase::CMDType	CmdSysConnectAgent			= 0x22;			// 시스템 중계 접속 (UDP 중계)		# 사용 안함
const PktBase::CMDType	CmdSysPacketTransmit		= 0x23;			// 시스템 패킷 전달 (UDP 중계)		# 사용 안함
const PktBase::CMDType	CmdCharLogout				= 0x24;			// 캐릭터 로그 아웃
const PktBase::CMDType	CmdDBGetData				= 0x25;			// DB 데이터 얻기
const PktBase::CMDType	CmdDBUpdateData				= 0x26;			// 업데이트 DB 데이터
const PktBase::CMDType	CmdAgentParty				= 0x27;			// DB 에이전트 파티
const PktBase::CMDType	CmdSysServerLogin			= 0x28;			// 시스템 서버 로그인

const PktBase::CMDType	CmdServerZone				= 0x29;			// 서버존 선택

const PktBase::CMDType	CmdGameCellInfo				= 0x2A;			// 게임 셀 정보 (오브젝트 정보)	
const PktBase::CMDType	CmdCharInfo					= 0x2B;			// 캐릭터 정보 (서버군 통합에 사용함)
const PktBase::CMDType	CmdCharAddressInfo			= 0x2C;			// 캐릭터 UDP 주소 정보
const PktBase::CMDType	CmdCharCellLogin			= 0x2D;			// 캐릭터 셀 로그인
const PktBase::CMDType	CmdCharCellLogout			= 0x2E;			// 캐릭터 셀 로그아웃
const PktBase::CMDType	CmdMonMove					= 0x2F;			// 몬스터 이동
const PktBase::CMDType	CmdCharAttackInfo			= 0x30;			// 캐릭터 공격 정보 (P2P)
const PktBase::CMDType	CmdCharAttacked				= 0x31;			// 캐릭터 공격 얻음
const PktBase::CMDType	CmdCharAward				= 0x32;			// 캐릭터 어워드 
const PktBase::CMDType	CmdCharItemInfo				= 0x33;			// 캐릭터 아이템 정보
const PktBase::CMDType	CmdCharPickUp				= 0x34;			// 캐릭터 집기
const PktBase::CMDType	CmdCharPullDown				= 0x35;			// 캐릭터 떨구기
const PktBase::CMDType	CmdCharPickUpInfo			= 0x36;			// 캐릭터 집기 정보
const PktBase::CMDType	CmdCharPullDownInfo			= 0x37;			// 캐릭터 떨구기 정보
const PktBase::CMDType	CmdCharCastObjectInfo		= 0x38;			// 캐릭터 던지기 오브젝트 정보
const PktBase::CMDType	CmdCharInstallSocket		= 0x39;			// 캐릭터 아이템 소켓 설치
const PktBase::CMDType	CmdCharLevelUp				= 0x3A;			// 캐릭터 레벨 업
const PktBase::CMDType	CmdCharPartyInfo			= 0x3B;			// 캐릭터 파티 정보
const PktBase::CMDType	CmdCharUpgradeItem			= 0x3C;			// 캐릭터 아이템 업그레이드
const PktBase::CMDType	CmdCharHPRegen				= 0x3D;			// 캐릭터 HP 리젠
const PktBase::CMDType	CmdCharLevelUpInfo			= 0x3E;			// 캐릭터 레벨업 정보
const PktBase::CMDType	CmdCharSplitItem			= 0x3F;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)
const PktBase::CMDType	CmdUpdateUIDTable			= 0x40;			// UID 테이블 업데이트
const PktBase::CMDType	CmdCharQuickSlotMove		= 0x41;			// 캐릭터 퀵슬롯 이동
const PktBase::CMDType	CmdCharSwitchEQ				= 0x42;			// 장비 바꾸기
const PktBase::CMDType	CmdSysMngerRegistry			= 0x43;			// 서버 관리자 등록
const PktBase::CMDType	CmdSysMngerRequest			= 0x44;			// 서버 관리자의 요청
const PktBase::CMDType	CmdSysMngerResponse			= 0x45;			// 서버 관리자로 응답
const PktBase::CMDType	CmdCharTakeItems			= 0x46;			// 캐릭터 아이템 집기 (복수)
const PktBase::CMDType	CmdCharTakeGold				= 0x47;			// 캐릭터 돈 집기
const PktBase::CMDType	CmdCharExchangeItem			= 0x48;			// 캐릭터 아이템 교환
const PktBase::CMDType	CmdCellBroadCasting			= 0x49;			// 셀 브로드 캐스팅
const PktBase::CMDType	CmdSysPatchAddress			= 0x4A;			// 패치 주소 
const PktBase::CMDType	CmdCharPartyCmdInfo			= 0x4B;			// 파티 명령 정보
const PktBase::CMDType	CmdServerLog				= 0x4C;			// 로그 정보 (???)
const PktBase::CMDType	CmdCharWhisper				= 0x4D;			// 캐릭터 귓속말
const PktBase::CMDType	CmdSysServerVerUpdate		= 0x4E;			// 서버 버젼 업데이트
const PktBase::CMDType	CmdSysMng					= 0x4F;			// 서버 관리 서버, 클라이언트가 사용하는 패킷 (임시) (???)
const PktBase::CMDType	CmdSysChannelUpdate			= 0x50;			// 서버 채널 업데이트
const PktBase::CMDType	CmdCharPartyFind			= 0x51;			// 파티 찾기
const PktBase::CMDType	CmdCharPartyMemData			= 0x52;			// 파티 멤버 데이터
const PktBase::CMDType	CmdCharControlOption		= 0x53;			// 캐릭터 옵션 조정
const PktBase::CMDType	CmdCharDuelCmd				= 0x54;			// 캐릭터 듀얼 명령
const PktBase::CMDType	CmdCharFameInfo				= 0x55;			// 캐릭터 명성 정보

const PktBase::CMDType	CmdLoginServerList			= 0x56;			// 서버 리스트 #!! 번호 수정 불가 !!#

const PktBase::CMDType	CmdCharSpellInfo			= 0x57;			// 캐릭터 챈트&인챈트 정보
const PktBase::CMDType	CmdCharSkillUnLock			= 0x58;			// 캐릭터 스킬 락 해제
const PktBase::CMDType	CmdSysPing					= 0x59;			// 서버 핑 패킷
const PktBase::CMDType	CmdCharMoveZone				= 0x5A;			// 존 이동

const PktBase::CMDType	CmdAgentZone				= 0x5B;			// 존 관리

const PktBase::CMDType  CmdDeposit              	= 0x5C;         // 창고 처리 패킷 (Client <--> GameServer)
const PktBase::CMDType  CmdDepositUpdate        	= 0x5D;         // 창고 업데이트 패킷 (GameServer <--> DBAgent)

const PktBase::CMDType	CmdCharStallOpen			= 0x5E;			// 캐릭터 노점상 개설
const PktBase::CMDType	CmdCharStallRegisterItem	= 0x5F;			// 캐릭터 노점상 아이템 등록
const PktBase::CMDType	CmdCharStallEnter			= 0x60;			// 캐릭터 노점상 입장
const PktBase::CMDType	CmdCharStallItemInfo		= 0x61;			// 캐릭터 노점상 아이템 정보

const PktBase::CMDType	CmdCharAdminCmd				= 0x62;			// 캐릭터 어드민

const PktBase::CMDType	CmdCharTeamBattleInfo		= 0x63;			// 팀배틀 정보

const PktBase::CMDType  CmdFriendAddRequest         = 0x64;         // 친구 추가
const PktBase::CMDType  CmdFriendRemoveRequest      = 0x65;         // 친구 삭제
const PktBase::CMDType  CmdFriendEtcRequest         = 0x66;         // 친구 기타
const PktBase::CMDType  CmdFriendAck                = 0x67;         // 친구 Ack
const PktBase::CMDType  CmdFriendDB                 = 0x68;         // 친구 데이터 (및 친구 리스트 정보)

const PktBase::CMDType  CmdEliteBonus				= 0x69;         // 엘리트 보너스

const PktBase::CMDType	CmdCharStartQuest			= 0x6A;			// 퀘스트 시작
const PktBase::CMDType	CmdCharOperateTrigger		= 0x6B;			// 트리거 발동
const PktBase::CMDType	CmdQuestDB					= 0x6C;			// 퀘스트 정보 (DB에 세이브/로드, 캐릭터 로긴시)
const PktBase::CMDType	CmdCharEndQuest				= 0x6D;			// 퀘스트 종료

const PktBase::CMDType	CmdCharDisappearItem		= 0x6E;			// 캐릭터 아이템 사라지기

const PktBase::CMDType	CmdCharAuthorizePanel		= 0x6F;			// 캐릭터 인증 판넬
const PktBase::CMDType	CmdCharPeaceMode			= 0x70;			// 캐릭터 반전 모드
const PktBase::CMDType	CmdConfigInfoDB				= 0x71;			// 설정 정보
const PktBase::CMDType	CmdCharAutoRouting			= 0x72;			// 캐릭터 오토 루팅
const PktBase::CMDType	CmdRankingInfo				= 0x73;			// 랭킹 정보
const PktBase::CMDType	CmdCharStateRedistribution	= 0x74;			// 스탯 재분배 (클래스 처음 상태로)
const PktBase::CMDType  CmdBillingTimeoutNotify     = 0x75;         // 빌링 타임아웃 Notify정보

const PktBase::CMDType  CmdAdminToolGetData         = 0x76;         // 운영툴에서 DBAgent의 데이터 얻어 오기.
const PktBase::CMDType  CmdAdminToolSetData         = 0x77;         // 운영툴에서 DBAgent로 데이터 쓰기.

const PktBase::CMDType	CmdEventDropItem			= 0x78;			// 아이템 떨구기 이벤트
const PktBase::CMDType	CmdCharCancelQuest			= 0x79;			// 퀘스트 취소
const PktBase::CMDType	CmdBillingTimeCheckNotify	= 0x7A;			// 빌링 타임 체크
const PktBase::CMDType	CmdCharLotteryResult		= 0x7B;			// 복권 결과
const PktBase::CMDType	CmdCharSummonCmd			= 0x7C;			// 소환수 명령

const PktBase::CMDType  CmdChatClientLogin          = 0x7D;         // 클라이언트에서 채팅서버에 붙을 때 정보 줌
const PktBase::CMDType  CmdChatLogin                = 0x7E;         // 중계서버가 채팅서버로 클라이언트 로그인 줌.
const PktBase::CMDType  CmdChatLogout               = 0x7F;         // 중계서버가 채팅서버로 클라이언트 로그아웃 줌.
const PktBase::CMDType  CmdChatInfoChanged          = 0x80;         // 중계서버가 채팅서버로 클라이언트 정보 변경 줌.

const PktBase::CMDType	CmdCharSummon				= 0x81;			// 소환

const PktBase::CMDType	CmdJapanAuthAccount	        = 0x82;			// 일본쪽 계정 인증 (ID, Password대신, 인증된 코드가 온다.)

const PktBase::CMDType	CmdCharBattleGroundRespawn	= 0x83;			// 리스폰 (배틀그라운드 대기중)
const PktBase::CMDType	CmdCharRespawnWaitQueue		= 0x84;			// 리스폰 큐 대기인 정보 (배틀그라운드용)
const PktBase::CMDType	CmdStatueInfo				= 0x85;			// 석상 정보
const PktBase::CMDType	CmdCameraScript				= 0x86;			// 카메라 스크립트
const PktBase::CMDType	CmdCharEquipDurability		= 0x87;			// 캐릭터 장비 내구도

const PktBase::CMDType	CmdCreateGuild				= 0x88;			// 길드 생성
const PktBase::CMDType	CmdGuildCmd					= 0x89;			// 길드 멤버 관련 명령
const PktBase::CMDType	CmdGuildMark				= 0x8A;			// 길드 마크 변경
const PktBase::CMDType	CmdGuildLevel				= 0x8B;			// 길드 레벨 변경
const PktBase::CMDType	CmdGuildRelation			= 0x8C;			// 길드 관계 변경
const PktBase::CMDType	CmdGuildList				= 0x8D;			// 길드 리스트
const PktBase::CMDType	CmdGuildDB					= 0x8E;			// 길드 데이터
const PktBase::CMDType	CmdGuildRight				= 0x8F;			// 길드 권한 설정
const PktBase::CMDType	CmdGuildMemberList			= 0x90;			// 길드 멤버 리스트
const PktBase::CMDType	CmdMyGuildInfo				= 0x91;			// 자기 길드 정보
const PktBase::CMDType	CmdGuildSafe				= 0x92;			// 길드 금고
const PktBase::CMDType	CmdGuildMemberInfoUpdate	= 0x93;			// 길드 멤버 정보 업데이트

const PktBase::CMDType	CmdCharStatusRetrain		= 0x94;			// 스테이터스 재훈련 (일정량 다시 돌려받음)
const PktBase::CMDType	CmdSysServerLogout			= 0x95;			// 서버 로그아웃

const PktBase::CMDType	CmdCharUseCashItem			= 0x96;			// 캐릭터 캐쉬 아이템 사용
const PktBase::CMDType	CmdExtraEvent				= 0x97;			// 엑스트라 이벤트 관련

//const PktBase::CMDType	CmdCharPartyAddress			= 0x96;			// 파티원 UDP 주소 정보
//const PktBase::CMDType	CmdCharPartyMemAddress		= 0x97;			// 파티원 UDP 주소 정보 (P2P)

const PktBase::CMDType	CmdBGServerMapList			= 0x98;			// 배틀 그라운드 서버 맵(방) 정보 리스트 요청
const PktBase::CMDType	CmdBGServerResultList		= 0x99;			// 배틀 그라운드 서버 맵(방) 결과 리스트 요청
const PktBase::CMDType	CmdBGServerMoveZone			= 0x9A;			// 배틀 그라운드 서버 맵(방) 이동 (존 이동)
const PktBase::CMDType	CmdBGServerMileageChange	= 0x9B;			// 배틀 그라운드 서버 환전소 명령
const PktBase::CMDType	CmdBGServerCharSlot			= 0x9C;			// 배틀 그라운드 서버 정섭 캐릭터 정보 요청

const PktBase::CMDType  CmdHanBTNWarning            = 0x9D;         // 한게임 통합빌링 접속 끊기전 경고 메시지
const PktBase::CMDType  CmdHanBTNUserKill           = 0x9E;         // 한게임 통합빌링 접속 끊기 메시지

const PktBase::CMDType  CmdCharRepairAllItem        = 0x9F;         // 장비 아이템 모두 수리
const PktBase::CMDType	CmdCSAuth					= 0xA0;			// 인증 코드 (게임 가드)
const PktBase::CMDType	CmdCharItemChemical			= 0xA1;			// 아이템 합성
const PktBase::CMDType  CmdItemQtyCheck             = 0xA2;         // 아이템 수량 제어
const PktBase::CMDType	CmdGuildInclination			= 0xA3;			// 길드 성향
const PktBase::CMDType	CmdGuildMemberFameUpdate	= 0xA4;			// 길드 멤버 명성 업데이트

const PktBase::CMDType	CmdCastleInfo				= 0xA5;			// 성 정보 보내기			(DBAgentServer -> GameServer)
const PktBase::CMDType	CmdCampInfo					= 0xA6;			// 길드 요새 정보 보내기	(DBAgentServer -> GameServer)
const PktBase::CMDType	CmdCreateCastle				= 0xA7;			// 성 생성					(GameServer -> Client)
const PktBase::CMDType	CmdCreateCamp				= 0xA8;			// 길드 요새 생성			(Client <- GameServer -> DBAgentServer)
const PktBase::CMDType	CmdCreateSiegeArms			= 0xA9;			// 공성 병기 생성			(Client <- GameServer -> DBAgentServer)
const PktBase::CMDType	CmdCastleCmd				= 0xAA;			// 성 관련 명령 패킷		(Client <-> GameServer <-> DBAgentServer)
const PktBase::CMDType	CmdCampCmd					= 0xAB;			// 길드 요새 관련 명령 패킷	(Client <-> GameServer <-> DBAgentServer)
const PktBase::CMDType	CmdSiegeArmsCmd				= 0xAC;			// 공성 병기 관련 명령 패킷	(Client <-> GameServer <-> DBAgentServer)
const PktBase::CMDType	CmdCastleRight				= 0xAD;			// 성 관리 권한 패킷		(Client <-> GameServer <-> DBAgentServer)
const PktBase::CMDType	CmdCampRight				= 0xAE;			// 길드 요새 관리 권한 패킷
const PktBase::CMDType	CmdSiegeBroadCast			= 0xAF;			// 공성 오브젝트 브로드 캐스트
const PktBase::CMDType	CmdGameTimeInfo				= 0xB0;			// 게임 시간 정보
const PktBase::CMDType	CmdStealthInfo				= 0xB1;			// 스텔스 사용가능 정보
//const PktBase::CMDType	CmdCastleUpdate				= 0xB2;			// 성 정보 업데이트			(DBAgentServer -> GameServer -> Client)

const PktBase::CMDType  CmdCellBroadCast2nd         = 0xB3;         // 셀 브로드캐스트 수정된 패킷

const PktBase::CMDType	CmdCharRespawnInfo			= 0xB4;			// 리스폰 정보 (리스폰 지역 리스트 정보)
const PktBase::CMDType	CmdCharRespawnAreaInfo		= 0xB5;			// 선택한 리스폰 지역의 세부 정보

const PktBase::CMDType	CmdCharEquipShopInfo		= 0xB6;			// NPC 장비 상점 정보
const PktBase::CMDType	CmdSiegeBroadCast2nd		= 0xB7;			// 공성 오브젝트 브로드 캐스트 다른 버젼
const PktBase::CMDType	CmdCharItemOptionGraft		= 0xB8;			// 아이템 옵션 이식
const PktBase::CMDType	CmdCharItemCompensation		= 0xB9;			// 아이템 보상 판매

const PktBase::CMDType	CmdGuildMemberGoldUpdate	= 0xBA;			// 길드 멤버 돈 업데이트

const PktBase::CMDType	CmdCampMessage				= 0xBB;			// 길드 요새 관련 메시지 패킷
const PktBase::CMDType  CmdCharDeadInfo             = 0xBC;         // 캐릭터 사망 정보

const PktBase::CMDType	CmdNewSiegeBroadCast		= 0xBD;			// 공성 오브젝트 브로드 캐스트 최신 버젼 (스피어 트리 사용)


const PktBase::CMDType	CmdSelectAccountNation		= 0xBE;			// 계정 국적 변경 패킷

const PktBase::CMDType	CmdNationChangeResult		= 0xBF;			// 계정 국적 변경으로 인한 변경된 캐릭터 정보 패킷

const PktBase::CMDType  CmdUnifiedCharInfo          = 0xC0;         // 서버통합시 캐릭터 정보 보내기
const PktBase::CMDType  CmdUnifiedCharSelect        = 0xC1;         // 서버통합시 캐릭터 / 창고 선택

const PktBase::CMDType  CmdChatBan					= 0xC2;			// 채팅 금지
const PktBase::CMDType  CmdGiveItemToTempInven      = 0xC3;         // 임시 인벤으로 아이템 지급
const PktBase::CMDType  CmdCharNameChange           = 0xC4;         // 캐릭터 이릅 바꾸기

const PktBase::CMDType	CmdFertilityInfo			= 0xC5;			// 지력 정보 전송				(DBAgentServer -> GameServer)
const PktBase::CMDType	CmdMiningCampMineralInfo	= 0xC6;			// 채굴기 누적 광물 정보 전송	(GameServer -> Client)
const PktBase::CMDType	CmdProcessMining			= 0xC7;			// 채굴기 프로세스 실행
const PktBase::CMDType	CmdWorldWeaponInfo			= 0xC8;			// 월드 웨폰 정보 전송			(DBAgentServer -> GameServer)
const PktBase::CMDType	CmdCampShopInfo				= 0xC9;			// 길드 요새 상점 정보 전송		(GameServer -> DBAgentServer)

const PktBase::CMDType	CmdCharInstallRuneSocket	= 0xCA;			// 룬 아이템 설치/제거정보.

const PktBase::CMDType	CmdTakeMaterial				= 0xCB;			// 길드 요새 공성 자재 넣기/빼기

const PktBase::CMDType	CmdCastleTaxMove			= 0xCC;			// 성의 임시 세금이 누적 세금으로 옮겨지는 정보 전송
const PktBase::CMDType	CmdCastleSiegeCount			= 0xCD;			// 공성 시간후 남은 공성 횟수 정보 전송

const PktBase::CMDType  CmdCompressedPacket         = 0xCE;         // 패킷 여러개를 압축해서 보낼 때 사용

const PktBase::CMDType	CmdTakeCastleJewel			= 0xCF;			// 성 상징물 보석 아이템 넣기/빼기
const PktBase::CMDType	CmdCastleMineralInfo		= 0xD0;			// 성의 임시 광물 세금 업데이트, 성의 누적 광물 세금 정보 전송
																	// (GS -> DBAS),				 (GS -> Client)

const PktBase::CMDType	CmdWarOnOff					= 0xD1;			// 길드, 국가 전쟁 참가 패킷 (Client <-> GS).

const PktBase::CMDType	CmdRealmPoint				= 0xD2;			// 국가 전쟁 공헌 훈장 포인트 전송 (DBAgentServer -> GameServer -> Client).

const PktBase::CMDType	CmdStatueCmd				= 0xD3;			// 국가 전쟁 석상 커맨드 패킷

const PktBase::CMDType	CmdSaveEnemy				= 0xD4;			// 듀얼시 연사 방지 패킷 (DBAgentServer <-> GameServer).

const PktBase::CMDType	CmdGuildHostilityList		= 0xD5;			// 적대 관계 현황 리스트 요청 패킷
const PktBase::CMDType	CmdGuildRelationInfo		= 0xD6;			// 길드 가입시 자기 길드의 관계 리스트 요청 패킷

const PktBase::CMDType	CmdGuildPosition			= 0xD7;			// 운영툴에서 길드 포지션 설정.
const PktBase::CMDType	CmdAdminCommandLog			= 0xD8;			// 운영자 명령어 로그 패킷.

const PktBase::CMDType	CmdChatBanInfo				= 0xD9;			// 채팅금지 정보 (GameServer <-> ChatServer).

const PktBase::CMDType	CmdKeyInfo					= 0xDA;			// 조이스틱 키 정보.

const PktBase::CMDType	CmdFinalPacketNum			= 0xDB;			// 마지막 패킷 번호	
														
#endif
