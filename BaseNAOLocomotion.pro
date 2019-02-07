TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

QT += core

DESTDIR = bin
CONFIG += c++11

DEFINES += NON_MATLAB_PARSING \
           MAX_EXT_API_CONNECTIONS=255 \
           _GLIBCXX_USE_CXX11_ABI=0 \
#           TEST_DEPLOY

QMAKE_CFLAGS += -m64 -g

QMAKE_CXXFLAGS += $(QMAKE_CFLAGS) \
                    -D__linux \
                    -fpermissive

# Source files
INCLUDEPATH *= . include remoteApi include/utils

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

LIBS += -lm \
        -L. \
        -lga \
        -lpthread \
        -lboost_iostreams \
        -lboost_system \
        -lboost_filesystem

SOURCES += \
    joint.cpp \
    jointPair.cpp \
    main.cpp \
    NAO.cpp \
    robot.cpp \
    shape.cpp \
    VRepClass.cpp \
    remoteApi/extApi.c \
    remoteApi/extApiPlatform.c \
    include/utils/gnuplot/gnuplot.cc \
    test.cc \
    include/utils/utils.cc \
    include/utils/vrep/vrepwrapper.cc

HEADERS += \
    include/_dirent.h \
    include/luaFunctionData.h \
    include/luaFunctionDataItem.h \
    include/scriptFunctionData.h \
    include/scriptFunctionDataItem.h \
    include/socketInConnection.h \
    include/socketOutConnection.h \
    include/v_repConst.h \
    include/v_repLib.h \
    include/v_repTypes.h \
    remoteApi/extApi.h \
    remoteApi/extApiInternal.h \
    remoteApi/extApiPlatform.h \
    helpers.hpp \
    joint.hpp \
    jointPair.hpp \
    NAO.hpp \
    robot.hpp \
    shape.hpp \
    VRepClass.hpp \
    include/utils/printcolors.hpp \
    include/utils/gnuplot/gnuplot-iostream.h \
    include/utils/printcolors.hpp \
    include/utils/gnuplot/gnuplot.hh \
    include/utils/utils.hh \
    include/utils/vrep/vrepwrapper.hh
