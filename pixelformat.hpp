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
using QMPlay2PixelFormats = QVector<QMPlay2PixelFormat>;
#endif // PIXELFORMAT_HPP
