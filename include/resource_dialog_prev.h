#ifndef RESOURCE_DIALOG_PREV_H
#define RESOURCE_DIALOG_PREV_H

#include <ui_resource_dialog_prev.h>

#define BG_R 0.2f
#define BG_G 0.2f
#define BG_B 0.3f

class texture_widget;
class nstexture;
class nsmesh;
class nsresource;
class mesh_widget;
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

    void set_texture(nstexture * tex_=NULL);

    void set_mesh(nsmesh * mesh_);

    void set_mesh(const nsstring & model_fname);

    uivec2 get_editing_res();

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

    void preview_updated();

private:
    void _set_mesh_widget_fields(nsmesh * msh, nsplugin * plg);

    void _update_submesh_info(nsmesh * msh, uint32 sub_index);

    void _reset_fields();

    void _setup_preview_controls_tex();

    void _setup_preview_controls_mesh();

    void _add_node_to_tree(void * node_, QTreeWidgetItem * item);

    Ui::resource_dialog_prev m_ui;

    nsresource * m_editing_res;

    texture_widget * m_tex_widget;
    mesh_widget * m_mesh_widget;

    nsstring m_starting_res;
    nsstring m_starting_plug;
    nsstring m_starting_subdir;

    uivec2 m_last_edit;

};

#endif // RESOURCE_DIALOG_PREV_H
