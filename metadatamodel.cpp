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

#include <QHash>
#include <QLatin1Char>
#include <QObject>
#include <QString>

#include <qmmp/metadatamodel.h>

#include "metadatamodel.h"

AdplugMetaDataModel::AdplugMetaDataModel(const QString &path, QObject *parent) :
  MetaDataModel(parent)
{
  try
  {
    AdplugWrap adplug(path.toUtf8().constData());
    fill_in_audio_properties(adplug);
    fill_in_descriptions(adplug);
  }
  catch(AdplugWrap::InvalidFile)
  {
  }
}

void AdplugMetaDataModel::fill_in_audio_properties(AdplugWrap &adplug)
{
  if(adplug.instrument_count() != 0)
  {
    QString text;

    for(const std::string &s : adplug.instruments())
    {
      text += QString::fromStdString(s) + "\n";
    }
    desc.insert(tr("Instruments"), text);
  }
}

void AdplugMetaDataModel::fill_in_descriptions(AdplugWrap &adplug)
{
  ap.insert(tr("Title"), QString::fromStdString(adplug.title()));
  ap.insert(tr("Format"), QString::fromStdString(adplug.format()));
  ap.insert(tr("Author"), QString::fromStdString(adplug.author()));
  ap.insert(tr("Description"), QString::fromStdString(adplug.author()));
  if(adplug.pattern_count() != 0)
  {
    ap.insert(tr("Patterns"), QString::number(adplug.pattern_count()));
  }
  if(adplug.instrument_count() != 0)
  {
    ap.insert(tr("Instruments"), QString::number(adplug.instrument_count()));
  }
}

QHash<QString, QString> AdplugMetaDataModel::audioProperties()
{
  return ap;
}

QHash<QString, QString> AdplugMetaDataModel::descriptions()
{
  return desc;
}
