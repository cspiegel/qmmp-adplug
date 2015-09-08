#ifndef QMMP_ADPLUG_ADPLUGWRAP_H
#define QMMP_ADPLUG_ADPLUGWRAP_H

#include <cstddef>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include <adplug/adplug.h>
#include <adplug/emuopl.h>

class AdplugWrap
{
  public:
    struct Frame
    {
      Frame(size_t n, unsigned char *buf) : n(n), buf(buf) { }
      size_t n;
      unsigned char *buf;
    };

    class InvalidFile : public std::exception
    {
      public:
        InvalidFile() : std::exception() { }
    };

    explicit AdplugWrap(std::string);
    AdplugWrap(const AdplugWrap &) = delete;
    AdplugWrap &operator=(const AdplugWrap &) = delete;

    static std::string version() { return CAdPlug::get_version(); }

    int rate() { return 44100; }
    int depth() { return 16; }
    int channels() { return 1; }

    unsigned long length() { return player->songlength(); }
    void seek(unsigned long pos) { player->seek(pos); }

    Frame read();

    std::string title() { return player->gettitle(); }
    std::string format() { return player->gettype(); }
    std::string author() { return player->getauthor(); }
    std::string description() { return player->getdesc(); }
    unsigned int pattern_count() { return player->getpatterns(); }
    unsigned int instrument_count() { return player->getinstruments(); }

    std::vector<std::string> instruments();

  private:
    std::unique_ptr<Copl> opl;
    std::unique_ptr<CPlayer> player;
    short buf[16384];
    size_t remaining = 0;
};

#endif
