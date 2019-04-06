HEADERS += decoderfactory.h decoder.h metadatamodel.h adplugwrap.h
SOURCES += decoder.cpp decoderfactory.cpp metadatamodel.cpp magic.cpp adplugwrap.cpp

CONFIG += warn_on plugin link_pkgconfig c++11

TEMPLATE = lib

QMAKE_CLEAN += lib$${TARGET}.so

unix {
  CONFIG += link_pkgconfig
  PKGCONFIG += qmmp adplug

  QMMP_PREFIX = $$system(pkg-config qmmp --variable=prefix)
  PLUGIN_DIR = $$system(pkg-config qmmp --variable=plugindir)/Input
  INCLUDEPATH += $${QMMP_PREFIX}/include

  plugin.path = $${PLUGIN_DIR}
  plugin.files = lib$${TARGET}.so
  INSTALLS += plugin
}
