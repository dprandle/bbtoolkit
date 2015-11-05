#include <nsmesh.h>
#include <nsmath.h>
#include <nstypes.h>
#include <nsvector.h>

#include <include/edit_submesh_data_widget.h>
#include <ui_edit_submesh_data_widget.h>


edit_submesh_data_widget::edit_submesh_data_widget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_submesh_data_widget)
{
    ui->setupUi(this);
}

edit_submesh_data_widget::~edit_submesh_data_widget()
{
    delete ui;
}

void edit_submesh_data_widget::init(nsmesh * mesh, uint32 sub_index)
{
    nsmesh::submesh * sub = mesh->sub(sub_index);
    uint32 size = sub->m_verts.size();
    ui->tbl_data->setRowCount(mesh->sub(sub_index)->m_verts.size());

    for (uint32 i = 0; i < size; ++i)
    {
        QTableWidgetItem * vert_item = ui->tbl_data->item(i,0);
        QTableWidgetItem * norm_item = ui->tbl_data->item(i,1);
        QTableWidgetItem * tang_item = ui->tbl_data->item(i,2);
        QTableWidgetItem * uvcoord_item = ui->tbl_data->item(i,3);
        QTableWidgetItem * joint_weight_item = ui->tbl_data->item(i,4);

        vert_item->setText(sub->m_verts[i].to_string().c_str());
        norm_item->setText(sub->m_normals[i].to_string().c_str());
        tang_item->setText(sub->m_tangents[i].to_string().c_str());
        uvcoord_item->setText(sub->m_tex_coords[i].to_string().c_str());
        vert_item->setText(sub->m_verts[i].to_string().c_str());
    }
}

void edit_submesh_data_widget::on_cb_vertices_toggled(bool hide)
{
    if (hide)
        ui->tbl_data->hideColumn(0);
    else
        ui->tbl_data->showColumn(0);
}

void edit_submesh_data_widget::on_cb_normals_toggled(bool hide)
{
    if (hide)
        ui->tbl_data->hideColumn(1);
    else
        ui->tbl_data->showColumn(1);
}

void edit_submesh_data_widget::on_cb_tangents_toggled(bool hide)
{
    if (hide)
        ui->tbl_data->hideColumn(2);
    else
        ui->tbl_data->showColumn(2);
}

void edit_submesh_data_widget::on_cb_uvcoords_toggled(bool hide)
{
    if (hide)
        ui->tbl_data->hideColumn(3);
    else
        ui->tbl_data->showColumn(3);
}

void edit_submesh_data_widget::on_cb_joint_weights_toggled(bool hide)
{
    if (hide)
        ui->tbl_data->hideColumn(4);
    else
        ui->tbl_data->showColumn(4);
}
