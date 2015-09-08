/*-
 * Copyright 2014 Chris Spiegel.
 *
 * This file is part of qmmp-adplug.
 *
 * qmmp-adplug is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version
 * 2 or 3, as published by the Free Software Foundation.
 *
 * qmmp-adplug is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with qmmp-adplug.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>

#include <QString>
#include <QtGlobal>

#include <qmmp/decoder.h>

#include "decoder.h"
#include "adplugwrap.h"

AdplugDecoder::AdplugDecoder(const QString &path)
        : Decoder(),
          path(path)
{
}

bool AdplugDecoder::initialize()
{
  try
  {
    adplug = std::unique_ptr<AdplugWrap>(new AdplugWrap(path.toUtf8().constData()));
  }
  catch(AdplugWrap::InvalidFile)
  {
    return false;
  }

  configure(adplug->rate(), adplug->channels(), Qmmp::PCM_S16LE);

  length = adplug->length();
  divisor = (adplug->rate() * adplug->channels() * (adplug->depth() / 8)) / 1000.0;

  return true;
}

qint64 AdplugDecoder::totalTime()
{
  return adplug->length();
}

int AdplugDecoder::bitrate()
{
  return adplug->depth();
}

qint64 AdplugDecoder::read(char *audio, qint64 max_size)
{
  qint64 copied;

  copied = copy(audio, max_size);
  audio += copied;
  max_size -= copied;

  /* Some songs loop endlessly.  If we pass the length threshold (Adplug
   * caps the reported length at 10 minutes), then report EOF.
   */
  if(time > length) return 0;

  if(buf_filled == 0)
  {
    AdplugWrap::Frame frame = adplug->read();

    if(frame.n == 0) return copied;

    bufptr = frame.buf;
    buf_filled += frame.n;
  }

  copied += copy(audio, max_size);

  time += copied / divisor;

  return copied;
}

qint64 AdplugDecoder::copy(char *audio, qint64 max_size)
{
  qint64 to_copy;

  to_copy = qMin(buf_filled, max_size);

  memcpy(audio, bufptr, to_copy);

  bufptr += to_copy;
  buf_filled -= to_copy;

  return to_copy;
}

void AdplugDecoder::seek(qint64 pos)
{
  adplug->seek(pos);
  time = pos;
}
