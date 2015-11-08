#include <nsmesh.h>
#include <nsmath.h>
#include <nstypes.h>
#include <nsvector.h>
#include <sstream>
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
        QTableWidgetItem * vert_item = new QTableWidgetItem;
        ui->tbl_data->setItem(i,0, vert_item);
        QTableWidgetItem * norm_item = new QTableWidgetItem;
        ui->tbl_data->setItem(i,1,norm_item);
        QTableWidgetItem * tang_item = new QTableWidgetItem;
        ui->tbl_data->setItem(i,2,tang_item);
        QTableWidgetItem * uvcoord_item = new QTableWidgetItem;
        ui->tbl_data->setItem(i,3,uvcoord_item);
        QTableWidgetItem * joint_weight_item = new QTableWidgetItem;
        ui->tbl_data->setItem(i,4,joint_weight_item);

        vert_item->setText(sub->m_verts[i].to_string().c_str());
        norm_item->setText(sub->m_normals[i].to_string().c_str());
        tang_item->setText(sub->m_tangents[i].to_string().c_str());
        uvcoord_item->setText(sub->m_tex_coords[i].to_string().c_str());
        std::stringstream ss;
        ss << sub->m_joints[i].m_joint_ids[0] << ":" << sub->m_joints[i].m_weights[0] << " " << sub->m_joints[i].m_joint_ids[1] << ":" << sub->m_joints[i].m_weights[1] << " " << sub->m_joints[i].m_joint_ids[2] << ":" << sub->m_joints[i].m_weights[2] << " " << sub->m_joints[i].m_joint_ids[3] << ":" << sub->m_joints[i].m_weights[3];
        joint_weight_item->setText(ss.str().c_str());

        QTableWidgetItem * hi = new QTableWidgetItem(QString::number(i));
        ui->tbl_data->setVerticalHeaderItem(i,hi);
    }
    ui->tbl_data->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    on_cb_vertices_toggled(true);
    on_cb_joint_weights_toggled(false);
    on_cb_normals_toggled(true);
    on_cb_uvcoords_toggled(false);
    on_cb_tangents_toggled(false);
}

void edit_submesh_data_widget::on_cb_vertices_toggled(bool show)
{
    if (!show)
        ui->tbl_data->hideColumn(0);
    else
        ui->tbl_data->showColumn(0);
}

void edit_submesh_data_widget::on_cb_normals_toggled(bool show)
{
    if (!show)
        ui->tbl_data->hideColumn(1);
    else
        ui->tbl_data->showColumn(1);
}

void edit_submesh_data_widget::on_cb_tangents_toggled(bool show)
{
    if (!show)
        ui->tbl_data->hideColumn(2);
    else
        ui->tbl_data->showColumn(2);
}

void edit_submesh_data_widget::on_cb_uvcoords_toggled(bool show)
{
    if (!show)
        ui->tbl_data->hideColumn(3);
    else
        ui->tbl_data->showColumn(3);
}

void edit_submesh_data_widget::on_cb_joint_weights_toggled(bool show)
{
    if (!show)
        ui->tbl_data->hideColumn(4);
    else
        ui->tbl_data->showColumn(4);
}
