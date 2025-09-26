// ----------------------------------------------------------------
// !! 주의 : 서버에서도 이 숫자를 사용하므로 수정시 주의 바람 !!
// 각종 팝업 메뉴

// 인간 NPC
#define POPUP_NORMAL					1
/*#define POPUP_GUARD						2
#define POPUP_NOBIND					3

#define POPUP_WEAPONMERCHANT			11
#define POPUP_ARMOURMERCHANT			12
#define POPUP_ETCMERCHANT				13

#define POPUP_WEAPONNARMOURMERCHANT		14

#define POPUP_DARKWEAPONMERCHANT		16
#define POPUP_DARKARMOURMERCHANT		17
#define POPUP_DARKETCMERCHANT			18

#define POPUP_BLACKSMITH				21

#define POPUP_COUNSELLER				26
#define POPUP_MEDALSHOP					27

#define POPUP_ADVENTURETRAINER			31
#define POPUP_FIGHTERTRAINER			32
#define POPUP_ROGUETRAINER				33
#define POPUP_MAGETRAINER				34
#define POPUP_ACOLYTETRAINER			35

#define POPUP_GUILDMAKER				36

#define POPUP_TELEPORTER				41
#define POPUP_PEACEGUILD				42
#define POPUP_CONTAINER					43

#define POPUP_STATUSCLEANER				44
#define POPUP_CLASSCLEANER				45

#define POPUP_COMPENSATION				46
#define POPUP_SKILLMERCHANT				47

// 아칸 NPC
#define POPUP_A_NORMAL					51
#define POPUP_A_GUARD					52
#define POPUP_A_NOBIND					53

#define POPUP_A_WEAPONMERCHANT			61
#define POPUP_A_ARMOURMERCHANT			62
#define POPUP_A_ETCMERCHANT				63

#define POPUP_A_DARKWEAPONMERCHANT		66
#define POPUP_A_DARKARMOURMERCHANT		67
#define POPUP_A_DARKETCMERCHANT			68

#define POPUP_A_BLACKSMITH				71

#define POPUP_A_COUNSELLER				76
#define POPUP_A_MEDALSHOP				77

#define POPUP_A_BEGINNERTRAINER			81
#define POPUP_A_COMBATANTTRAINER		82
#define POPUP_A_OPPICATERTRAINER		83

#define POPUP_A_GUILDMAKER				86

#define POPUP_A_TELEPORTER				91
#define POPUP_A_PEACEGUILD				92
#define POPUP_A_CONTAINER				93

#define POPUP_A_STATUSCLEANER			94
#define POPUP_A_CLASSCLEANER			95

// PC
#define POPUP_PC						100
#define POPUP_LEADER					101
#define POPUP_MEMBER					102

// Battle Ground Client 관련
#define POPUP_BATTLEGROUND_MANAGER      103
#define POPUP_MONOEY_CHANGER            104
#define POPUP_BATTLEGROUND_EXITER       105

#define POPUP_A_BATTLEGROUND_MANAGER    106
#define POPUP_A_MONOEY_CHANGER          107
#define POPUP_A_BATTLEGROUND_EXITER     108
*/
// 공성 관련 POPUP 메뉴
#define POPUP_EMBLEM					109			
#define POPUP_GATE						110
#define POPUP_BACKDOOR					111
#define POPUP_CAMP						112
#define POPUP_UPGRADE_CAMP				113					//업그레이드 된 요새
#define POPUP_CAMP_SHOP					114					//상점 요새
#define POPUP_MINING_CAMP				115					//채굴기

#define POPUP_CASTLE_ARMS_NPC			116	
#define POPUP_CASTLE_GUARD				117
#define POPUP_SHORT_RANGE_CASTLE_ARMS	118
#define POPUP_LONG_RANGE_CASTLE_ARMS	119
#define POPUP_SHORT_RANGE_SIEGE_ARMS	120
#define POPUP_LONG_RANGE_SIEGE_ARMS		121
#define POPUP_AIRSHIP					122
/*
// TODO : 추후 NPC가 3개 이상의 상점창을 갖을 수 있도록 수정한 후 종족별로 합칩니다.
// TODO : 그 다음 단계로는 아예 팝업과 탭을 스크립트로 제어할 수 있게끔 하여 NPC 직업을 제거합시다.
// 스킬 상위 트레이너
#define POPUP_ADVANCEDADVENTURE			123
#define POPUP_ADVANCEDFIGHTER			124
#define POPUP_ADVANCEDROGUE				125
#define POPUP_ADVANCEDMAGE				126
#define POPUP_ADVANCEDACOLYTEE			127
#define POPUP_ADVANCEDBEGINNER			128
#define POPUP_ADVANCEDCOMBATANT			129
#define POPUP_ADVANCEDOPPICATER			130

// 
#define POPUP_SELLINGLIST               131         // 탭 이름에 '판매목록' 하나 있는 넘

#define POPUP_CHANGENAME				132			// 이름 바꾸는 NPC
*/
// 임시로 뒤로 빼놓았음 (위에 번호가 NPC 스크립트에 영향을 미치기 때문에...)
#define POPUP_WORLDWEAPON				133					//월드웨폰

