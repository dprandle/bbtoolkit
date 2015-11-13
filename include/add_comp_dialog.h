#ifndef ADD_COMP_WIDGET_H
#define ADD_COMP_WIDGET_H

#include <QDialog>
#include <nstypes.h>

namespace Ui {
class add_comp_dialog;
}

class add_comp_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit add_comp_dialog(QWidget *parent = 0);
    ~add_comp_dialog();

    uint32 selected_comp();

public slots:

    void on_tb_anim_pressed();

    void on_tb_cam_pressed();

    void on_tb_light_pressed();

    void on_tb_occupy_pressed();

    void on_tb_render_pressed();

    void on_tb_tile_pressed();

    void on_tb_tile_brush_pressed();

    void on_tb_terrain_pressed();

    void on_tb_sel_pressed();

    void on_tb_particle_pressed();

private:
    Ui::add_comp_dialog * m_ui;
    uint32 m_comp_type;
};

#endif // ADD_COMP_WIDGET_H
