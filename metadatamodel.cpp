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

AdplugMetaDataModel::AdplugMetaDataModel(const QString &path) :
  MetaDataModel(true)
{
  try
  {
    AdplugWrap adplug(path.toUtf8().constData());
    fill_in_extra_properties(adplug);
    fill_in_descriptions(adplug);
  }
  catch(const AdplugWrap::InvalidFile &)
  {
  }
}

void AdplugMetaDataModel::fill_in_extra_properties(AdplugWrap &adplug)
{
  if(adplug.instrument_count() != 0)
  {
    QString text;

    for(const std::string &s : adplug.instruments())
    {
      text += QString::fromStdString(s) + "\n";
    }
    desc << MetaDataItem(tr("Instruments"), text);
  }
}

void AdplugMetaDataModel::fill_in_descriptions(AdplugWrap &adplug)
{
  ap << MetaDataItem(tr("Title"), QString::fromStdString(adplug.title()));
  ap << MetaDataItem(tr("Title"), QString::fromStdString(adplug.title()));
  ap << MetaDataItem(tr("Format"), QString::fromStdString(adplug.format()));
  ap << MetaDataItem(tr("Author"), QString::fromStdString(adplug.author()));
  ap << MetaDataItem(tr("Description"), QString::fromStdString(adplug.author()));
  if(adplug.pattern_count() != 0)
  {
    ap << MetaDataItem(tr("Patterns"), QString::number(adplug.pattern_count()));
  }
  if(adplug.instrument_count() != 0)
  {
    ap << MetaDataItem(tr("Instruments"), QString::number(adplug.instrument_count()));
  }
}

QList<MetaDataItem> AdplugMetaDataModel::extraProperties() const
{
  return ap;
}

QList<MetaDataItem> AdplugMetaDataModel::descriptions() const
{
  return desc;
}
