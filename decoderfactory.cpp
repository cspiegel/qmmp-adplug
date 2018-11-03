/*-
 * Copyright 2014-2015 Chris Spiegel.
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

#include <QFile>
#include <QIODevice>
#include <QList>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QTranslator>
#include <QtPlugin>

#include <qmmp/fileinfo.h>
#include <qmmp/qmmp.h>

#include "decoderfactory.h"
#include "adplugwrap.h"
#include "decoder.h"
#include "magic.h"
#include "metadatamodel.h"

class InputStreamQIO : public InputStream
{
  public:
    explicit InputStreamQIO(QIODevice *device) : device(device) { }

    bool read(std::size_t offset, void *buf, std::size_t n)
    {
      qint64 ret;

      if(!device->seek(offset)) return false;

      ret = device->read(reinterpret_cast<char *>(buf), n);

      return ret > 0 && ret == (qint64)n;
    }

    size_t size()
    {
      if(device->isSequential() || !device->isOpen()) throw InputStream::NoRandomAccess();

      return device->size();
    }

  private:
    QIODevice *device;
};

bool AdplugDecoderFactory::canDecode(QIODevice *device) const
{
  return adplug_supports(InputStreamQIO(device));
}

const DecoderProperties AdplugDecoderFactory::properties() const
{
  DecoderProperties properties;

  properties.name = tr("AdPlug Plugin");
  properties.filters << "*.adl" << "*.hsc" << "*.ksm" << "*.lds";
  properties.description = tr("AdPlug (Adlib) Files");
  properties.shortName = "cas-adplug";
  properties.hasAbout = true;
  properties.hasSettings = false;
  properties.noInput = true;
  properties.protocols << "file";

  return properties;
}

Decoder *AdplugDecoderFactory::create(const QString &path, QIODevice *)
{
  return new AdplugDecoder(path);
}

QList<FileInfo *> AdplugDecoderFactory::createPlayList(const QString &filename, bool, QStringList *)
{
  QList<FileInfo *> list;

  try
  {
    AdplugWrap adplug(filename.toUtf8().constData());
    FileInfo *file_info = new FileInfo(filename);

    file_info->setLength(adplug.length() / 1000);

    list << file_info;
  }
  catch(const AdplugWrap::InvalidFile &)
  {
  }

  return list;
}

MetaDataModel *AdplugDecoderFactory::createMetaDataModel(const QString &path, QObject *parent)
{
  return new AdplugMetaDataModel(path, parent);
}

void AdplugDecoderFactory::showSettings(QWidget *)
{
}

void AdplugDecoderFactory::showAbout(QWidget *parent)
{
  QString title = tr("About Adplug Audio Plugin");
  QString text = tr("Adplug Audio Plugin");
  text += "\n";
  text += tr("Written by: Chris Spiegel <cspiegel@gmail.com>");
  QMessageBox::about(parent, title, text);
}

QTranslator *AdplugDecoderFactory::createTranslator(QObject *)
{
  return nullptr;
}
