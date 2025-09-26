#ifndef _TYPE_ARRAY_H_
#define _TYPE_ARRAY_H_

class CTypeName 
{
public:

    CTypeName() : m_nType(0), m_lpszName(0) { }
	CTypeName(unsigned char nType, char* lpszName) : m_nType(nType), m_lpszName(lpszName) { }

    unsigned char   GetTypeValue() const { return m_nType; }
    const char*     GetTypeName()  const { return m_lpszName; }

    static bool Test(const CTypeName* lpTypeArray, int nArrayNum, const char* lpTypeArrayName);

private:

    unsigned char		m_nType;
	char*				m_lpszName;
};

#endif