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

#ifndef QMMP_ADPLUG_METADATAMODEL_H
#define QMMP_ADPLUG_METADATAMODEL_H

#include <QObject>
#include <QString>
#include <QHash>

#include <qmmp/metadatamodel.h>

#include "adplugwrap.h"

class AdplugMetaDataModel : public MetaDataModel
{
  Q_OBJECT

  public:
    AdplugMetaDataModel(const QString &, QObject *);
    QHash<QString, QString> audioProperties();
    QHash<QString, QString> descriptions();

  private:
    void fill_in_audio_properties(AdplugWrap &);
    void fill_in_descriptions(AdplugWrap &);

    QHash<QString, QString> ap;
    QHash<QString, QString> desc;
};

#endif
