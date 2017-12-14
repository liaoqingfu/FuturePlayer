#ifndef PIXELFORMAT_HPP
#define PIXELFORMAT_HPP
#include <QVector>


enum class QMPlay2PixelFormat //Compatible with FFmpeg
{
    YUV420P =  0,
    YUV422P =  4,
    YUV444P =  5,
    YUV410P =  6,
    YUV411P =  7,
    YUV440P = 33,

    Count   =  6
};

class  QMPlay2PixelFormats: public QVector<QMPlay2PixelFormat>
{
public:

    QMPlay2PixelFormats(){}
    ~QMPlay2PixelFormats(){}
};

#endif // PIXELFORMAT_HPP
