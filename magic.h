#ifndef QMMP_ADPLUG_MAGIC_H
#define QMMP_ADPLUG_MAGIC_H

#include <cstddef>
#include <string>

class InputStream
{
  public:
    virtual bool read(std::size_t offset, void *buf, std::size_t n) = 0;
};

bool adplug_supports(InputStream &&);

#endif
