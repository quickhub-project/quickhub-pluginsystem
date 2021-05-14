TEMPLATE = lib
DESTDIR = ../bin/lib
#QMAKE_LFLAGS += -Wl,-rpath,"'$$ORIGIN'"
HEADERS += \
    src/PluginManager.h \
    src/IPlugin.h \
    src/QHPluginSystem_global.h

SOURCES += \
    src/PluginManager.cpp \
    src/IPlugin.cpp


INSTALLS += target
target.path = /usr/lib
