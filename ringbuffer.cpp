#include "ringbuffer.hpp"

void RingBuffer::Init(int capacity)
{
    m_front = 0;
    m_rear = 0;
    if (capacity > RING_BUFFER_SIZE)
    {
        m_capacity = RING_BUFFER_SIZE;
    }
    else
    {
        m_capacity = capacity;
    }
    m_size = 0;
}

/**
 *
 * @param capacity must < RING_BUFFER_SIZE
 */
RingBuffer::RingBuffer(int capacity)
{
    Init(capacity);
}


RingBuffer::~RingBuffer(void)
{
    void *ptr = nullptr;

    _mutex.lock();
    while (m_size > 0)      // 释放资源
    {
        ptr = array[m_front];
        delete [] ptr;
        m_front = (m_front + 1) % m_capacity;
        m_size--;
    }
    _mutex.unlock();
}

/**
 * @brief check the ringbuffer is empty (no data in it)
 *
 * @param rb ringbuffer
 *
 * @retval 1 empty
 * @retval 0 not empty
 */
bool RingBuffer::RingBufferIsEmpty()
{
    bool res;

    _mutex.lock();
    res = (m_front == m_rear);
    _mutex.unlock();

    return res;
}

/**
 * @brief check the ringbuffer is full (no more data will fit)
 *
 * @param rb ringbuffer
 *
 * @retval 1 empty
 * @retval 0 no empty
 */
bool RingBuffer::RingBufferIsFull()
{
    bool full;
    _mutex.lock();
    full = (m_front == ((m_rear + 1) % m_capacity));
    _mutex.unlock();
    return full;
}

/**
 * @brief get number of items in the ringbuffer
 *
 * @param rb ringbuffer
 */
int RingBuffer::RingBufferSize()
{
    int size = 0;
    _mutex.lock();
    size = m_size;
    _mutex.unlock();
    return size;
}

void *RingBuffer::RingBufferSrSwGet(int *p_len)
{
    void *ptr = NULL;
    if (RingBufferIsEmpty())
    {
        *p_len = 0;
        return NULL;
    }
    else
    {
        _mutex.lock();
        ptr = array[m_front];
        *p_len = array_len[m_front];
        m_front = (m_front + 1) % m_capacity;
        m_size--;
        _mutex.unlock();
        return ptr;
    }
}

int RingBuffer::RingBufferSrSwPut(void *ptr, int len)
{
    int ret;
    if (RingBufferIsFull())
    {
        return -1;
    }
    _mutex.lock();
    array[m_rear] = ptr;
    array_len[m_rear] = len;
    m_rear = (m_rear + 1) % m_capacity;
    m_size++;
    _mutex.unlock();

    return 0;
}
