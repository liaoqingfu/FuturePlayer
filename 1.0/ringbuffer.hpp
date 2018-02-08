#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <QMutex>
#include <pthread.h>
#include <inttypes.h>

#define RING_BUFFER_SIZE 500

class RingBuffer
{
public:

    void Init(int capacity);

    /**
     *
     * @param capacity must < RING_BUFFER_SIZE
     */
    RingBuffer(int capacity = RING_BUFFER_SIZE);


    ~RingBuffer(void);

    /**
     * @brief check the ringbuffer is empty (no data in it)
     *
     * @param rb ringbuffer
     *
     * @retval 1 empty
     * @retval 0 not empty
     */
    bool RingBufferIsEmpty();

    /**
     * @brief check the ringbuffer is full (no more data will fit)
     *
     * @param rb ringbuffer
     *
     * @retval 1 empty
     * @retval 0 no empty
     */
    bool RingBufferIsFull();

    /**
     * @brief get number of items in the ringbuffer
     *
     * @param rb ringbuffer
     */
    int RingBufferSize();
    void *RingBufferSrSwGet(int *p_len);

    int RingBufferSrSwPut(void *ptr, int len);
private:
    int m_front;
    int m_rear;
    int m_size;
    int m_capacity;
    void *array[RING_BUFFER_SIZE];
    int array_len[RING_BUFFER_SIZE];
    QMutex _mutex;
};
#endif // RINGBUFFER_HPP
