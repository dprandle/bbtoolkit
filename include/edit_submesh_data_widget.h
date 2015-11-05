#ifndef EDIT_DATA_WIDGET_H
#define EDIT_DATA_WIDGET_H

#include <nstypes.h>
#include <QDialog>

class nsmesh;

namespace Ui {
class edit_submesh_data_widget;
}

class edit_submesh_data_widget : public QDialog
{
    Q_OBJECT

public:
    explicit edit_submesh_data_widget(QWidget *parent = 0);
    ~edit_submesh_data_widget();

    void init(nsmesh * mesh, uint32 sub_index);

    Ui::edit_submesh_data_widget *ui;

public slots:

    void on_cb_vertices_toggled(bool);

    void on_cb_normals_toggled(bool);

    void on_cb_tangents_toggled(bool);

    void on_cb_uvcoords_toggled(bool);

    void on_cb_joint_weights_toggled(bool);
};

#endif // EDIT_DATA_WIDGET_H
