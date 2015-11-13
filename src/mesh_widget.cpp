#include <nsengine.h>
#include <nsmesh.h>
#include <nsentity.h>
#include <nstform_comp.h>
#include <nsmaterial.h>
#include <nscamera_system.h>
#include <nscam_comp.h>
#include <nsrender_comp.h>
#include <nssel_comp.h>
#include <nsrender_system.h>
#include <nsplugin.h>
#include <nstile_grid.h>
#include <nsscene.h>

#include <toolkit.h>
#include <resource_dialog_prev.h>
#include <preview.h>
#include <mesh_widget.h>
#include <ui_mesh_widget.h>
#include <edit_submesh_data_widget.h>

#include <QTreeWidget>

mesh_widget::mesh_widget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::mesh_widget),
    m_edit_mesh(nullptr),
    m_prev(nullptr)
{
    m_ui->setupUi(this);
}

mesh_widget::~mesh_widget()
{
    delete m_ui;
}

void mesh_widget::init(Preview * prev_)
{
    m_prev = prev_;
    connect(m_prev, SIGNAL(opengl_updated()), this, SLOT(preview_updated()));
}

void mesh_widget::done()
{
    m_prev->make_current();
    nsplugin * plg = nse.active();
    nsentity * ent = plg->get<nsentity>("preview_mesh_ent");
    if (ent != NULL)
    {
        nstform_comp * tc = ent->get<nstform_comp>();
        if (m_edit_mesh->tree()->m_root == NULL)
        {
            m_edit_mesh->bake_scaling(tc->scaling());
            m_edit_mesh->bake_rotation(tc->orientation());
            m_edit_mesh->bake_translation(tc->wpos());
        }
        else
        {
            m_edit_mesh->bake_node_scaling(tc->scaling());
            m_edit_mesh->bake_node_rotation(tc->orientation());
            m_edit_mesh->bake_node_translation(tc->wpos());
        }
    }
    m_edit_mesh = NULL;
}

void mesh_widget::set_mesh(nsmesh * mesh_)
{
    m_edit_mesh = mesh_;

    m_prev->make_current();
    nsplugin * plg = nse.active();

    // For the mesh we want focus mode
    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_focus);
    nse.system<nscamera_system>()->set_sensitivity(DEFAULT_CAM_SENSITIVITY*0.02,nscamera_system::sens_strafe);
    nse.system<nscamera_system>()->set_zoom(DEFAULT_CAM_ZOOM_FACTOR*0.03f);

    // Create the scene for viewing the preview and set it to current
    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,0.0f));

    // Create our directional light and camera and add it to the scene
    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,DEF_LT_DIFF,DEF_LT_AMB);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_prev->width(), m_prev->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
    scn->add(dir_light, fvec3(0,0,-20.0f));
    scn->add(camera, fvec3(0.0f,-4.0f,-4.0f), ::orientation(fvec3(-70.0f,0.0f,0.0f),fvec3::XYZ));
    scn->set_camera(camera);

    // Set up a basic material to use for previewing the mesh - by default wireframe enabled
    nsmaterial * mat = plg->create<nsmaterial>("preview_mat");
    mat->set_color_mode(true);
    mat->set_specular(4.0f, 0.5f, fvec3(1,1,1));
    mat->set_color(fvec3(0.7,0.7,0.7));
    mat->enable_wireframe(false);

    // This will be the entity which is used to view the mesh
    nsentity * ent = plg->create<nsentity>("preview_mesh_ent");
    nsrender_comp * rcomp = ent->create<nsrender_comp>();
    rcomp->set_mesh_id(m_edit_mesh->full_id());
    for (uint i = 0; i < m_edit_mesh->count(); ++i)
        rcomp->set_material(i, mat->full_id(), true);
    ent->create<nssel_comp>();
    scn->add(ent);
    ent->get<nstform_comp>()->enable_snap(false);

    // Make the base tile layer for seeing size comparison
    nsentity * base_tile = plg->create_tile("preview_tile", "diffuseGrass.png", "normalGrass.png", fvec3(),4.0f, 0.1f, fvec3(),true );
    scn->add_gridded(base_tile, ivec3(16,16,1), nstile_grid::world(ivec3(-8,-8,1)));
    base_tile->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !m_ui->cb_tile_layer->isChecked());

    // Setup the center point of the mesh
    nsentity * center_point = plg->create<nsentity>("preview_center");
    nsrender_comp * rc = center_point->create<nsrender_comp>();
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_SKYDOME)->full_id());
    nsmaterial * center_mat = plg->create<nsmaterial>("preview_center");
    center_mat->set_color_mode(true);
    center_mat->set_color(fvec4(1.0f,0.2f,0.4f,1.0f));
    center_mat->set_specular_intensity(0.8f);
    center_mat->set_specular_power(16.0f);
    rc->set_material(0, center_mat->full_id());
    scn->add(center_point,fvec3(),fquat(),fvec3(0.04f,0.04f,0.04f));
    center_point->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !m_ui->cb_snap_point->isChecked());

    m_ui->lbl_total_verts->setText(QString::number(m_edit_mesh->vert_count()));
    m_ui->lbl_total_indices->setText(QString::number(m_edit_mesh->indice_count()));
    m_ui->lbl_submesh_count->setText(QString::number(m_edit_mesh->count()));
    m_ui->sb_submesh->setMinimum(1);
    m_ui->sb_submesh->setMaximum(m_edit_mesh->count());
    m_ui->btn_node_tree->setEnabled(m_edit_mesh->tree()->m_root != NULL);
    m_ui->btn_joint->setEnabled(m_edit_mesh->tree()->m_name_joint_map.size() > 0);
    _update_submesh_info(0);
}

