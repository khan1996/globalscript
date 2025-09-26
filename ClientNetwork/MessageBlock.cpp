#include "stdafx.h"
#include "MessageBlock.h"
#include <xmemory>
#include "GMMemory.h"

CMessageBlock::CMessageBlock()
:   internal_buffer_(0), rd_ptr_(0), wr_ptr_(0), buffer_size_(0)    
{

}

CMessageBlock::CMessageBlock(size_t n)
:   internal_buffer_(0), rd_ptr_(0), wr_ptr_(0), buffer_size_(0)    
{
    init(n);
}

CMessageBlock::~CMessageBlock()
{
    destroy();
}

bool CMessageBlock::init(size_t n)
{
    if(buffer_size_ < n)
    {
        destroy();
        internal_buffer_ = new char[n];
    }

    rd_ptr_ = wr_ptr_ = internal_buffer_;
    buffer_size_ = n;
    
    return (0 != internal_buffer_);
}

void CMessageBlock::destroy()
{    
    if(NULL != internal_buffer_)
    {
        delete [] internal_buffer_;
    }
    
    buffer_size_ = 0;
    rd_ptr_ = wr_ptr_ = internal_buffer_ = 0;    
}

bool CMessageBlock::push(const void* ptr, size_t n)
{
    if(NULL != wr_ptr_ && n <= capacity() - length())
    {
        memcpy(wr_ptr_, ptr, n);
        wr_ptr_ += n;
        return true;
    }

    return false;
}

void CMessageBlock::pop_read_data()
{
    if(NULL != internal_buffer_)
    {        
        size_t n = length();
        memmove(internal_buffer_, rd_ptr_, n);
        wr_ptr_ = internal_buffer_ + n;
        rd_ptr_ = internal_buffer_;
    }
}

CMessageBlock* CMessageBlock::clone(CMessageBlock& msg_block)
{
    CMessageBlock* lpMessageBlock = new CMessageBlock(msg_block.length());
    
    if(NULL != lpMessageBlock)
    {
        memcpy(lpMessageBlock->wr_ptr(), msg_block.rd_ptr(), msg_block.length());
        lpMessageBlock->wr_ptr(msg_block.length());
        return lpMessageBlock;
    }

    return NULL;
}

bool CMessageBlock::resize(size_t n)
{    
    char* temp_buffer = new char[n];

    if(NULL != temp_buffer)
    {
        memcpy(temp_buffer, internal_buffer_, 
            (buffer_size_ < n) ? n : buffer_size_);

        rd_ptr_ = temp_buffer + (rd_ptr_ - internal_buffer_);
        wr_ptr_ = temp_buffer + (wr_ptr_ - internal_buffer_);
        std::swap(internal_buffer_,temp_buffer);

        delete [] temp_buffer;
        return true;
    }

    return false;
}

