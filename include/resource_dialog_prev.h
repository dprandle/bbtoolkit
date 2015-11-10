#ifndef RESOURCE_DIALOG_PREV_H
#define RESOURCE_DIALOG_PREV_H

#include <ui_resource_dialog_prev.h>

#define BG_R 0.3f
#define BG_G 0.3f
#define BG_B 0.4f

#define PREV_LIGHT_NAME "preview_dir_light"
#define PREV_CAM_NAME "preview_cam"

class texture_widget;
class nstexture;
class material_widget;
class nsentity;
class nsmaterial;
class nsmesh;
class nsresource;
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

    bool set_entity(nsentity * mesh_);

    bool set_entity(const nsstring & model_fname);

    bool set_texture(nstexture * tex_=NULL);

    bool set_mesh(nsmesh * mesh_);

    bool set_mesh(const nsstring & model_fname);

    bool set_material(nsmaterial * mat_=NULL);

    uivec2 get_editing_res();

    void reject();

public slots:

    void tex_cubemap_triggered();

    void tex_tex2d_triggered();

    void mesh_show_verts();

    void submesh_index_changed(int index);

    void mesh_toggle_uv_coords(bool new_val);

    void mesh_toggle_show_tile_layer(bool new_val);

    void mesh_toggle_show_center(bool new_val);

    void mesh_wireframe_toggled(bool new_val);

    void mesh_view_joints();

    void mesh_view_node_tree();

    void mesh_scale(double new_val);

    void mesh_translate();

    void mesh_rotate(int);

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

    void mesh_flip_all_uvs();

    void mesh_flip_sub_uvs();

    void mesh_flip_all_norms();

    void mesh_flip_sub_norms();

    void mat_preview_mesh_changed(int new_index);

    void mat_from_ui();

    void mat_color_tb();

    void mat_spec_color_tb();

    void mat_btn_shader();

    void mat_btn_diffuse();

    void mat_btn_normal();

    void mat_btn_opac();

    void preview_updated();

    void on_cb_shadows_toggled(bool);

    void mat_dif_erase();

    void mat_norm_erase();

    void mat_opac_erase();

    void mat_shader_erase();

private:
    void _set_mesh_widget_fields(nsmesh * msh, nsplugin * plg);

    void _set_ent_widget_fields(nsentity * ent, nsplugin * plg);

    void _update_submesh_info(nsmesh * msh, uint32 sub_index);

    void _reset_fields();

    void _setup_preview_controls_tex();

    void _setup_preview_controls_mesh();

    void _add_node_to_tree(void * node_, QTreeWidgetItem * item);

    Ui::resource_dialog_prev m_ui;

    nsresource * m_editing_res;

    texture_widget * m_tex_widget;
    mesh_widget * m_mesh_widget;
    material_widget * m_mat_widget;
    entity_widget * m_ent_widget;

    nsstring m_starting_res;
    nsstring m_starting_plug;
    nsstring m_starting_subdir;

    uivec2 m_last_edit;

};

#endif // RESOURCE_DIALOG_PREV_H
