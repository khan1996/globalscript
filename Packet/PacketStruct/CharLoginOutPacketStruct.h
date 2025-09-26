#ifndef _CHAR_LOGIN_OUT_PACKET_STRUCT_
#define _CHAR_LOGIN_OUT_PACKET_STRUCT_

#include <DB/DBDefine.h>

namespace DBUpdateData
{    
	enum UpdateList
	{ 
		STATUS_UPDATE       	= 0,    
        POSITION_UPDATE         = 1,
        SKILL_UPDATE        	= 2,    
        QUICKSLOT_UPDATE        = 3,	
		SPELL_UPDATE	        = 4,
        ITEM_EQUIP_UPDATE   	= 5,
        ITEM_INVEN_UPDATE       = 6,
        ITEM_EXTRA_UPDATE   	= 7,    
        ITEM_EXCHANGE_UPDATE    = 8, 
		ITEM_TEMPINVEN_UPDATE	= 9, 

        MAX_UPDATE_DB			= 10
	};

	enum UpdateType
    {
        LOGIN           = 0,    
        LOGOUT      	= 1,
        UPDATE          = 2,    
        ADMIN_LOGIN 	= 3,
        ZONEMOVE        = 4,    
        ABNORMAL_LOGOUT = 5,
        MAX_UPDATE_TYPE = 6
    };

    enum Limit
    {
        MAX_DBUPDATE_SIZE  = 
            sizeof(CHAR_INFOST) + sizeof(CHAR_POS) + sizeof(SKILL) + sizeof(QUICK) + sizeof(SPELL) +
			sizeof(EQUIP) + sizeof(INVEN) + sizeof(EXTRA) + sizeof(EXCHANGE) + sizeof(TEMPINVEN)
    };
};


#endif