#ifndef _MINI_LZO_WRAPPER_H_
#define _MINI_LZO_WRAPPER_H_

namespace CMiniLZO
{
	bool CheckLZOUsable();
	
    bool Compress(const char* in, unsigned long in_len, 
		char* out, unsigned long* lp_out_len);

    bool Decompress(const char* in, unsigned long in_len, 
		char* out, unsigned long* buffersize_in_out_len);

	inline unsigned long GetLeastCompressBuffer(unsigned long dwInputBlockSize) 
	{
		return dwInputBlockSize + (dwInputBlockSize / 64) + 16 + 3;
	}
};

#endif