HEADERS += decoderfactory.h decoder.h metadatamodel.h adplugwrap.h
SOURCES += decoder.cpp decoderfactory.cpp metadatamodel.cpp magic.cpp adplugwrap.cpp

CONFIG += warn_on plugin link_pkgconfig c++11

TEMPLATE = lib

QMAKE_CLEAN += lib$${TARGET}.so

QT += widgets

unix {
  CONFIG += link_pkgconfig

  equals(QT_MAJOR_VERSION, 5) {
    QMMP = qmmp-1
  } else: equals(QT_MAJOR_VERSION, 6) {
    QMMP = qmmp
  } else {
    error("Unsupported Qt version: 5 or 6 is required")
  }

  PKGCONFIG += $${QMMP} adplug

  QMMP_PREFIX = $$system(pkg-config $${QMMP} --variable=prefix)
  PLUGIN_DIR = $$system(pkg-config $${QMMP} --variable=plugindir)/Input
  LOCAL_INCLUDES = $${QMMP_PREFIX}/include
  LOCAL_INCLUDES -= $$QMAKE_DEFAULT_INCDIRS
  INCLUDEPATH += $$LOCAL_INCLUDES

  plugin.path = $${PLUGIN_DIR}
  plugin.files = lib$${TARGET}.so
  INSTALLS += plugin
}
