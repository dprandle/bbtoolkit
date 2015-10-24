#ifndef NEW_RESOURCE_DIALOG_H
#define NEW_RESOURCE_DIALOG_H

#include <ui_resource_dialog.h>

class resource_dialog : public QDialog
{
    Q_OBJECT
public:

    resource_dialog(QWidget * parent=NULL);
    ~resource_dialog();

    void init();

private:
    Ui::resource_dialog m_ui;
};

#endif // NEW_RESOURCE_DIALOG_H
