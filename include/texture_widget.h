#ifndef TEXTURE_WIDGET_H
#define TEXTURE_WIDGET_H

#include <nstypes.h>
#include <QWidget>

class nstexture;
class nsresource;
class Preview;

namespace Ui {
class texture_widget;
}

class texture_widget : public QWidget
{
    Q_OBJECT

public:
    explicit texture_widget(QWidget *parent = 0);
    ~texture_widget();

    void init(Preview * prev_);

    void done();

    void clear_fields();

    void set_texture(nstexture * tex_);

    Ui::texture_widget * ui;

public slots:

    void on_m_tex_type_cmb_currentIndexChanged(int);

    void on_m_image_file_tb_pressed();

    void on_m_cm_front_tb_pressed();

    void on_m_cm_back_tb_pressed();

    void on_m_cm_right_tb_pressed();

    void on_m_cm_left_tb_pressed();

    void on_m_cm_top_tb_pressed();

    void on_m_cm_bottom_tb_pressed();

signals:
    void edit_res_change(nsresource *);

private:

    void _load_cubemap();

    void _load_tex2d();

    nstexture * m_edit_tex;

    Preview * m_prev;

};

#endif // TEXTURE_WIDGET_H
