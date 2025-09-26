/* TODO : REMOVE HERE
#ifndef _OBJECT_INFO_H_
#define _OBJECT_INFO_H_

#include <Network/Packet/PacketStruct/CharBroadcastPacket.h>


namespace BroadcastInfo
{
    // 전방 참조
    class CObjectInfo;
    class CAggresiveCreatureInfo;
    class CMonsterInfo;
	class CSummonMonsterInfo;
    
    // 브로드캐스팅 정보 처리용 인터페이스
    struct  IProcessBroadcastInfo
    {
        // 각 처리 루틴을 전부 처리하여야 함. 타입에 맞춰서 함수가 호출된다.
        virtual void Process(CObjectInfo& objectInfo) = 0;
        virtual void Process(CAggresiveCreatureInfo& aggresiveCreatureInfo) = 0;
        virtual void Process(CMonsterInfo& monsterInfo) = 0;
		virtual void Process(CSummonMonsterInfo& monsterInfo) = 0;
    };

    class CObjectInfo
    {
    public:

        virtual ~CObjectInfo() { }
        virtual void Update(const SObject& object, unsigned long dwLastUpdateTick);
        virtual void Process(IProcessBroadcastInfo& processBroadcastInfo) { processBroadcastInfo.Process(*this); }
        
        unsigned long           GetCID() const { return m_Object.m_dwCID; }
        const CNetworkPos&      GetNetworkPos() const { return m_Object.m_NetworkPos; }
        const ObjectType::Type  GetObjectType() const { return m_Object.GetObjectType(); }        
        unsigned long           GetLastUpdateTick() const { return m_dwLastUpdateTick; }        

        struct LessCID
        {
            bool operator () (const BroadcastInfo::CObjectInfo& lhs, const BroadcastInfo::CObjectInfo& rhs) { return lhs.GetCID() < rhs.GetCID(); }
            bool operator () (const BroadcastInfo::CObjectInfo& lhs, unsigned long dwCID) { return lhs.GetCID() < dwCID; }
            bool operator () (unsigned long dwCID, const BroadcastInfo::CObjectInfo& rhs) { return dwCID < rhs.GetCID(); }

            bool operator () (const BroadcastInfo::CObjectInfo* lhs, const BroadcastInfo::CObjectInfo* rhs) { return lhs->GetCID() < rhs->GetCID(); }
            bool operator () (const BroadcastInfo::CObjectInfo* lhs, unsigned long dwCID) { return lhs->GetCID() < dwCID; }
            bool operator () (unsigned long dwCID, const BroadcastInfo::CObjectInfo* rhs) { return dwCID < rhs->GetCID(); }
        };

    protected:

        CObjectInfo();
        
        SObject         m_Object;
        unsigned long   m_dwLastUpdateTick;

        friend class    CObjectInfoMgr;
    };


    class CAggresiveCreatureInfo : public CObjectInfo
    {
    public:

        virtual void Update(const SObject& object, unsigned long dwLastUpdateTick);
        virtual void Process(IProcessBroadcastInfo& processBroadcastInfo) { processBroadcastInfo.Process(*this); }

		unsigned short  GetNowHP()	        { return m_nNowHP;	}
        EnchantInfo&    GetEnchantInfo()    { return m_EnchantInfo; }

        static CAggresiveCreatureInfo* DowncastToAggresiveCreatureInfo(CObjectInfo* lpObjectInfo);

    protected:

        CAggresiveCreatureInfo();

		EnchantInfo		m_EnchantInfo;
		unsigned short	m_nNowHP;

        friend class    CObjectInfoMgr;
    };


	class CMonsterInfo : public CAggresiveCreatureInfo
    {
    public:

		virtual void Update(const SObject& object, unsigned long dwLastUpdateTick);
        virtual void Process(IProcessBroadcastInfo& processBroadcastInfo) { processBroadcastInfo.Process(*this); }

		static CMonsterInfo* DowncastToMonsterInfo(CObjectInfo* lpObjectInfo);

    protected:

        CMonsterInfo();

		friend class    CObjectInfoMgr;
    };


    class CSummonMonsterInfo : public CMonsterInfo
    {
    public:

		virtual void Update(const SObject& object, unsigned long dwLastUpdateTick);
        virtual void Process(IProcessBroadcastInfo& processBroadcastInfo) { processBroadcastInfo.Process(*this); }

		unsigned long GetMasterID() { return m_dwMasterID; }

		static CSummonMonsterInfo* DowncastToSummonMonsterInfo(CObjectInfo* lpObjectInfo);

    protected:

        CSummonMonsterInfo();

		unsigned long   m_dwMasterID;

		friend class    CObjectInfoMgr;
    };	
}

#endif
*/