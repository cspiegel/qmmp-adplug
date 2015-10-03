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

#ifndef QMMP_ADPLUG_DECODERFACTORY_H
#define QMMP_ADPLUG_DECODERFACTORY_H

#include <QList>
#include <QIODevice>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTranslator>
#include <QWidget>

#include <qmmp/decoder.h>
#include <qmmp/decoderfactory.h>
#include <qmmp/qmmp.h>

class AdplugDecoderFactory : public QObject, DecoderFactory
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qmmp.qmmp.DecoderFactoryInterface.1.0")
  Q_INTERFACES(DecoderFactory)

  public:
    bool supports(const QString &) const;
    bool canDecode(QIODevice *) const;
    const DecoderProperties properties() const;
    Decoder *create(const QString &, QIODevice *);
    QList<FileInfo *> createPlayList(const QString &, bool, QStringList *);
    MetaDataModel *createMetaDataModel(const QString &, QObject * = 0);
    void showSettings(QWidget *);
    void showAbout(QWidget *);
    QTranslator *createTranslator(QObject *);
};

#endif
