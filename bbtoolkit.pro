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
    src/toolkit.cpp \
    src/select_res_dialog.cpp \
    src/mesh_widget.cpp \
    src/import_resources_dialog.cpp \
    src/edit_submesh_data_widget.cpp \
    src/material_widget.cpp \
    src/entity_widget.cpp \
    src/render_comp_widget.cpp \
    src/light_comp_widget.cpp \
    src/occupy_comp_widget.cpp \
    src/tform_comp_widget.cpp \
    src/cam_comp_widget.cpp \
    src/sel_comp_widget.cpp \
    src/tile_comp_widget.cpp \
    src/tile_brush_comp_widget.cpp \
    src/particle_comp_widget.cpp \
    src/terrain_comp_widget.cpp \
    src/anim_comp_widget.cpp \
    src/component_widget.cpp \
    src/add_comp_dialog.cpp \
    src/component_selection_widget.cpp

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
    include/toolkitdef.h \
    include/select_res_dialog.h \
    include/mesh_widget.h \
    include/import_resources_dialog.h \
    include/edit_submesh_data_widget.h \
    include/material_widget.h \
    include/entity_widget.h \
    include/render_comp_widget.h \
    include/light_comp_widget.h \
    include/occupy_comp_widget.h \
    include/tform_comp_widget.h \
    include/cam_comp_widget.h \
    include/sel_comp_widget.h \
    include/tile_comp_widget.h \
    include/tile_brush_comp_widget.h \
    include/particle_comp_widget.h \
    include/terrain_comp_widget.h \
    include/anim_comp_widget.h \
    include/component_widget.h \
    include/add_comp_dialog.h \
    include/component_selection_widget.h

FORMS += \
    ui/transformcompwidget.ui \
    ui/toolkit.ui \
    ui/tileview.ui \
    ui/tilecompwidget.ui \
    ui/tilebrushcompwidget.ui \
    ui/texture_widget.ui \
    ui/selecttiledialog.ui \
    ui/select_res_dialog.ui \
    ui/selcompwidget.ui \
    ui/resourceinfowidget.ui \
    ui/resource_dialog_prev_lighting.ui \
    ui/resource_dialog_prev.ui \
    ui/resource_dialog.ui \
    ui/resource_browser.ui \
    ui/rendercompwidget.ui \
    ui/previewwidget.ui \
    ui/plugindetailsdialog.ui \
    ui/particlecompwidget.ui \
    ui/occupycompwidget.ui \
    ui/objectview.ui \
    ui/newtile.ui \
    ui/newresourcedialog.ui \
    ui/newplugindialog.ui \
    ui/newmap.ui \
    ui/newfilterdialog.ui \
    ui/materialwidget.ui \
    ui/materialdialog.ui \
    ui/manage_plugins_dialog.ui \
    ui/manage_maps_dialog.ui \
    ui/lightcompwidget.ui \
    ui/inputcompwidget.ui \
    ui/entityeditordialog.ui \
    ui/editplugindialog.ui \
    ui/editmapdialog.ui \
    ui/camerasettings.ui \
    ui/camcompwidget.ui \
    ui/brushmenuwidget.ui \
    ui/animcompwidget.ui \
    ui/addnewbrushdialog.ui \
    ui/mesh_widget.ui \
    ui/import_resources_dialog.ui \
    ui/edit_submesh_data_widget.ui \
    ui/material_widget.ui \
    ui/entity_widget.ui \
    ui/render_comp_widget.ui \
    ui/light_comp_widget.ui \
    ui/occupy_comp_widget.ui \
    ui/tform_comp_widget.ui \
    ui/cam_comp_widget.ui \
    ui/sel_comp_widget.ui \
    ui/tile_comp_widget.ui \
    ui/tile_brush_comp_widget.ui \
    ui/particle_comp_widget.ui \
    ui/terrain_comp_widget.ui \
    ui/anim_comp_widget.ui \
    ui/add_comp_dialog.ui \
    ui/component_selection_widget.ui

CONFIG += qt debug_and_release c++11
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../nsengine/include/nsengine
INCLUDEPATH += $$PWD/../nsengine/include/nsengine/asset
INCLUDEPATH += $$PWD/../nsengine/include/nsengine/component
INCLUDEPATH += $$PWD/../nsengine/include/nsengine/nsmath
INCLUDEPATH += $$PWD/../nsengine/include/nsengine/opengl
INCLUDEPATH += $$PWD/../nsengine/include/nsengine/system

UI_DIR = $$PWD/ui
RESOURCES = $$PWD/toolkit.qrc
QT += opengl gui
ARCH = x64

DEFINES += GLEW_STATIC GLEW_MX

unix {
QMAKE_CXXFLAGS += -pipe
}

win32 {
!contains(QMAKE_TARGET.arch, x86_64) {
    ARCH = x86
} else {
    ARCH = x64
}
system(\"$$PWD/config.bat\" \"$$PWD\")
}

LIBS += -L$$PWD/../nsengine/lib/$$ARCH

DEFINES += NSDEBUG

CONFIG(debug, debug|release){

win32 {
LIBS += -lnsengined -lnsmathd -lassimp-vc140-mt -lglfw3ddll 
LIBS += -llibsndfile-1 -lOpenAL32 -lDbgHelp -lfreetyped -lOpenGL32
}

unix {
LIBS += -lnsengined -lnsmathd -lassimp -lglfwd -lsndfile -lopenal -lfreetyped -lGL
QMAKE_CXXFLAGS += -O0
}

DCONFIG = Debug
TARGET = bbtoolkitd

}

CONFIG(release, debug|release){

win32 {
LIBS += -lnsengine -lnsmath -lassimp-vc140-mt -lglfw3dll 
LIBS += -llibsndfile-1 -lOpenAL32 -lDbgHelp -lfreetype -lOpenGL32
}

unix {
LIBS += -lnsengine -lnsmath -lassimp -lglfw -lsndfile -lopenal -lfreetype -lGL
}

DCONFIG = Release
TARGET = bbtoolkit
}

DESTDIR  = $$PWD/bin/$$ARCH
MOC_DIR = $$PWD/build/$$ARCH/$$DCONFIG/moc
OBJECTS_DIR = $$PWD/build/$$ARCH/$$DCONFIG/obj
RCC_DIR = $$PWD/build/$$ARCH/$$DCONFIG/res