nsmesh * mesh_widget::mesh()
{
    return m_edit_mesh;
}

void mesh_widget::on_btn_flip_all_norms_pressed()
{
    m_edit_mesh->flip_normals();
}

void mesh_widget::on_btn_flip_all_uvs_pressed()
{
    m_edit_mesh->flip_uv();
}

void mesh_widget::on_btn_flip_sub_norms_pressed()
{
    m_edit_mesh->flip_normals(m_ui->sb_submesh->value()-1);
}

void mesh_widget::on_btn_flip_sub_uvs_pressed()
{
    m_edit_mesh->flip_uv(m_ui->sb_submesh->value()-1);
}

void mesh_widget::on_btn_verts_pressed()
{
    m_prev->make_current();
    edit_submesh_data_widget sw(this);
    sw.init(m_edit_mesh, m_ui->sb_submesh->value()-1);
    sw.exec();
}

void mesh_widget::on_btn_joint_pressed()
{
    QHBoxLayout * layout = new QHBoxLayout;
    QVBoxLayout * layoutv = new QVBoxLayout;
    QDialog * dialog = new QDialog(this);
    QTreeWidget * tree = new QTreeWidget;

    tree->setColumnCount(3);
    QStringList sl;
    sl.append("joint");
    sl.append("property");
    sl.append("data");
    tree->setHeaderLabels(sl);

    auto iter = m_edit_mesh->tree()->m_name_joint_map.begin();
    while (iter != m_edit_mesh->tree()->m_name_joint_map.end())
    {
        QTreeWidgetItem * item = new QTreeWidgetItem();
        item->setText(0, iter->first.c_str());
        QTreeWidgetItem * child = new QTreeWidgetItem();
        QTreeWidgetItem * child2 = new QTreeWidgetItem();
        child->setText(1, "ID");
        child->setText(2, QString::number(iter->second.m_joint_id));
        child2->setText(1, "Offset Transform");
        child2->setText(2, iter->second.m_offset_tform.round_to_zero().to_string().c_str());
        item->addChild(child);
        item->addChild(child2);
        tree->addTopLevelItem(item);
        ++iter;
    }
    layoutv->addWidget(tree);
    dialog->setLayout(layoutv);

    QPushButton * done_button = new QPushButton("Done");

    QSpacerItem * spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addSpacerItem(spacer);
    layout->addWidget(done_button);
    layoutv->addLayout(layout);
    connect(done_button, SIGNAL(pressed()), dialog, SLOT(accept()));
    dialog->resize(500,500);
    dialog->exec();
    delete dialog;
}

