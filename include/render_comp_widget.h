#ifndef RENDER_COMP_WIDGET_H
#define RENDER_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class render_comp_widget;
}

class render_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit render_comp_widget(QWidget *parent = 0);
    ~render_comp_widget();

    uint32 type();

    void set_entity(nsentity * ent_);

public slots:

    void on_btn_mesh_pressed();

    void on_cb_cast_shadows(bool);

    void on_tb_clear_mesh();

    void on_tb_del_mat();

    void on_tb_edit_mat();

    void on_tb_add_mat();

private:

    Ui::render_comp_widget * m_ui;
};

#endif // RENDER_COMP_WIDGET_H
