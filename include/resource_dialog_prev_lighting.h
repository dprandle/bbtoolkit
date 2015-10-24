#ifndef RESOURCE_DIALOG_PREV_LIGHTING_H
#define RESOURCE_DIALOG_PREV_LIGHTING_H

#include <ui_resource_dialog_prev_lighting.h>

class resource_dialog_prev_lighting : public QDialog
{
    Q_OBJECT
public:

    resource_dialog_prev_lighting(QWidget * parent=NULL);

    void init();

private:
    Ui::resource_dialog_prev_lighting m_ui;
};

#endif // RESOURCE_DIALOG_PREV_LIGHTING_H
