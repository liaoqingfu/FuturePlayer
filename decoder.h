#ifndef DECODER_H
#define DECODER_H

#include <QString>
class Decoder
{
public:
    Decoder();
    virtual QString name() const = 0;
};

#endif // DECODER_H
