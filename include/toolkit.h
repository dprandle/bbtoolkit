/*!
\file toolkit.h

\brief Header file for Toolkit class

This file contains all of the neccessary declarations for the Toolkit class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/

#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <QMainWindow>
#include <ui_toolkit.h>
#include <qdir.h>
#include <qlist.h>

#define LAYER_ABOVE_TEXT "Above"
#define LAYER_BELOW_TEXT "Below"
#define LAYER_ALL_TEXT "All Except"
#define LAYER_ONLY_TEXT "Only"


class QMessageBox;
class MapView;
class nsengine;
class OutputView;
class QSpinBox;
class QCheckBox;
class QLabel;
class QMenu;
class QIcon;
class QToolButton;
class BrushMenuWidget;
class CameraSettingsDialog;
class TileView;
class ObjectView;
class EntityEditorDialog;
class resource_browser;
class QComboBox;

class resource_dialog;
class resource_dialog_prev;
class resource_dialog_prev_lighting;

class Toolkit : public QMainWindow
{
	Q_OBJECT

public:
    Toolkit(QWidget *parent = 0);
	~Toolkit();

	void closeEvent(QCloseEvent *pEvent);

    MapView * map_view();

    OutputView * output_view();

    ObjectView * object_view();

    TileView *  tile_view();

    CameraSettingsDialog * camera_settings();

    resource_browser * res_browser();

    resource_dialog * res_dialog();

    resource_dialog_prev * res_dialog_prev();

    resource_dialog_prev_lighting * res_dialog_prev_lighting();

    void load_plugin_files(const QDir & startingDir);

    void refresh_views();

    void update_brush_tool_button();

	void init();

    void update_ui();

    static Toolkit & inst();

public slots:

    void on_actionSelect_triggered(bool);
    void on_actionSelectArea_triggered(bool);
    void on_actionTileBrush_triggered(bool);
    void on_actionObjectBrush_triggered(bool);
    void on_actionEraserBrush_triggered(bool);
    void on_actionNew_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionUnlock_toggled(bool);
    void on_actionSwapTiles_triggered();
    void on_actionDeleteSelection_triggered();
    void on_actionNewBrush_triggered();
    void on_actionLayerMode_toggled(bool);
    void on_actionStampMode_toggled(bool);
    void on_actionHideSelection_triggered();
    void on_actionUnhideSelection_triggered();
    void on_actionCameraCenter_triggered(bool);
    void on_actionFrontView_triggered();
    void on_actionObjectCenter_triggered(bool);
    void on_actionIsoView_triggered();
    void on_actionTopDown_triggered();
    void on_actionRedo_triggered();
    void on_actionUndo_triggered();
    void on_actionSwitchMap_triggered();
    void on_actionCameraSettings_triggered();
    void on_actionShowAllHidden_toggled(bool);
    void on_actionMirrorMode_toggled(bool);
    void on_actionToggleLighting_toggled(bool);
    void on_actionResource_Browser_triggered();


    void on_mirror_center_change();
    void on_change_layer(int);
    void on_brush_height_change(int);
    void on_brush_double_click();
    void on_set_current_brush(nsentity *);
    void on_brush_change(QListWidgetItem*);
    void on_layer_index_change(const QString &);

    void on_debug_view(bool);
    void on_view_occupied_spaces(bool);

private:

    void _disable_side_tb_actions();

    static Toolkit * m_ptr;

    Ui_Toolkit m_ui;

    QSpinBox * m_brush_height;
    QSpinBox * m_current_layer;
    QSpinBox * m_grid_x;
    QSpinBox * m_grid_y;
    QComboBox * m_layer_CB;
    QMenu * m_brush_menu;
    QToolButton * m_brush_tool_btn;

    BrushMenuWidget * m_brush_menu_widget;
    CameraSettingsDialog * m_cam_settings;
    resource_browser * m_resource_browser;

    resource_dialog * m_res_dialog;
    resource_dialog_prev * m_res_dialog_prev;
    resource_dialog_prev_lighting * m_res_dialog_prev_lighting;

    int32 m_layers_above_hidden;
    int32 m_spinbox_val;
    QString m_prev_layer_text;
};


#define bbtk Toolkit::inst()

#endif // TOOLKIT_H
