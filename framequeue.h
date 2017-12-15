#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H

#include <vector>
#include <iostream>
#include <memory>

class TBBuffer
{
public:
   static TBBuffer * CreateInstance(int size = 2048,const char * filter = 0);
   virtual ~TBBuffer(){}
   virtual char * Data() { return 0;}
   virtual int Size() {return 0;}
   virtual void Add(void * data, int len){}
   virtual void Clear() {}
   virtual TBBuffer * Clone() { return 0;}
};


class TBSharedBuffer:
    public TBBuffer
{
public:
    TBSharedBuffer(int size = 1024 );
    ~TBSharedBuffer();

    virtual char * Data();
    virtual int Size();
    virtual void Add(void * data, int len);
    virtual void Clear();
    virtual TBBuffer *Clone();
protected:
    typedef std::shared_ptr<std::vector<char> > BufPtr;
    BufPtr buf_;
};



class FrameQueue
{
public:
    FrameQueue();
};

#endif // FRAMEQUEUE_H
