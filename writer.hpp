#ifndef WRITER_H
#define WRITER_H

#include <QString>
class Writer
{
public:
    Writer();
    virtual QString name() const = 0;
};

#endif // WRITER_H
