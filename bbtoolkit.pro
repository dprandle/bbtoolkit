TEMPLATE = app

SOURCES += \
    src/addnewbrushdialog.cpp \
    src/brushgraphicsview.cpp \
    src/brushmenuwidget.cpp \
    src/camerasettingsdialog.cpp \
    src/clickableframe.cpp \
    src/componentwidgets.cpp \
    src/entityeditordialog.cpp \
    src/listwidgetcust.cpp \
    src/main.cpp \
    src/managemapsdialog.cpp \
    src/managepluginsdialog.cpp \
    src/mapview.cpp \
    src/materialwidget.cpp \
    src/newmapdialog.cpp \
    src/newresourcedialog.cpp \
    src/newtiledialog.cpp \
    src/objectview.cpp \
    src/outputview.cpp \
    src/preview.cpp \
    src/previewwidget.cpp \
    src/resource_browser.cpp \
    src/resource_dialog.cpp \
    src/resource_dialog_prev.cpp \
    src/resource_dialog_prev_lighting.cpp \
    src/resourceinfowidget.cpp \
    src/texture_widget.cpp \
    src/tileview.cpp \
    src/toolkit.cpp

HEADERS += \
    include/addnewbrushdialog.h \
    include/brushgraphicsview.h \
    include/brushmenuwidget.h \
    include/camerasettingsdialog.h \
    include/clickableframe.h \
    include/componentwidgets.h \
    include/entityeditordialog.h \
    include/listwidgetcust.h \
    include/managemapsdialog.h \
    include/managepluginsdialog.h \
    include/mapview.h \
    include/materialwidget.h \
    include/newmapdialog.h \
    include/newresourcedialog.h \
    include/newtiledialog.h \
    include/objectview.h \
    include/outputview.h \
    include/preview.h \
    include/previewwidget.h \
    include/resource_browser.h \
    include/resource_dialog.h \
    include/resource_dialog_prev.h \
    include/resource_dialog_prev_lighting.h \
    include/resource1.h \
    include/resourceinfowidget.h \
    include/resourcewidget.h \
    include/texture_widget.h \
    include/tileview.h \
    include/toolkit.h \
    include/toolkitdef.h

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