// ----------------------------------------------------------------
// 팝업 메뉴의 종류

//-- PC용 
#define POPUPMENU_EXCHANGE			                0
#define POPUPMENU_DUEL				                1
#define POPUPMENU_PARTYINVATE		                2
#define POPUPMENU_STREETSTALL		                3
#define POPUPMENU_LEADER			                4
#define POPUPMENU_VANISH			                5
#define POPUPMENU_PARTYDUEL			                6
#define POPUPMENU_FRIEND			                7
#define POPUPMENU_BAN				                8
#define POPUPMENU_FOLLOW			                9

//-- NPC용
#define POPUPMENU_DIALOG			                10000   // 다이얼로그 띄우는 메뉴

#define POPUPMENU_SAVEPOS			                101     // 위치 저장
#define POPUPMENU_UPGRADE			                102     // 재련
#define POPUPMENU_UNLOCK			                103     // 스킬 언락?(안씀?)
#define POPUPMENU_JOBCHANGE			                104     // 전직
#define POPUPMENU_ZONEMOVE			                105     // 텔레포트
#define POPUPMENU_DIALOG2			                106     // 이거 테스트 후 지울 거
#define POPUPMENU_DEPOSIT			                107     // 창고
#define POPUPMENU_QUEST				                108     // 퀘스트
#define POPUPMENU_QUESTTRIGGER						109		// 퀘스트 트리거
#define POPUPMENU_PEACE				                111     // 평화모드
#define POPUPMENU_STATUSCLEAN		                113     // 스탯 초기화
#define POPUPMENU_CLASSCLEAN		                114     // 클래스 초기화
#define POPUPMENU_CHANGENAME						115		// 이름 바꾸기

#define POPUPMENU_TRADE						        401     // 거래하기( 뒤에 숫자 붙는 것들은 차후에 사라질 예정, 거래도 이 아이디 하나로 여러개 붙임 )
#define POPUPMENU_TRADE0						    402     
#define POPUPMENU_TRADE1						    403
#define POPUPMENU_TRADE2						    404

// 길드 관련
#define POPUPMENU_GUILDINVITE		                221     // 길드 초대( PC용 팝업 메뉴 )
#define POPUPMENU_MAKEGUILD			                222     // 길드 만들기

#define POPUPMENU_STATUSRETRAIN		                223     // 스탯 재분배

// 배틀로한 관련
#define POPUPMENU_BATTLEGROUND_MANAGE               224     // 배틀로한 
#define POPUPMENU_BATTLEGROUNDPREV_RESULT           225     // 지난 경기 결과 보기
#define POPUPMENU_MONEY_CHANGE                      226     // 환전소
#define POPUPMENU_BATTLEGROUND_EXIT                 227     // 경기중 나가기( 라지맵상에 탈출버튼 만들고 NPC가 사라지면서 안씀 )

//---------------------------------------------
// 공성관련 PopupMenu 

// 성 상징물
#define POPUPMENU_TAXCONTROL						330		// 판매 물건 세율 조정
#define POPUPMENU_CASTLERIGHTSETTING				331		// 성 구조물에 대한 권한 설정
#define POPUPMENU_CASTLEEMBLEMUPGRADE				332		// 성 소유 상징물의 업그레이드
#define POPUPMENU_CASTLEEMBLEMUPGRADEEXPLANATION	333		// 성 소유 업그레이드 설명 보기explanation
#define POPUPMENU_CURRENTTEX						334		// 세금 현황

// 성문
#define POPUPMENU_CASTLEOPEN						335		// 성문 열기
#define POPUPMENU_CASTLECLOSE						336		// 성문 닫기
#define POPUPMENU_CASTLEDEFENCE						337		// 성문 막기
#define POPUPMENU_CASTLEUPGRADE						338		// 성문 업그레이드
#define POPUPMENU_CASTLEREPAIR						339		// 성문 수리
#define POPUPMENU_CASTLERECOVERY					340		// 성문 복구

// 뒷문
#define POPUPMENU_CASTLEIN							341		// 성 안으로
#define POPUPMENU_CASTLEOUT							342		// 성 밖으로

