#ifndef RESOURCE_DIALOG_PREV_H
#define RESOURCE_DIALOG_PREV_H

#include <ui_resource_dialog_prev.h>

#define BG_R 0.3f
#define BG_G 0.3f
#define BG_B 0.4f

#define DEF_LT_AMB 1.0f
#define DEF_LT_DIFF 0.0f

#define PREV_LIGHT_NAME "preview_dir_light"
#define PREV_CAM_NAME "preview_cam"

class texture_widget;
class nstexture;
class material_widget;
class component_selection_widget;
class nsentity;
class nsmaterial;
class nsmesh;
class nsasset;
class mesh_widget;
class entity_widget;
class nsplugin;
class QTreeWidgetItem;
class QTreeWidget;

class resource_dialog_prev : public QDialog
{
    Q_OBJECT
public:

    enum edit_type {
        edit_mesh,
        edit_tex
    };

    resource_dialog_prev(QWidget * parent=NULL);

    void init();

    bool set_entity(nsentity * ent_);

    bool set_entity(const nsstring & model_fname);

    bool set_texture(nstexture * tex_=NULL);

    bool set_mesh(nsmesh * mesh_);

    bool set_mesh(const nsstring & model_fname);

    bool set_material(nsmaterial * mat_=NULL);

    uivec2 get_editing_res();

    void reject();

public slots:

    void edit_res_changed(nsasset * res);

    void entity_edit_component(uint32 comp_type);

    void comp_sel_widget();

    void on_m_okay_btn_pressed();

    void on_m_cancel_btn_pressed();

    void on_m_plugin_tb_pressed();

    void on_m_folder_tb_pressed();

    void on_m_icon_path_tb_pressed();

    void on_m_icon_create_btn_pressed();

    void on_sldr_diffuse_valueChanged();

    void on_sldr_ambient_valueChanged();

    void on_sldr_horiz_plane_valueChanged();

    void on_slider_vert_plane_valueChanged();

    void on_cb_shadows_toggled(bool);

private:

    bool _setup_resource_ui(nsasset * incoming);

    void _reset_fields();

    void _setup_preview_controls_limited();

    void _setup_preview_controls_full();

    Ui::resource_dialog_prev m_ui;

    nsasset * m_editing_res;

    texture_widget * m_tex_widget;
    mesh_widget * m_mesh_widget;
    material_widget * m_mat_widget;
    entity_widget * m_ent_widget;
    component_selection_widget * m_comp_sel;

    nsstring m_starting_res;
    nsstring m_starting_plug;
    nsstring m_starting_subdir;

    uivec2 m_last_edit;
};

#endif // RESOURCE_DIALOG_PREV_H