void mesh_widget::on_btn_node_tree_pressed()
{
    QHBoxLayout * layout = new QHBoxLayout;
    QVBoxLayout * layoutv = new QVBoxLayout;
    QDialog * dialog = new QDialog(this);
    QTreeWidget * tree = new QTreeWidget;

    tree->setColumnCount(3);
    QStringList sl;
    sl.append("node");
    sl.append("property");
    sl.append("data");
    tree->setHeaderLabels(sl);

    QTreeWidgetItem * item = new QTreeWidgetItem;
    _add_node_to_tree(m_edit_mesh->tree()->m_root, item);
    tree->addTopLevelItem(item);

    layoutv->addWidget(tree);
    dialog->setLayout(layoutv);
    QPushButton * done_button = new QPushButton("Done");
    QSpacerItem * spacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addSpacerItem(spacer);
    layout->addWidget(done_button);
    layoutv->addLayout(layout);
    connect(done_button, SIGNAL(pressed()), dialog, SLOT(accept()));
    dialog->resize(500,500);
    dialog->exec();
    delete dialog;
}

void mesh_widget::_add_node_to_tree(void * node_, QTreeWidgetItem * item)
{
    nsmesh::node * node = static_cast<nsmesh::node*>(node_);

    QTreeWidgetItem * node_id_item = new QTreeWidgetItem;
    node_id_item->setText(1, "ID");
    node_id_item->setText(2, QString::number(node->m_node_id));

    QTreeWidgetItem * node_ntfrom_item = new QTreeWidgetItem;
    node_ntfrom_item->setText(1, "Local Transform");
    node_ntfrom_item->setText(2, node->m_node_tform.round_to_zero().to_string().c_str());

    QTreeWidgetItem * node_wtform_item = new QTreeWidgetItem;
    node_wtform_item->setText(1, "World Transform");
    node_wtform_item->setText(2, node->m_world_tform.round_to_zero().to_string().c_str());

    item->setText(0, node->m_name.c_str());
    item->addChild(node_id_item);
    item->addChild(node_ntfrom_item);
    item->addChild(node_wtform_item);

    QTreeWidgetItem * node_child_item;
    auto iter = node->m_child_nodes.begin();
    while (iter != node->m_child_nodes.end())
    {
        node_child_item = new QTreeWidgetItem;
        _add_node_to_tree(*iter,node_child_item);
        item->addChild(node_child_item);
        ++iter;
    }
}

void mesh_widget::on_cb_tile_layer_toggled(bool new_val)
{
    m_prev->make_current();
    nsentity * base_tile = nse.active()->get<nsentity>("preview_tile");
    if (base_tile == NULL)
        return;
    base_tile->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !new_val);
}

void mesh_widget::on_cb_snap_point_toggled(bool new_val)
{
    m_prev->make_current();
    nsentity * prev_center = nse.active()->get<nsentity>("preview_center");
    if (prev_center == NULL)
        return;
    prev_center->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !new_val);
}

void mesh_widget::on_cb_wireframe_toggled(bool new_val)
{
    m_prev->make_current();
    nse.active()->get<nsmaterial>("preview_mat")->enable_wireframe(new_val);
}

void mesh_widget::on_cb_uv_coords_toggled(bool new_val)
{
    m_edit_mesh->sub(m_ui->sb_submesh->value()-1)->m_has_tex_coords = new_val;
}

void mesh_widget::on_dsb_offset_x_valueChanged(double)
{
    _mesh_translate();
}

void mesh_widget::on_dsb_offset_y_valueChanged(double)
{
    _mesh_translate();
}

void mesh_widget::on_dsb_offset_z_valueChanged(double)
{
    _mesh_translate();
}

void mesh_widget::on_dsb_scale_x_valueChanged(double val_)
{
    _mesh_scale(val_);
}

void mesh_widget::on_dsb_scale_y_valueChanged(double val_)
{
    _mesh_scale(val_);
}

void mesh_widget::on_dsb_scale_z_valueChanged(double val_)
{
    _mesh_scale(val_);
}

void mesh_widget::on_dsb_rot_x_valueChanged(int)
{
    _mesh_rotate();
}

void mesh_widget::on_dsb_rot_y_valueChanged(int)
{
    _mesh_rotate();
}

void mesh_widget::on_dsb_rot_z_valueChanged(int)
{
    _mesh_rotate();
}

void mesh_widget::on_sb_submesh_valueChanged(int i)
{
    _update_submesh_info(i-1);
}

