#ifndef IMPORT_RESOURCES_DIALOG_H
#define IMPORT_RESOURCES_DIALOG_H

#include <QDialog>

namespace Ui {
class import_resources_dialog;
}

class import_resources_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit import_resources_dialog(QWidget *parent = 0);
    ~import_resources_dialog();

    Ui::import_resources_dialog *ui;

public slots:

    void on_btn_browse_pressed();
};

#endif // IMPORT_RESOURCES_DIALOG_H
