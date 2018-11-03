#ifndef QMMP_ADPLUG_MAGIC_H
#define QMMP_ADPLUG_MAGIC_H

#include <cstddef>
#include <exception>
#include <string>

class InputStream
{
  public:
    class NoRandomAccess : public std::exception
    {
      public:
        NoRandomAccess() : std::exception() { }
    };

    virtual bool read(std::size_t offset, void *buf, std::size_t n) = 0;
    virtual size_t size() = 0;
};

bool adplug_supports(InputStream &&);

#endif
