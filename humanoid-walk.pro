QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           NON_MATLAB_PARSING \
           MAX_EXT_API_CONNECTIONS=255 \
           _GLIBCXX_USE_CXX11_ABI=0 \

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = bin
CONFIG += c++11

QMAKE_CFLAGS += -m64

QMAKE_CXXFLAGS += $(QMAKE_CFLAGS) \
                    -D__linux \
                    -fpermissive

# Source files
INCLUDEPATH *= $$PWD $$PWD/src $$PWD/src/robot $$PWD/src/utils $$PWD/src $$PWD/remoteApi

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

LIBS *= -lm \
        -lga \
        -L. \
        -lpthread \
        -lboost_iostreams \
        -lboost_system \
        -lboost_filesystem

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

HEADERS += \
    remoteApi/_dirent.h \
    remoteApi/extApi.h \
    remoteApi/extApiInternal.h \
    remoteApi/extApiPlatform.h \
    remoteApi/luaFunctionData.h \
    remoteApi/luaFunctionDataItem.h \
    remoteApi/scriptFunctionData.h \
    remoteApi/scriptFunctionDataItem.h \
    remoteApi/socketInConnection.h \
    remoteApi/socketOutConnection.h \
    remoteApi/v_repConst.h \
    remoteApi/v_repLib.h \
    remoteApi/v_repTypes.h \
    src/robot/joint.hpp \
    src/robot/jointPair.hpp \
    src/robot/NAO.hpp \
    src/robot/robot.hpp \
    src/robot/shape.hpp \
    src/utils/gnuplot/gnuplot-iostream.h \
    src/utils/vrep/vrep.hh \
    src/utils/vrep/vrepwrapper.hh \
    src/utils/printcolors.hpp \
    src/utils/utils.hh \
    src/helpers.hpp \
    src/VRepClass.hpp \
    src/simulationwrapper.hh

SOURCES += \
    src/simulationwrapper.cc \
    src/robot/joint.cpp \
    src/robot/jointPair.cpp \
    src/robot/NAO.cpp \
    src/robot/robot.cpp \
    src/robot/shape.cpp \
    src/utils/vrep/vrep.cc \
    src/utils/vrep/vrepwrapper.cc \
    src/utils/utils.cc \
    src/VRepClass.cpp \
    main.cpp \
    remoteApi/extApi.c \
    remoteApi/extApiPlatform.c
