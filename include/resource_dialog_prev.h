#ifndef RESOURCE_DIALOG_PREV_H
#define RESOURCE_DIALOG_PREV_H

#include <ui_resource_dialog_prev.h>

class texture_widget;
class nstexture;
class nsmesh;
class nsresource;

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

    void set_mesh(nsmesh * mesh_=NULL);

public slots:

    void tex_cubemap_triggered();

    void tex_tex2d_triggered();

    void on_m_okay_btn_pressed();

    void on_m_cancel_btn_pressed();

    void on_m_plugin_tb_pressed();

    void on_m_folder_tb_pressed();

    void on_m_icon_path_tb_pressed();

    void on_m_icon_create_btn_pressed();

private:
    void _reset_fields();

    void _setup_preview_controls();

    Ui::resource_dialog_prev m_ui;

    texture_widget * m_tex_widget;

    nsresource * m_editing_res;

    nsstring m_starting_res;
    nsstring m_starting_plug;
    nsstring m_starting_subdir;

};

#endif // RESOURCE_DIALOG_PREV_H
