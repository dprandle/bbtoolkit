TEMPLATE = app
DESTDIR  = $$PWD/bin
HEADERS += $$PWD/include/*.h*
SOURCES += $$PWD/src/*.c*
CONFIG += qt debug_and_release
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../nsengine/include/nsengine
INCLUDEPATH += $$PWD/../nsengine/deps/glew-1.12.0/include
MOC_DIR = $$PWD/build/moc
OBJECTS_DIR = $$PWD/build/obj
RCC_DIR = $$PWD/build/res
UI_DIR = $$PWD/ui
FORMS += $$PWD/ui/*.ui
RESOURCES = $$PWD/toolkit.qrc
QT += opengl gui

DEFINES += GLEW_STATIC GLEW_MX

unix {
QMAKE_CXXFLAGS += -pipe -O0
}

CONFIG(debug, debug|release){
DEFINES += NSDEBUG
LIBS += -L$$PWD/../nsengine/deps/glew-1.12.0/lib/x64
LIBS += -L$$PWD/../nsengine/deps/soil-1.16.0/lib/x64
LIBS += -L$$PWD/../nsengine/deps/hashlib-1.0.0/lib/x64
LIBS += -L$$PWD/../nsengine/lib/x64
LIBS += -L$$PWD/../nsengine/deps/assimp-3.1.1/lib/x64
LIBS += -L$$PWD/../nsengine/deps/devil-1.7.8/lib/x64
LIBS += -lOpenGL32 -lglew32mxsd -lnsengined -lsoilsd -lhashlibsd -lIL -lassimpd
}

CONFIG(release, debug|release){
LIBS += -L$$PWD/../nsengine/deps/soil-1.16.0/lib/x64
LIBS += -L$$PWD/../nsengine/deps/hashlib-1.0.0/lib/x64
LIBS += -L$$PWD/../nsengine/deps/glew-1.12.0/lib/x64
LIBS += -L$$PWD/../nsengine/lib/x64
LIBS += -L$$PWD/../nsengine/deps/assimp-3.1.1/lib/x64
LIBS += -L$$PWD/../nsengine/deps/devil-1.7.8/lib/x64
LIBS += -lOpenGL32 -lglew32mxs -lnsengine -lsoils -lhashlibs -lIL -lassimp
}
