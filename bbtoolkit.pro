TEMPLATE = app
HEADERS += $$PWD/include/*.h*
SOURCES += $$PWD/src/*.c*
CONFIG += qt debug_and_release
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../nsengine/include/nsengine
INCLUDEPATH += $$PWD/../nsengine/include
UI_DIR = $$PWD/ui
FORMS += $$PWD/ui/*.ui
RESOURCES = $$PWD/toolkit.qrc
QT += opengl gui

ARCH = x64

DEFINES += GLEW_STATIC GLEW_MX

unix {
QMAKE_CXXFLAGS += -pipe -O0
}

win32 {
    !contains(QMAKE_TARGET.arch, x86_64) {
        ARCH = x86
    } else {
        ARCH = x64
    }
    system(\"$$PWD/config.bat\" \"$$PWD\")
}

CONFIG(debug, debug|release){
DEFINES += NSDEBUG
LIBS += -L$$PWD/../nsengine/lib/$$ARCH
LIBS += -L$$PWD/../nsengine/deps/assimp-3.1.1/lib/$$ARCH
LIBS += -L$$PWD/../nsengine/deps/devil-1.7.8/lib/$$ARCH
LIBS += -lOpenGL32 -lnsengined -lIL -lassimpd
DCONFIG = Debug
TARGET = bbtoolkitd
}

CONFIG(release, debug|release){
LIBS += -L$$PWD/../nsengine/lib/$$ARCH
LIBS += -L$$PWD/../nsengine/deps/assimp-3.1.1/lib/$$ARCH
LIBS += -L$$PWD/../nsengine/deps/devil-1.7.8/lib/$$ARCH
LIBS += -lOpenGL32 -lnsengine -lIL -lassimp
DCONFIG = Release
TARGET = bbtoolkit
}

DESTDIR  = $$PWD/bin/$$ARCH
MOC_DIR = $$PWD/build/$$ARCH/$$DCONFIG/moc
OBJECTS_DIR = $$PWD/build/$$ARCH/$$DCONFIG/obj
RCC_DIR = $$PWD/build/$$ARCH/$$DCONFIG/res
