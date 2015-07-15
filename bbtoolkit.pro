TEMPLATE = vcapp
DESTDIR  = $$PWD/bin
HEADERS += $$PWD/include/*.h*
HEADERS += $$PWD/../nsengine/include/*.h
SOURCES += $$PWD/src/*.c*
CONFIG += qt
CONFIG += debug_and_release
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../nsengine/include
MOC_DIR = $$PWD/build/moc
OBJECTS_DIR = $$PWD/build/obj
RCC_DIR = $$PWD/build/res
UI_DIR = $$PWD/ui
FORMS += $$PWD/ui/*.ui
RESOURCES = $$PWD/toolkit.qrc
CONFIG
QT += opengl

CONFIG(debug, debug|release){
LIBS += "F:\Users\DreamInColor\My Documents\EmacsProjects\nsengine\lib\nsengined.lib"
}

CONFIG(release, debug|release){
LIBS += "F:\Users\DreamInColor\My Documents\EmacsProjects\nsengine\lib\nsengine.lib"
}
