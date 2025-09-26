/* TODO : REMOVE HERE

#ifndef _BROADCAST_PACKET_H
#define _BROADCAST_PACKET_H

#include <vector>

namespace BroadcastInfo
{
    // 전방 참조
    struct  SObject;
    class   CObjectInfo;
    struct  IProcessBroadcastInfo;

    // 생성 및 관리, 검색, Iteration등등을 수행할 수 있음.
    class CObjectInfoMgr
    {
    public:

        typedef std::vector<CObjectInfo*>   ObjectArray;
        typedef ObjectArray::iterator       ObjectIterator;

        CObjectInfoMgr() { }
        virtual ~CObjectInfoMgr();

        bool Update(const char* szBuffer_In, unsigned long dwBufferSize, bool bRemove = true);
        void Process(IProcessBroadcastInfo& processBroadcastInfo);
        CObjectInfo* GetObjectInfo(unsigned long dwCID);
        unsigned long GetObjectNum() { return static_cast<unsigned long>(m_ObjectArray.size()); }

        CObjectInfo* CreateOrUpdate(const SObject& object, unsigned long dwCurrentTime);
        void Remove(unsigned long dwCID);

        ObjectIterator  begin() { return m_ObjectArray.begin(); }
        ObjectIterator  end()   { return m_ObjectArray.end(); }
        void clear();        

    private:

        void Eliminate(unsigned long dwExceptTime);

        CObjectInfo*    Create(const SObject& object);
        ObjectArray     m_ObjectArray;        
    };
}

#endif
*/