void mesh_widget::_mesh_translate()
{
    m_prev->make_current();
    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == NULL)
        return;
    nstform_comp * tc = ent->get<nstform_comp>();
    tc->set_pos(fvec3(m_ui->dsb_offset_x->value(),
                      m_ui->dsb_offset_y->value(),
                      m_ui->dsb_offset_z->value()));
}

void mesh_widget::_mesh_scale(double new_val)
{
    m_prev->make_current();
    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == NULL)
        return;
    nstform_comp * tc = ent->get<nstform_comp>();

    if (m_ui->cb_constrain_dim->isChecked())
    {
        m_ui->dsb_scale_x->blockSignals(true);
        m_ui->dsb_scale_x->setValue(new_val);
        m_ui->dsb_scale_x->blockSignals(false);

        m_ui->dsb_scale_y->blockSignals(true);
        m_ui->dsb_scale_y->setValue(new_val);
        m_ui->dsb_scale_y->blockSignals(false);

        m_ui->dsb_scale_z->blockSignals(true);
        m_ui->dsb_scale_z->setValue(new_val);
        m_ui->dsb_scale_z->blockSignals(false);
    }
    tc->set_scale(fvec3( m_ui->dsb_scale_x->value(),
                         m_ui->dsb_scale_y->value(),
                         m_ui->dsb_scale_z->value()));
}

void mesh_widget::_mesh_rotate()
{
    m_prev->make_current();

    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == NULL)
        return;
    nstform_comp * tc = ent->get<nstform_comp>();
    fvec3::EulerOrder eo = fvec3::EulerOrder(m_ui->cmb_euler_order->currentIndex());

    fmat4 rotmat = ::rotation_mat4(fvec3(m_ui->dsb_rot_x->value(),m_ui->dsb_rot_y->value(),m_ui->dsb_rot_z->value()),eo,false);
    tc->set_orientation(fquat().from(rotmat));
}

void mesh_widget::_update_submesh_info(uint32 sub_index)
{
    m_ui->le_submesh_name->setText(m_edit_mesh->sub(sub_index)->m_name.c_str());
    m_ui->lbl_submesh_verts->setText(QString::number(m_edit_mesh->vert_count(sub_index)));
    m_ui->lbl_indices->setText(QString::number(m_edit_mesh->indice_count(sub_index)));
    m_ui->cb_uv_coords->setChecked(m_edit_mesh->sub(sub_index)->m_has_tex_coords);

    if (m_edit_mesh->sub(sub_index)->m_node != NULL)
        m_ui->lbl_submesh_node->setText(m_edit_mesh->sub(sub_index)->m_node->m_name.c_str());
    else
        m_ui->lbl_submesh_node->setText("None");

    if (m_edit_mesh->sub(sub_index)->m_prim_type == GL_TRIANGLES)
        m_ui->lbl_prim_type->setText("Triangle");
    else if (m_edit_mesh->sub(sub_index)->m_prim_type == GL_LINES)
        m_ui->lbl_prim_type->setText("Line");
    else if (m_edit_mesh->sub(sub_index)->m_prim_type == GL_POINTS)
        m_ui->lbl_prim_type->setText("Point");
    else
        m_ui->lbl_prim_type->setText("What the crap?");

    m_ui->lbl_joint_count->setText(QString::number(m_edit_mesh->joint_count()));
    m_ui->lbl_node_count->setText(QString::number(m_edit_mesh->node_count()));
}

void mesh_widget::preview_updated()
{
    if (m_prev == nullptr || m_edit_mesh == nullptr)
        return;

    m_prev->make_current();
    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == nullptr)
        return;

    nstform_comp * tc = ent->get<nstform_comp>();

    m_ui->dsb_offset_x->blockSignals(true);
    m_ui->dsb_offset_x->setValue(tc->wpos().x);
    m_ui->dsb_offset_x->blockSignals(false);

    m_ui->dsb_offset_y->blockSignals(true);
    m_ui->dsb_offset_y->setValue(tc->wpos().y);
    m_ui->dsb_offset_y->blockSignals(false);

    m_ui->dsb_offset_z->blockSignals(true);
    m_ui->dsb_offset_z->setValue(tc->wpos().z);
    m_ui->dsb_offset_z->blockSignals(false);
}
