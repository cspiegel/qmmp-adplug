#include <cstddef>
#include <string>
#include <vector>

#include <adplug/adplug.h>
#include <adplug/emuopl.h>

#include "adplugwrap.h"

AdplugWrap::AdplugWrap(std::string filename) :
  opl(new CEmuopl(rate(), true, false)),
  player(CAdPlug::factory(filename.c_str(), opl.get()))
{
  if(!player) throw InvalidFile();
}

AdplugWrap::Frame AdplugWrap::read()
{
  size_t to_write;
  size_t bufsiz = sizeof(buf) / sizeof(*buf);

  if(remaining == 0)
  {
    if(!player->update()) return Frame(0, nullptr);
    remaining = rate() / player->getrefresh();
  }

  if(remaining > bufsiz) to_write = bufsiz;
  else                   to_write = remaining;

  opl->update(buf, to_write);

  remaining -= to_write;

  return Frame(to_write * 2, reinterpret_cast<unsigned char *>(buf));
}

std::vector<std::string> AdplugWrap::instruments()
{
  std::vector<std::string> insts;

  for(unsigned int i = 0; i < instrument_count(); i++)
  {
    insts.push_back(player->getinstrument(i));
  }

  return insts;
}
