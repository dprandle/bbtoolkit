#ifndef TEXTURE_WIDGET_H
#define TEXTURE_WIDGET_H

#include <QWidget>

namespace Ui {
class texture_widget;
}

class texture_widget : public QWidget
{
    Q_OBJECT

public:
    explicit texture_widget(QWidget *parent = 0);
    ~texture_widget();

public slots:
    void on_m_tex_type_cmb_currentIndexChanged(int);
    void on_m_image_file_tb_pressed();
    void on_m_cm_front_tb_pressed();
    void on_m_cm_back_tb_pressed();
    void on_m_cm_right_tb_pressed();
    void on_m_cm_left_tb_pressed();
    void on_m_cm_top_tb_pressed();
    void on_m_cm_bottom_tb_pressed();
    void on_m_auto_cb_toggled(bool);
    void on_m_random_cb_toggled(bool);
    void on_m_levels_sb_editingFinished();

private:
    Ui::texture_widget *m_ui;
};

#endif // TEXTURE_WIDGET_H