// 진지
#define POPUPMENU_CAMPPOSSAVE						343		// 위치 저장
#define POPUPMENU_CAMPCARE							344		// 진지 손질하기
#define POPUPMENU_CAMPUPGRADE						345		// 진지 업그레이드
#define POPUPMENU_CAMPREPAIR						346		// 진지 수리하기
#define POPUPMENU_CAMPRIGHTSETTING					347		// 진지 관리 권한 설정
#define POPUPMENU_CAMPDESTROY						348		// 진지 파괴
#define POPUPMENU_CAMPEXPLANATION					349		// 진지 도움말 (설명)
#define POPUPMENU_CHANGEBASE						350		// 기본형으로 변형
#define POPUPMENU_CHANGESHOP						351		// 상점 타입으로 변형
#define POPUPMENU_CHANGEMINING						352		// 채굴기 타입으로 변형

// 근거리 공성병기
#define POPUPMENU_SHORTSIEGEARMSRIDE				353		// 근거리 공성병기 탑승
#define POPUPMENU_SHORTSIEGEARMSREPAIR				354		// 근거리 공성병기 수리
#define POPUPMENU_SHORTSIEGEARMSSTARTKITRECOVERY	355		// 근거리 공성병기 스타터 킷 복구

// 원거리 공성병기
#define POPUPMENU_LONGSIEGEARMSRIDE					356		// 원거리 공성병기 탑승
#define POPUPMENU_LONGSIEGEARMSREPAIR				357		// 원거리 공성병기 수리
#define POPUPMENU_LONGSIEGEARMSSTARTKITRECOVERY 	358		// 원거리 공성병기 스타터 킷 복구

// 근거리 수성병기
#define POPUPMENU_SHORTCASTLEARMSRIDE				359		// 근거리 수성병기 탑승
#define POPUPMENU_SHORTCASTLEARMSREPAIR				360		// 근거리 수성병기 수리
#define POPUPMENU_SHORTCASTLEARMSUPGRADE			361		// 근거리 수성병기 업그레이드
#define POPUPMENU_SHORTCASTLEARMSDESTROY			362		// 근거리 수성병기 파괴

// 원거리 수성병기
#define POPUPMENU_LONGCASTLEARMSRIDE				363		// 원거리 수성병기 탑승
#define POPUPMENU_LONGCASTLEARMSREPAIR				364		// 원거리 수성병기 수리
#define POPUPMENU_LONGCASTLEARMSUPGRADE				365		// 원거리 수성병기 업그레이드
#define POPUPMENU_LONGCASTLEARMSDESTROY				366		// 원거리 수성병기 파괴

// 비공정
#define POPUPMENU_DROPSHIPRIDE						367		// 원거리 공성병기 탑승
#define POPUPMENU_DROPSHIPREPAIR					368		// 원거리 공성병기 수리
#define POPUPMENU_DROPSHIPRECOVERY					369		// 원거리 공성병기 스타터 킷 복구 

// 병기 개발 NPC
#define POPUPMENU_CASTLEDEFENCEARMSMENU				370		// 성 방어 병기 설명 보기
#define POPUPMENU_GUARDDEVELOP						371		// 가드 개발
#define POPUPMENU_SHORTDEFENCEARMSDEVELOP			372		// 근거리 방어용 병기 개발
#define POPUPMENU_LONGDEFENCEARMSDEVELOP			373		// 원거리 방아용 병기 개발

// 가드
#define POPUPMENU_CASTLEGUARDREPAIR					374		// 원거리 수성병기 수리
#define POPUPMENU_CASTLEGUARDUPGRADE				375		// 원거리 수성병기 업그레이드
#define POPUPMENU_CASTLEGUARDDESTROY				376		// 원거리 수성병기 파괴

//---------------------------------------------

#define POPUPMENU_OPTIONCOMPOSITION				    379		// 옵션 이식
#define POPUPMENU_COMPENSATION					    380		// 보상 판매
#define POPUPMENU_MININGCAMPMENU					381		// 채굴기 메뉴

#define POPUPMENU_WEAPONLAUNCH						382		// 월드 웨폰 발사
#define POPUPMENU_WEAPONREPAIR						383		// 월드 웨폰 수리
#define POPUPMENU_WEAPONRIGHTSETTING				384		// 월드 웨폰 권한 설정
#define POPUPMENU_WEAPONDESTROY						385		// 월드 웨폰 파괴
#define POPUPMENU_WEAPONEXPLANATION					386		// 월드 웨폰 도움말
#define POPUPMENU_WEAPONLOAD						388		// 월드 웨폰 장전

#define POPUPMENU_CAMPSHOPMENU						389		// 길드 요새 상점 메뉴
#define POPUPMENU_WEAPONUPGRADE						390		// 월드 웨폰 업그레이드
#define POPUPMENU_CAMPNORIGHTMASTER					391		// 길드 요새 관련 - 권한 없음. 길드 마스터만 가능
#define POPUPMENU_CAMPNORIGHTADMIN					392		// 길드 요새 관련 - 권한 없음. 길드 마스터 + 관리자 가능
#define POPUPMENU_CAMPSHOPADMIN						393		// 길드 요새 상점 관리