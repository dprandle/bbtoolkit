TEMPLATE = app
DESTDIR  = $$PWD/bin
HEADERS += $$PWD/include/*.h*
SOURCES += $$PWD/src/*.c*
CONFIG += qt debug_and_release c++14
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../nsengine/include/nsengine
INCLUDEPATH += $$PWD/../nsengine/deps/glew-1.12.0/include
MOC_DIR = $$PWD/build/moc
OBJECTS_DIR = $$PWD/build/obj
RCC_DIR = $$PWD/build/res
UI_DIR = $$PWD/ui
FORMS += $$PWD/ui/*.ui
RESOURCES = $$PWD/toolkit.qrc
QT += opengl
QMAKE_CXXFLAGS += -pipe

CONFIG(debug, debug|release){
DEFINES += NSDEBUG
LIBS += $$PWD/../nsengine/lib/x64/libnsengined_wdep.a
LIBS += -L$$PWD/../nsengine/deps/assimp-3.1.1/lib/x64
LIBS += -L$$PWD/../nsengine/deps/devil-1.7.8/lib/x64
LIBS += -lILd -lassimpd
}

CONFIG(release, debug|release){
LIBS += $$PWD/../nsengine/lib/x64/libnsengine_wdep.a
LIBS += -L$$PWD/../nsengine/deps/assimp-3.1.1/lib/x64
LIBS += -L$$PWD/../nsengine/deps/devil-1.7.8/lib/x64
LIBS += -lIL -lassimp
}
