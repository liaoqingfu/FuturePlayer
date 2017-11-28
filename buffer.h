#ifndef BUFFER_H
#define BUFFER_H


#include <QtGlobal>

extern "C"
{
    #include "libavutil/buffer.h"
    #include "libavutil/mem.h"
}

class Buffer
{
    AVBufferRef *m_bufferRef = nullptr;
    qint32 m_size = 0;

public:
    Buffer() = default;
    Buffer(const Buffer &other);
    inline Buffer(Buffer &&other);
    ~Buffer();

    inline bool isNull() const;
    inline bool isEmpty() const;

    inline qint32 size() const;
    qint32 capacity() const;

    bool isWritable() const;

    void resize(qint32 len);
    void remove(qint32 pos, qint32 len);
    void clear();

    const quint8 *data() const;
    inline const quint8 *constData() const;
    quint8 *data(); //Automatically detaches the buffer if necessary

    void assign(AVBufferRef *otherBufferRef, qint32 len = -1);
    void assign(const void *data, qint32 len, qint32 mem);
    inline void assign(const void *data, qint32 len);

    AVBufferRef *toAvBufferRef();

#if 0
    void append(const void *data, qint32 len);
#endif

    Buffer &operator =(const Buffer &other);
    inline Buffer &operator =(Buffer &&other);

private:
    inline void move(Buffer &other);
};

#endif // BUFFER_H
