#ifndef TEXTURE_WIDGET_H
#define TEXTURE_WIDGET_H

#include <nstypes.h>
#include <QWidget>

class nstexture;

namespace Ui {
class texture_widget;
}

class texture_widget : public QWidget
{
    Q_OBJECT

public:
    explicit texture_widget(QWidget *parent = 0);
    ~texture_widget();

    void init();

    void clear_fields();

    Ui::texture_widget * ui;

signals:

    void cubemap_triggered();
    void tex2d_triggered();
    void tex1d_triggered();

public slots:
    void on_m_tex_type_cmb_currentIndexChanged(int);
    void on_m_image_file_tb_pressed();
    void on_m_cm_front_tb_pressed();
    void on_m_cm_back_tb_pressed();
    void on_m_cm_right_tb_pressed();
    void on_m_cm_left_tb_pressed();
    void on_m_cm_top_tb_pressed();
    void on_m_cm_bottom_tb_pressed();

};

#endif // TEXTURE_WIDGET_H
