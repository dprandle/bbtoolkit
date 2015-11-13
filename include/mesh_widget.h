#ifndef MESH_WIDGET_H
#define MESH_WIDGET_H

#include <QWidget>
#include <nstypes.h>

class Preview;
class nsmesh;
class QTreeWidgetItem;

namespace Ui {
class mesh_widget;
}

class mesh_widget : public QWidget
{
    Q_OBJECT

public:
    explicit mesh_widget(QWidget *parent = 0);
    ~mesh_widget();

    void init(Preview * prev_);

    void done();

    void set_mesh(nsmesh * mesh_);

    nsmesh * mesh();

public slots:

    void on_btn_flip_all_norms_pressed();

    void on_btn_flip_all_uvs_pressed();

    void on_btn_flip_sub_norms_pressed();

    void on_btn_flip_sub_uvs_pressed();

    void on_btn_verts_pressed();

    void on_btn_joint_pressed();

    void on_btn_node_tree_pressed();

    void on_cb_tile_layer_toggled(bool);

    void on_cb_snap_point_toggled(bool);

    void on_cb_wireframe_toggled(bool);

    void on_cb_uv_coords_toggled(bool);

    void on_dsb_offset_x_valueChanged(double);

    void on_dsb_offset_y_valueChanged(double);

    void on_dsb_offset_z_valueChanged(double);

    void on_dsb_scale_x_valueChanged(double);

    void on_dsb_scale_y_valueChanged(double);

    void on_dsb_scale_z_valueChanged(double);

    void on_dsb_rot_x_valueChanged(int);

    void on_dsb_rot_y_valueChanged(int);

    void on_dsb_rot_z_valueChanged(int);

    void on_sb_submesh_valueChanged(int);

    void preview_updated();

private:

    void _update_submesh_info(uint32 i);

    void _add_node_to_tree(void * node_, QTreeWidgetItem * item);

    void _mesh_scale(double new_val);

    void _mesh_rotate();

    void _mesh_translate();

    Ui::mesh_widget *m_ui;

    nsmesh * m_edit_mesh;

    Preview * m_prev;

};

#endif // MESH_WIDGET_H
