#ifndef _CLIENTSOCKET_MESSAGE_BLOCK_H_
#define _CLIENTSOCKET_MESSAGE_BLOCK_H_

class CMessageBlock
{
public:

    explicit CMessageBlock();
    explicit CMessageBlock(size_t n);
    virtual ~CMessageBlock();

    bool is_valid() { return 0 != internal_buffer_; }

    // 읽기 - 쓰기 포인터 얻어 오기.
    char* rd_ptr() const   { return rd_ptr_; }
    void rd_ptr(size_t n)  { rd_ptr_ += n; }
    void rd_ptr(char* ptr) { rd_ptr_ = ptr; }
    
    char* wr_ptr() const   { return wr_ptr_; }
    void wr_ptr(size_t n)  { wr_ptr_ += n; }
    void wr_ptr(char* ptr) { wr_ptr_ = ptr; }

    // 버퍼 시작 및 끝경과(past-end) 값 얻기.
    char* begin() const { return internal_buffer_; }
    char* end() const { return internal_buffer_ + buffer_size_; }
   
    // 데이터를 버퍼로 복사.
    bool push(const void* ptr, size_t n);

    template<typename T>
    bool push(const T& t)
    {
        if(0 != wr_ptr_ && sizeof(T) <= capacity() - length())
        {
            memcpy(wr_ptr_, &t, sizeof(T));
            wr_ptr_ += sizeof(T);
            return true;
        }

        return false;
    }

    template<typename T>
    bool pop(T& t)
    {
        if(0 != rd_ptr_ && sizeof(T) <= length())
        {   
            memcpy(&t, rd_ptr_, sizeof(T));
            rd_ptr_ += sizeof(T);
            return true;
        }

        return false;
    }    

    // 이미 읽은 데이터들을 버리고 버퍼 및 내용을 재조정. 각 포인터들이 무효화됨.
    void pop_read_data();

    size_t length() const { return wr_ptr_ - rd_ptr_; }     // 데이터 길이
    size_t capacity() const { return buffer_size_; }        // 버퍼 크기
    size_t remain() const { return internal_buffer_ + buffer_size_ - wr_ptr_; }  // 남은 데이터 공간
    
    // 현재 자신의 내용을 복사한다.
    static CMessageBlock* clone(CMessageBlock& msg_block);

    // 버퍼 크기를 늘린다.
    bool resize(size_t n);

    // 초기화
    bool init(size_t n);

private:

    void destroy();

    char*   internal_buffer_;
    char*   rd_ptr_;
    char*   wr_ptr_;
    size_t  buffer_size_;
};

#endif