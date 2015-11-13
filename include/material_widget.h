#ifndef MATERIAL_WIDGET_H
#define MATERIAL_WIDGET_H

#include <QWidget>
#include <nstypes.h>

class Preview;
class QLineEdit;
class nsmaterial;

namespace Ui {
class material_widget;
}

class material_widget : public QWidget
{
    Q_OBJECT

public:
    explicit material_widget(QWidget *parent = 0);
    ~material_widget();

    void init(Preview * prev_);

    void done();

    void set_material(nsmaterial * mat_);

    nsmaterial * material();

public slots:

    void on_btn_diffuse_pressed();

    void on_btn_normal_pressed();

    void on_btn_shader_pressed();

    void on_btn_opacity_pressed();

    void on_btn_height_pressed();

    void on_tb_material_color_pressed();

    void on_tb_spec_color_pressed();

    void on_cb_color_mode_toggled(bool);

    void on_cb_cull_enable_toggled(bool);

    void on_cb_wireframe_toggled(bool);

    void on_cmb_cull_mode_currentIndexChanged(int);

    void on_sldr_mat_shininess_valueChanged(int);

    void on_sb_spec_power_valueChanged(int);

    void on_tb_norm_erase_pressed();

    void on_tb_dif_erase_pressed();

    void on_tb_opac_erase_pressed();

    void on_tb_height_erase_pressed();

    void on_tb_shader_erase_pressed();

    void on_cmb_preview_mesh_currentIndexChanged(int);

private:

    void _btn_texture(uint32 map_type, QLineEdit * plg_, QLineEdit * name_);

    Ui::material_widget * m_ui;

    nsmaterial * m_edit_mat;

    Preview * m_prev;

};

#endif // MATERIAL_WIDGET_H
