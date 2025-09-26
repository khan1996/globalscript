#ifndef _CRC32STATIC_H_
#define _CRC32STATIC_H_

#include "Common.h"
#include <windows.h>

class CCrc32Static
{
public:
	CCrc32Static();
	virtual ~CCrc32Static();

	static unsigned long BufferCrc32(const char *szBuffer, size_t size, unsigned long &dwCrc32);
	static unsigned long StringCrc32(const char *szString, unsigned long &dwCrc32);
	static unsigned long FileCrc32Streams(const char *szFilename, unsigned long &dwCrc32);
	static unsigned long FileCrc32Win32(const char *szFilename, unsigned long &dwCrc32);
	static unsigned long FileCrc32Filemap(const char *szFilename, unsigned long &dwCrc32);
	static unsigned long FileCrc32Assembly(const char *szFilename, unsigned long &dwCrc32);

protected:
	static bool GetFileSizeQW(const HANDLE hFile, __int64 &qwSize);
	static inline void CalcCrc32(const unsigned char byte, unsigned long &dwCrc32);

	static unsigned long s_arrdwCrc32Table[256];
};

#endif
