#include "stdafx.h"
#include "TypeArray.h"

#include <Log/ServerLog.h>
#include "GMMemory.h"

bool CTypeName::Test(const CTypeName* lpTypeArray, int nArrayNum, const char* lpTypeArrayName)
{
	for(int nCount = 0; nCount < nArrayNum; ++nCount) 
    {
		if(lpTypeArray[nCount].m_nType != nCount)
        {
			ERRLOG2(g_Log, "검사한 타입들의 배열 인덱스와 타입 값이 맞지 않습니다."
				"타입 테이블 이름은 %s이고, 맞지 않는 커맨드 번호는 %d입니다.", lpTypeArrayName, nCount);
			return false;
		}
	}
	return true;
}
