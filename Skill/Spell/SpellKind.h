#ifndef _SPELL_KIND_H_
#define _SPELL_KIND_H_

namespace Skill
{
    namespace SpellID
    {
        enum Type
        {
            None				= 0,

		    BattleSong	        = 1,		
			MaintenanceChant	= 2,
            AccelerationChant	= 3,	    
			LifeAura	        = 4,	
			SpeedBuff			= 5,		
			Regeneration	    = 6,		
            Slow			    = 7,		
			ArmorBroken		    = 8,
            Blaze			    = 9,		
			Charging		    = 10,
			Stealth			    = 11,		
			ManaShell		    = 12,
            Encourage		    = 13,		
			EnchantWeapon	    = 14,

            BrightArmor		    = 15,		
			HardenSkin		    = 16,
			Flexibility		    = 17,		
			Guard			    = 18,
            Hold			    = 19,		
			Stun			    = 20,
            Frozen			    = 21,		
			Poisoned		    = 22,
			LowerStrength	    = 23,		
			Invincible		    = 24,
            Hide                = 25,		
			StoneForm			= 26,		
			CounterAttack		= 27, 
			Envenom				= 28,		

			BombSet				= 29, 
			Fired				= 30,
			CurseOfBlind		= 31,		
			DeCoolDown			= 32,		
			ExpOrb				= 33,
			LuckyOrb			= 34,

			DemagePotion		= 35,
			ArmorPotion			= 36,		
			HitRatePotion		= 37,		
			EvadePotion			= 38,
			MaxHPPotion			= 39,		
			MaxMPPotion			= 40,
			HPRegenPotion		= 41,
			MPRegenPotion		= 42,		
			CriticalPotion		= 43,
			BlockPotion			= 44,		
			SpeedPotion			= 45,
			DeCoolDownPotion	= 46,		
			MagicPowerPotion	= 47,
			MagicResistPotion	= 48,

			PowerStatue			= 49,		
			IntStatue			= 50,		
			ExpStatue			= 51,		
			WealthStatue		= 52,		
			RealmHP				= 53,		
			RealmMP				= 54,
			Concentration		= 55,		

			StealHand			= 56,	// 어세 추가스킬 
			MichaelBless		= 57,

			// 이 아래로는 버프창에서 표시가 안된다.
			TasteWine			= 58, 
			WifeCracker			= 59,		
			KarterantWorldBuff	= 60,		
			MerkadiaWorldBuff	= 61,
			KarterantWorldDeBuff= 62,		
			MerkadiaWorldDeBuff	= 63,
			HundredLevel		= 64,

			Drunk				= 65,


			CD_8D12				= 66,
			CD_8D14				= 67,
			CD_8D16				= 68,
			CD_99A1				= 69,
			CD_99A2				= 70,
			CD_99A3				= 71,

			MAX_SPELL_TYPE		= 72
        };
    }

	namespace SpellTarget
    {
        enum Type
        {
            NONE                    = 0,
            CHANT                   = 1,
            FRIEND_TARGET_ENCHANT   = 2,
            ENEMY_TARGET_ENCHANT    = 3,
			ALL_ENCHANT				= 4
        };
    };

	namespace Disenchant
	{
		enum Type
		{
			NONE		= 0,
			LEVEL		= 1,
			DURATION	= 2,
			DEAD		= 3
		};

		enum Const
		{
			INFINITE_NUM	= UCHAR_MAX
		};
	};

	namespace SpellType
	{
		enum Type
		{
			NONE						= 0,
			MAGICAL_SPELL				= 1,			// 마법형 주문
			PHYSICAL_SPELL				= 2,			// 물리적 주문 (ex - 넷)
			WORLDWEAPON_SPELL			= 3,			// 월드 웨폰 인챈트

			REALM_SPELL					= 4,			// 국가전쟁 공헌훈장 효과 인첸트.

			BUFF_SPELL					= 5,			// DB에 저장되는 버프관련 인첸트 (죽으면 삭제된다)
			PAYBUFF_SPELL				= 6,			// 유료 아이템 이건 죽어도 삭제안된다.
			STATUE_SPELL				= 7,			// 석상 보상 효과 인첸트

			ETERNAL_SPELL				= 8,			// 영구적으로 유지되야 하는 인챈트 (ex - 100랩 이펙트)
		};
	};
};

#endif