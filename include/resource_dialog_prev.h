#ifndef RESOURCE_DIALOG_PREV_H
#define RESOURCE_DIALOG_PREV_H

#include <ui_resource_dialog_prev.h>

class texture_widget;

class resource_dialog_prev : public QDialog
{
    Q_OBJECT
public:

    resource_dialog_prev(QWidget * parent=NULL);

    void init();

    void set_texture();

    void set_animation();

    void set_mesh();

private:

    Ui::resource_dialog_prev m_ui;

    texture_widget * m_tex_widget;

};

#endif // RESOURCE_DIALOG_PREV_H
