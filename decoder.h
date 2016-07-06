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

#ifndef QMMP_ADPLUG_DECODER_H
#define QMMP_ADPLUG_DECODER_H

#include <memory>

#include <QString>
#include <QtGlobal>

#include <qmmp/decoder.h>

#include <adplug/adplug.h>
#include <adplug/emuopl.h>

#include "adplugwrap.h"

class AdplugDecoder : public Decoder
{
  public:
    explicit AdplugDecoder(const QString &);

    bool initialize();
    qint64 totalTime();
    int bitrate();
    qint64 read(unsigned char *, qint64);
    void seek(qint64);

  private:
    qint64 copy(unsigned char *, qint64);

    QString path;
    std::unique_ptr<AdplugWrap> adplug;
    unsigned char *bufptr = nullptr;
    qint64 buf_filled = 0;
    unsigned long time = 0;
    unsigned long length = 0;
    double divisor = 0.0;
};

#endif
