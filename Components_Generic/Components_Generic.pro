
QT += core gui network opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += COMPONENTS_GENERIC_LIB
INCLUDEPATH += $$PWD/../Quick3D/Quick3D/Source
INCLUDEPATH += $$PWD/../Quick3D/qt-plus/source/cpp
INCLUDEPATH += $$PWD/../Quick3D/qt-plus/source/cpp/Web
DEPENDPATH += $$PWD/../Quick3D/Quick3D
DESTDIR = $$PWD/../bin/Plugins

# C++ Flags
QMAKE_CXXFLAGS += -Wno-invalid-offsetof
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-reorder

# Target
CONFIG(debug, debug|release) {
    TARGET = Components_Genericd
} else {
    TARGET = Components_Generic
}

# Libraries
CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../Quick3D/Quick3D/bin/ -lQuick3Dd
} else {
    LIBS += -L$$PWD/../Quick3D/Quick3D/bin/ -lQuick3D
}

CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../Quick3D/qt-plus/bin/ -lqt-plusd
} else {
    LIBS += -L$$PWD/../Quick3D/qt-plus/bin/ -lqt-plus
}

# Code
HEADERS += \
    Source/Components_Generic_Global.h \
    Source/Components_Generic.h \
    Source/Constants.h \
    Source/CNavaidDatabase.h

SOURCES += \
    Source/Components_Generic.cpp \
    Source/CNavaidDatabase.cpp

# Copy qt-plus to bin
copyfile = $$PWD/../Quick3D/qt-plus/bin/*.dll
copydest = $$PWD/../bin

QMAKE_PRE_LINK += $$QMAKE_COPY $$quote($$shell_path($$copyfile)) $$quote($$shell_path($$copydest)) $$escape_expand(\\n\\t)

# Copy Quick3D to Binary
copyfile = $$PWD/../Quick3D/Quick3D/bin/*.dll
copydest = $$PWD/../bin

QMAKE_PRE_LINK += $$QMAKE_COPY $$quote($$shell_path($$copyfile)) $$quote($$shell_path($$copydest)) $$escape_expand(\\n\\t)