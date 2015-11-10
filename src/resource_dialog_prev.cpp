#include <nsengine.h>
#include <nsplugin.h>
#include <nsinput_map.h>
#include <nscamera_system.h>
#include <nsinput_system.h>
#include <nscam_comp.h>
#include <nsentity.h>
#include <nsscene.h>
#include <nsmesh.h>
#include <nsmaterial.h>
#include <nsrender_comp.h>
#include <nstexture.h>
#include <nsrender_system.h>
#include <nsshader.h>
#include <nssel_comp.h>
#include <nstex_manager.h>
#include <nsselection_system.h>
#include <nsselection_system.h>
#include <nsfile_os.h>
#include <nslight_comp.h>
#include <nsplugin_manager.h>

#include <toolkit.h>
#include <select_res_dialog.h>
#include <resource_dialog_prev.h>

#include <mesh_widget.h>
#include <ui_mesh_widget.h>

#include <texture_widget.h>
#include <ui_texture_widget.h>

#include <edit_submesh_data_widget.h>
#include <ui_edit_submesh_data_widget.h>

#include <material_widget.h>
#include <ui_material_widget.h>

#include <entity_widget.h>
#include <ui_entity_widget.h>

#include <select_res_dialog.h>

#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QTreeWidget>
#include <QPixmap>
#include <QBitmap>
#include <QSpacerItem>

resource_dialog_prev::resource_dialog_prev(QWidget * parent):
    QDialog(parent),
    m_editing_res(NULL),
    m_tex_widget(new texture_widget()),
    m_mesh_widget(new mesh_widget()),
    m_mat_widget(new material_widget()),
    m_ent_widget(new entity_widget())
{
    m_ui.setupUi(this);    
    connect(m_tex_widget, SIGNAL(cubemap_triggered()), this, SLOT(tex_cubemap_triggered()));
    connect(m_tex_widget, SIGNAL(tex2d_triggered()), this, SLOT(tex_tex2d_triggered()));
    connect(m_mesh_widget->ui->cb_wireframe, SIGNAL(toggled(bool)), this, SLOT(mesh_wireframe_toggled(bool)));
    connect(m_mesh_widget->ui->btn_verts, SIGNAL(pressed()), this, SLOT(mesh_show_verts()));
    connect(m_mesh_widget->ui->sb_submesh, SIGNAL(valueChanged(int)), SLOT(submesh_index_changed(int)));
    connect(m_mesh_widget->ui->btn_joint, SIGNAL(pressed()), this, SLOT(mesh_view_joints()));
    connect(m_mesh_widget->ui->btn_node_tree, SIGNAL(pressed()), this, SLOT(mesh_view_node_tree()));
    connect(m_ui.m_preview, SIGNAL(opengl_updated()), this, SLOT(preview_updated()));
    connect(m_mesh_widget->ui->dsb_offset_x, SIGNAL(valueChanged(double)), this, SLOT(mesh_translate()));
    connect(m_mesh_widget->ui->dsb_offset_y, SIGNAL(valueChanged(double)), this, SLOT(mesh_translate()));
    connect(m_mesh_widget->ui->dsb_offset_z, SIGNAL(valueChanged(double)), this, SLOT(mesh_translate()));
    connect(m_mesh_widget->ui->dsb_scale_x, SIGNAL(valueChanged(double)), this, SLOT(mesh_scale(double)));
    connect(m_mesh_widget->ui->dsb_scale_y, SIGNAL(valueChanged(double)), this, SLOT(mesh_scale(double)));
    connect(m_mesh_widget->ui->dsb_scale_z, SIGNAL(valueChanged(double)), this, SLOT(mesh_scale(double)));
    connect(m_mesh_widget->ui->dsb_rot_x, SIGNAL(valueChanged(int)), this, SLOT(mesh_rotate(int)));
    connect(m_mesh_widget->ui->dsb_rot_y, SIGNAL(valueChanged(int)), this, SLOT(mesh_rotate(int)));
    connect(m_mesh_widget->ui->dsb_rot_z, SIGNAL(valueChanged(int)), this, SLOT(mesh_rotate(int)));
    connect(m_mesh_widget->ui->cb_tile_layer, SIGNAL(toggled(bool)), this, SLOT(mesh_toggle_show_tile_layer(bool)));
    connect(m_mesh_widget->ui->cb_snap_point, SIGNAL(toggled(bool)), this, SLOT(mesh_toggle_show_center(bool)));
    connect(m_mesh_widget->ui->btn_flip_all_norms, SIGNAL(pressed()), this, SLOT(mesh_flip_all_norms()));
    connect(m_mesh_widget->ui->btn_flip_all_uvs, SIGNAL(pressed()), this, SLOT(mesh_flip_all_uvs()));
    connect(m_mesh_widget->ui->btn_flip_sub_norms, SIGNAL(pressed()), this, SLOT(mesh_flip_sub_norms()));
    connect(m_mesh_widget->ui->btn_flip_sub_uvs, SIGNAL(pressed()), this, SLOT(mesh_flip_sub_uvs()));

    // mat connections
    connect(m_mat_widget->ui->cmb_preview_mesh, SIGNAL(currentIndexChanged(int)), this, SLOT(mat_preview_mesh_changed(int)));

    connect(m_mat_widget->ui->btn_diffuse, SIGNAL(pressed()), this, SLOT(mat_btn_diffuse()));
    connect(m_mat_widget->ui->btn_normal, SIGNAL(pressed()), this, SLOT(mat_btn_normal()));
    connect(m_mat_widget->ui->btn_shader, SIGNAL(pressed()), this, SLOT(mat_btn_shader()));
    connect(m_mat_widget->ui->btn_opacity, SIGNAL(pressed()), this, SLOT(mat_btn_opac()));

    connect(m_mat_widget->ui->tb_material_color, SIGNAL(pressed()), this, SLOT(mat_color_tb()));
    connect(m_mat_widget->ui->tb_spec_color, SIGNAL(pressed()), this, SLOT(mat_spec_color_tb()));

    connect(m_mat_widget->ui->cb_color_mode, SIGNAL(toggled(bool)), this, SLOT(mat_from_ui()));
    connect(m_mat_widget->ui->cb_cull_enable, SIGNAL(toggled(bool)), this, SLOT(mat_from_ui()));
    connect(m_mat_widget->ui->cb_wireframe, SIGNAL(toggled(bool)), this, SLOT(mat_from_ui()));
    connect(m_mat_widget->ui->cmb_cull_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(mat_from_ui()));
    connect(m_mat_widget->ui->sldr_mat_shininess, SIGNAL(valueChanged(int)), this, SLOT(mat_from_ui()));
    connect(m_mat_widget->ui->sb_spec_power, SIGNAL(valueChanged(int)), this, SLOT(mat_from_ui()));

    connect(m_mat_widget->ui->tb_norm_erase, SIGNAL(pressed()), this, SLOT(mat_norm_erase()));
    connect(m_mat_widget->ui->tb_dif_erase, SIGNAL(pressed()), this, SLOT(mat_dif_erase()));
    connect(m_mat_widget->ui->tb_opac_erase, SIGNAL(pressed()), this, SLOT(mat_opac_erase()));
    connect(m_mat_widget->ui->tb_shader_erase, SIGNAL(pressed()), this, SLOT(mat_shader_erase()));

    m_ent_widget->set_preview(m_ui.m_preview);
}

void resource_dialog_prev::init()
{
    m_ui.m_preview->init();
    m_tex_widget->init();
}

uivec2 resource_dialog_prev::get_editing_res()
{
    return m_last_edit;
}


void resource_dialog_prev::mesh_show_verts()
{
    m_ui.m_preview->make_current();

    if (m_editing_res == NULL)
        return;

    nsmesh * msg = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msg == NULL)
        return;

    edit_submesh_data_widget sw(this);
    sw.init(msg, m_mesh_widget->ui->sb_submesh->value()-1);
    sw.exec();
}

void resource_dialog_prev::submesh_index_changed(int index)
{
    m_ui.m_preview->make_current();
    _update_submesh_info(nse.resource<nsmesh>(m_editing_res->full_id()), index-1);
}

void resource_dialog_prev::mesh_toggle_show_center(bool new_val)
{
    m_ui.m_preview->make_current();
    nsentity * prev_center = nse.active()->get<nsentity>("preview_center");
    if (prev_center == NULL)
        return;
    prev_center->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !new_val);
}

bool resource_dialog_prev::set_mesh(const nsstring & model_fname)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_mesh_widget);
 //   m_mesh_widget->clear_fields();
    _reset_fields();

    // make main window context current and get the passed in meshes owning plugin name
    nse.make_current(bbtk.map_view()->glewID());
    nsstring plug_name = nse.active()->name();

    // Now make the preview context current
    m_ui.m_preview->make_current();

    // This is a mesh that has not been loaded in to the engine yet
    m_starting_res = "";
    m_starting_subdir = "";
    m_starting_plug = plug_name;

    // Create a plugin in this context with the same name and set it to active
    nsplugin * plg = nse.create_plugin(plug_name);
    nse.set_active(plg);

    // Now lets set up the mesh in the preview context
    nsmesh * msh = plg->load_model_mesh(model_fname, false);
    if (msh == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Could not find mesh specified with path " + QString(model_fname.c_str()));
        mb.exec();
        on_m_cancel_btn_pressed();
        return false;
    }
    m_editing_res = msh;
    _set_mesh_widget_fields(msh, plg);
    return true;
}

void resource_dialog_prev::mesh_toggle_uv_coords(bool new_val)
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    msh->sub(m_mesh_widget->ui->sb_submesh->value()-1)->m_has_tex_coords = new_val;
}

void resource_dialog_prev::mesh_wireframe_toggled(bool new_val)
{
    m_ui.m_preview->make_current();
    nse.active()->get<nsmaterial>("preview_mat")->enable_wireframe(new_val);
}

bool resource_dialog_prev::set_mesh(nsmesh * mesh_)
{
    if (mesh_ == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Internal error - must set valid mesh. You gotta wonder - who writes these messages anyways??");
        mb.exec();
        return false;
    }

    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_mesh_widget);
 //   m_mesh_widget->clear_fields();
    _reset_fields();

    // make main window context current and get the passed in meshes owning plugin name
    nse.make_current(bbtk.map_view()->glewID());
    nsstring plug_name = nse.plugin(mesh_->plugin_id())->name();

    // indicate that we are editing an already loaded mesh by filling in these fields
    m_starting_res = mesh_->name();
    m_starting_subdir = mesh_->subdir();
    m_starting_plug = plug_name;

    // Now make the preview context current
    m_ui.m_preview->make_current();

    // Create a plugin in this context with the same name and set it to active
    nsplugin * plg = nse.create_plugin(plug_name);
    nse.set_active(plg);

    if (plg == NULL)
    {
        QMessageBox mb(this);
        mb.setWindowTitle("Preview Error");
        mb.setText("Could not load preview plugin - try again");
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        on_m_cancel_btn_pressed();
        return false;
    }

    // Now lets set up the mesh in the preview context
    nsmesh * msh = plg->load<nsmesh>(mesh_->subdir() + mesh_->name() + mesh_->extension());

    // If the mesh is still null it means the mesh could not be loaded from file which most
    // likely means the mesh has not been saved yet - the user created one and proceeded to
    // edit before saving for example. This, in general, should be allowed if the user is willing
    // to save the mesh.
    if (msh == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Cannot edit unsaved resource - would you like to save to file?");
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int ret = mb.exec();
        if (ret == QMessageBox::Yes)
        {
            bbtk.map_view()->make_current();
            nsplugin * eplg = nse.plugin(plug_name);
            eplg->save<nsmesh>(mesh_->name());
            m_ui.m_preview->make_current();
            msh = plg->load<nsmesh>(mesh_->subdir() + mesh_->name() + mesh_->extension());
        }
        else
        {
            on_m_cancel_btn_pressed();
            return false;
        }
    }
    msh->set_icon_path(mesh_->icon_path());
    m_editing_res = msh;
    _set_mesh_widget_fields(msh, plg);
    return true;
}

void resource_dialog_prev::_reset_fields()
{
    m_ui.m_name_le->clear();
    m_ui.m_icon_path_le->clear();
    m_ui.m_folder_le->clear();
    m_ui.m_plugin_le->clear();
    m_ui.m_icon_lbl->clear();
    m_starting_plug.clear();
    m_starting_res.clear();
    m_starting_subdir.clear();
    m_editing_res = NULL;
    setResult(QDialog::Accepted);
}

void resource_dialog_prev::mesh_flip_all_uvs()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    msh->flip_uv();
}

void resource_dialog_prev::mesh_flip_sub_uvs()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    msh->flip_uv(m_mesh_widget->ui->sb_submesh->value()-1);
}

void resource_dialog_prev::mesh_flip_all_norms()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    msh->flip_normals();
}

void resource_dialog_prev::mesh_flip_sub_norms()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    msh->flip_normals(m_mesh_widget->ui->sb_submesh->value()-1);
}

/* MAKE SURE PREVIEW CONTEXT IS SET PREVIEW CALLING THIS FUNCTION!*/
void resource_dialog_prev::_set_mesh_widget_fields(nsmesh * msh, nsplugin * plg)
{
    m_ui.m_preview->make_current();

    // Create the scene for viewing the preview and set it to current
    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    // For the mesh we want focus mode
    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_focus);
    nse.system<nscamera_system>()->set_sensitivity(DEFAULT_CAM_SENSITIVITY*0.02,nscamera_system::sens_strafe);
    nse.system<nscamera_system>()->set_zoom(DEFAULT_CAM_ZOOM_FACTOR*0.03f);

    // Create our directional light and camera and add it to the scene
    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,0.8f,0.1f);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_ui.m_preview->width(), m_ui.m_preview->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
    scn->add(dir_light, fvec3(0,0,-20.0f));
    scn->add(camera, fvec3(0.0f,-4.0f,-4.0f), ::orientation(fvec3(-70.0f,0.0f,0.0f),fvec3::XYZ));
    scn->set_camera(camera);

    on_cb_shadows_toggled(false);

    m_ui.sldr_ambient->setSliderPosition(100);
    m_ui.sldr_diffuse->setSliderPosition(0);

    // Set the background color to light blue - this color will be used later to mask out the background
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,0.0f));

    // Set up a basic material to use for previewing the mesh - by default wireframe enabled
    nsmaterial * mat = plg->create<nsmaterial>("preview_mat");
    mat->set_color_mode(true);
    mat->set_specular(4.0f, 0.5f, fvec3(1,1,1));
    mat->set_color(fvec3(0.7,0.7,0.7));
    mat->enable_wireframe(false);

    // This will be the entity which is used to view the mesh
    nsentity * ent = plg->create<nsentity>("preview_mesh_ent");

    // Render comp set with mesh id
    nsrender_comp * rcomp = ent->create<nsrender_comp>();
    rcomp->set_mesh_id(msh->full_id());
    for (uint i = 0; i < msh->count(); ++i)
        rcomp->set_material(i, mat->full_id(), true);
    ent->create<nssel_comp>();
    scn->add(ent);
    ent->get<nstform_comp>()->enable_snap(false);

    // Make the base tile layer for seeing size comparison
    nsentity * base_tile = plg->create_tile("preview_tile", "diffuseGrass.png", "normalGrass.png", fvec3(),4.0f, 0.1f, fvec3(),true );
    scn->add_gridded(base_tile, ivec3(16,16,1), nstile_grid::world(ivec3(-8,-8,1)));
    base_tile->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !m_mesh_widget->ui->cb_tile_layer->isChecked());


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
    center_point->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !m_mesh_widget->ui->cb_snap_point->isChecked());

    // Setup resource preview dialog to match the mesh
    m_ui.m_plugin_le->setText(plg->name().c_str());
    m_ui.m_name_le->setText(msh->name().c_str());
    m_ui.m_folder_le->setText(msh->subdir().c_str());
    m_ui.m_icon_path_le->setText(msh->icon_path().c_str());
    if (!m_ui.m_icon_path_le->text().isEmpty())
        m_ui.m_icon_lbl->setPixmap(QPixmap(m_ui.m_icon_path_le->text()));

    // Indicate that an mesh from the main context is being edited
    _setup_preview_controls_mesh();
    nse.system<nsinput_system>()->push_context("cam_control");

    m_mesh_widget->ui->lbl_total_verts->setText(QString::number(msh->vert_count()));
    m_mesh_widget->ui->lbl_total_indices->setText(QString::number(msh->indice_count()));
    m_mesh_widget->ui->lbl_submesh_count->setText(QString::number(msh->count()));
    m_mesh_widget->ui->sb_submesh->setMinimum(1);
    m_mesh_widget->ui->sb_submesh->setMaximum(msh->count());
    m_mesh_widget->ui->btn_node_tree->setEnabled(msh->tree()->m_root != NULL);
    m_mesh_widget->ui->btn_joint->setEnabled(msh->tree()->m_name_joint_map.size() > 0);
    _update_submesh_info(msh, 0);
}

void resource_dialog_prev::on_sldr_diffuse_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nslight_comp * lc = prev_lt->get<nslight_comp>();
    lc->set_intensity(float(m_ui.sldr_diffuse->value()) / 100.0f, float(m_ui.sldr_ambient->value()) / 100.0f);
}

void resource_dialog_prev::on_cb_shadows_toggled(bool enable)
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nslight_comp * lc = prev_lt->get<nslight_comp>();
    lc->set_cast_shadows(enable);
}

void resource_dialog_prev::on_sldr_ambient_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nslight_comp * lc = prev_lt->get<nslight_comp>();
    lc->set_intensity(float(m_ui.sldr_diffuse->value()) / 100.0f, float(m_ui.sldr_ambient->value()) / 100.0f);
}

void resource_dialog_prev::on_sldr_horiz_plane_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nstform_comp * tc = prev_lt->get<nstform_comp>();
    tc->enable_parent(true);
    tc->set_parent(rotation_mat4(fvec3(60.0f - m_ui.slider_vert_plane->value() * 0.6f,
                                       0.0f,
                                       m_ui.sldr_horiz_plane->value()*3.6f - 180.0f),
                                 fvec3::ZYX));
}

void resource_dialog_prev::on_slider_vert_plane_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nstform_comp * tc = prev_lt->get<nstform_comp>();
    tc->enable_parent(true);
    tc->set_parent(rotation_mat4(fvec3(60.0f - m_ui.slider_vert_plane->value() * 0.6f,
                                       0.0f,
                                       m_ui.sldr_horiz_plane->value()*3.6f - 180.0f),
                                 fvec3::ZYX));
}

void resource_dialog_prev::mesh_toggle_show_tile_layer(bool new_val)
{
    m_ui.m_preview->make_current();
    nsentity * base_tile = nse.active()->get<nsentity>("preview_tile");
    if (base_tile == NULL)
        return;
    base_tile->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !new_val);
}

void resource_dialog_prev::mesh_view_joints()
{
    if (m_editing_res == NULL)
    {
        QMessageBox mb(this);
        mb.setText("This is one crazy sandwich... there is just no lunchmeat");
        mb.exec();
        return;
    }

    m_ui.m_preview->make_current();
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Out of the fire... and in to the frying pan");
        mb.exec();
        return;
    }

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

    auto iter = msh->tree()->m_name_joint_map.begin();
    while (iter != msh->tree()->m_name_joint_map.end())
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

void resource_dialog_prev::mesh_view_node_tree()
{
    if (m_editing_res == NULL)
    {
        QMessageBox mb(this);
        mb.setText("This is one crazy sandwich... there is just no lunchmeat");
        mb.exec();
        return;
    }
    m_ui.m_preview->make_current();
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Out of the fire... and in to the frying pan");
        mb.exec();
        return;
    }

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
    _add_node_to_tree(msh->tree()->m_root, item);
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

void resource_dialog_prev::_add_node_to_tree(void * node_, QTreeWidgetItem * item)
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

void resource_dialog_prev::_update_submesh_info(nsmesh * msh, uint32 sub_index)
{
    m_mesh_widget->ui->le_submesh_name->setText(msh->sub(sub_index)->m_name.c_str());
    m_mesh_widget->ui->lbl_submesh_verts->setText(QString::number(msh->vert_count(sub_index)));
    m_mesh_widget->ui->lbl_indices->setText(QString::number(msh->indice_count(sub_index)));

    m_mesh_widget->ui->cb_uv_coords->blockSignals(true);
    m_mesh_widget->ui->cb_uv_coords->setChecked(msh->sub(sub_index)->m_has_tex_coords);
    m_mesh_widget->ui->cb_uv_coords->blockSignals(false);

    if (msh->sub(sub_index)->m_node != NULL)
        m_mesh_widget->ui->lbl_submesh_node->setText(msh->sub(sub_index)->m_node->m_name.c_str());
    else
        m_mesh_widget->ui->lbl_submesh_node->setText("None");

    if (msh->sub(sub_index)->m_prim_type == GL_TRIANGLES)
        m_mesh_widget->ui->lbl_prim_type->setText("Triangle");
    else if (msh->sub(sub_index)->m_prim_type == GL_LINES)
        m_mesh_widget->ui->lbl_prim_type->setText("Line");
    else if (msh->sub(sub_index)->m_prim_type == GL_POINTS)
        m_mesh_widget->ui->lbl_prim_type->setText("Point");
    else
        m_mesh_widget->ui->lbl_prim_type->setText("What the crap?");

    m_mesh_widget->ui->lbl_joint_count->setText(QString::number(msh->joint_count()));
    m_mesh_widget->ui->lbl_node_count->setText(QString::number(msh->node_count()));
}

void resource_dialog_prev::reject()
{
    m_ui.m_preview->make_current();
    nsplugin * plg = nse.active();
    nse.system<nsselection_system>()->clear();
    if (plg == nullptr)
        return;
    nse.set_active(nullptr);
    while (nse.plugins()->begin() != nse.plugins()->end())
        nse.destroy_plugin(nse.plugins()->begin()->first);
    m_last_edit = uivec2();
    m_editing_res = nullptr;
    QDialog::reject();
}

bool resource_dialog_prev::set_material(nsmaterial * mat_)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_mat_widget);
    //m_mat_widget->reset_fields();
    _reset_fields();

    nse.make_current(bbtk.map_view()->glewID());

    nsstring plug_name = nse.active()->name();
    if (mat_ != NULL)
        plug_name = nse.plugin(mat_->plugin_id())->name();

    m_ui.m_preview->make_current();

    nsplugin * plg = nse.create_plugin(plug_name);
    nse.set_active(plg);

    if (plg == NULL)
    {
        QMessageBox mb(this);
        mb.setWindowTitle("Preview Error");
        mb.setText("Could not load preview plugin - try again");
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        on_m_cancel_btn_pressed();
        return false;
    }

    // Now lets set up the mesh in the preview context
    nsmaterial * mat;
    if (mat_ != NULL)
    {
        m_starting_res = mat_->name();
        m_starting_subdir = mat_->subdir();
        m_ui.m_name_le->setText(mat_->name().c_str());
        m_ui.m_folder_le->setText(mat_->subdir().c_str());
        m_ui.m_icon_path_le->setText(mat_->icon_path().c_str());

        mat = plg->load<nsmaterial>(mat_->subdir() + mat_->name() + mat_->extension());
        if (mat == NULL)
        {
            QMessageBox mb(this);
            mb.setWindowTitle("Unsaved Resource");
            mb.setIcon(QMessageBox::Question);
            mb.setText("Cannot edit unsaved resource - would you like to save to file?");
            mb.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            int ret = mb.exec();
            if (ret == QMessageBox::Yes)
            {
                bbtk.map_view()->make_current();
                nsplugin * eplg = nse.plugin(plug_name);
                eplg->save<nsmaterial>(mat_->name());
                m_ui.m_preview->make_current();
                mat = plg->load<nsmaterial>(mat_->subdir() + mat_->name() + mat_->extension());
            }
            else
            {
                on_m_cancel_btn_pressed();
                return false;
            }
        }
        mat->set_icon_path(mat_->icon_path());
    }
    else
        mat = plg->create<nsmaterial>("preview_mat");

    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_free);

    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,0.7f,0.3f);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_ui.m_preview->width(), m_ui.m_preview->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));

    on_cb_shadows_toggled(false);

    m_ui.sldr_diffuse->setSliderPosition(70);
    m_ui.sldr_ambient->setSliderPosition(30);

    scn->add(dir_light,fvec3(0,0,-10));
    scn->add(camera, fvec3(0,0,-2.7));
    scn->set_camera(camera);
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,0.0f));

    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_focus);
    nse.system<nscamera_system>()->set_sensitivity(DEFAULT_CAM_SENSITIVITY*0.02,nscamera_system::sens_strafe);
    nse.system<nscamera_system>()->set_zoom(DEFAULT_CAM_ZOOM_FACTOR*0.03f);

    // Setup mesh plane
    nsmesh_plane * plane = plg->create<nsmesh_plane>("preview_plane");
    plane->set_dim(fvec2(2,2));

    // Setup the 2d plane preview entity
    nsentity * plane_ent = plg->create<nsentity>("preview_plane");
    nsrender_comp * rc = plane_ent->create<nsrender_comp>();
    rc->set_mesh_id(plane->full_id());
    if (mat != NULL)
        rc->set_material(0, mat->full_id());

    // Setup the sphere preview entity
    nsentity * sphere_ent = plg->create<nsentity>("preview_sphere");
    rc = sphere_ent->create<nsrender_comp>();
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_SKYDOME)->full_id());
    if (mat != NULL)
        rc->set_material(0, mat->full_id());

    // Setup the tile preview entity
    nsentity * tile_ent = plg->create<nsentity>("preview_tile");
    rc = tile_ent->create<nsrender_comp>();
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_FULL_TILE)->full_id());
    if (mat != NULL)
        rc->set_material(0, mat->full_id());

    // Setup the half tile preview entity
    nsentity * tile_half_ent = plg->create<nsentity>("preview_half_tile");
    rc = tile_half_ent->create<nsrender_comp>();
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_HALF_TILE)->full_id());
    if (mat != NULL)
        rc->set_material(0, mat->full_id());


    _setup_preview_controls_mesh();
    nse.system<nsinput_system>()->push_context("cam_control");

    m_ui.m_plugin_le->setText(plug_name.c_str());
    m_starting_plug = plug_name;

    if (mat != NULL)
    {
        if (!m_ui.m_icon_path_le->text().isEmpty())
            m_ui.m_icon_lbl->setPixmap(QPixmap(m_ui.m_icon_path_le->text()));

        bbtk.map_view()->make_current();
        nstexture * tex_diff = nse.resource<nstexture>(mat->map_tex_id(nsmaterial::diffuse));
        nstexture * tex_norm = nse.resource<nstexture>(mat->map_tex_id(nsmaterial::normal));
        nstexture * tex_opac = nse.resource<nstexture>(mat->map_tex_id(nsmaterial::opacity));
        nsshader * mat_shader = nse.resource<nsshader>(mat->shader_id());

        nsstring diff_plug, norm_plug, shader_plug, opac_plug;
        if (tex_diff != NULL)
        {
            m_mat_widget->ui->le_diffuse_name->setText(tex_diff->name().c_str());
            nsplugin * tmp_plg = nse.plugin(tex_diff->plugin_id());
            diff_plug = tmp_plg->name();
            m_mat_widget->ui->le_diffuse_plugin->setText(tmp_plg->name().c_str());
        }
        if (tex_norm != NULL)
        {
            m_mat_widget->ui->le_normal_name->setText(tex_norm->name().c_str());
            nsplugin * tmp_plg = nse.plugin(tex_norm->plugin_id());
            norm_plug = tmp_plg->name();
            m_mat_widget->ui->le_normal_plugin->setText(tmp_plg->name().c_str());
        }
        if (tex_opac != NULL)
        {
            m_mat_widget->ui->le_opacity_name->setText(tex_opac->name().c_str());
            nsplugin * tmp_plg = nse.plugin(tex_opac->plugin_id());
            opac_plug = tmp_plg->name();
            m_mat_widget->ui->le_opacity_plugin->setText(tmp_plg->name().c_str());
        }
        if (mat_shader != NULL)
        {
            m_mat_widget->ui->le_shader_name->setText(mat_shader->name().c_str());
            nsplugin * tmp_plg = nse.plugin(mat_shader->plugin_id());
            shader_plug = tmp_plg->name();
            m_mat_widget->ui->le_shader_plugin->setText(tmp_plg->name().c_str());
        }
        m_ui.m_preview->make_current();
        if (tex_diff != NULL)
        {
            nsplugin * plgd = nse.plugin(diff_plug);
            if (plgd == NULL)
                plgd = nse.create_plugin(diff_plug,false);

            if (plgd->get<nstexture>(tex_diff->name()) == NULL)
                plgd->load(tex_diff->type(), tex_diff->subdir() + tex_diff->name() + tex_diff->extension());
        }
        if (tex_norm != NULL)
        {
            nsplugin * plgn = nse.plugin(norm_plug);
            if (plgn == NULL)
                plgn = nse.create_plugin(norm_plug,false);

            if (plgn->get<nstexture>(tex_norm->name()) == NULL)
                plgn->load(tex_norm->type(), tex_norm->subdir() + tex_norm->name() + tex_norm->extension());
        }
        if (tex_opac != NULL)
        {
            nsplugin * plgo = nse.plugin(opac_plug);
            if (plgo == NULL)
                plgo = nse.create_plugin(opac_plug,false);

            if (plgo->get<nstexture>(tex_opac->name()) == NULL)
                plgo->load(tex_opac->type(), tex_opac->subdir() + tex_opac->name() + tex_opac->extension());
        }
        if (mat_shader != NULL)
        {
            nsplugin * plgs = nse.plugin(shader_plug);
            if (plgs == NULL)
                plgs = nse.create_plugin(shader_plug,false);

            nsmaterial_shader * m_shd = plgs->get<nsmaterial_shader>(mat_shader->name());
            if (m_shd == NULL)
            {
                m_shd = static_cast<nsmaterial_shader*>(plgs->load(mat_shader->type(), mat_shader->subdir() + mat_shader->name() + mat_shader->extension()));
                m_shd->compile();
                m_shd->link();
                m_shd->init_uniforms();
            }
        }

        m_mat_widget->ui->cb_color_mode->setChecked(mat->color_mode());
        m_mat_widget->ui->cb_cull_enable->setChecked(mat->culling());
        m_mat_widget->ui->sldr_mat_shininess->setValue(mat->specular_intensity()*100.0f);
        m_mat_widget->ui->sb_spec_power->setValue(int(mat->specular_power()));

        QColor color(mat->color().r*255,mat->color().g*255,mat->color().b*255,mat->color().a*255);
        QPixmap px(20, 20);
        px.fill(color);
        m_mat_widget->ui->tb_material_color->setIcon(px);

        QColor scolor(mat->specular_color().r*255,mat->specular_color().g*255,mat->specular_color().b*255,255);
        QPixmap spx(20, 20);
        spx.fill(scolor);
        m_mat_widget->ui->tb_spec_color->setIcon(spx);

        if (mat->cull_mode() == GL_BACK)
            m_mat_widget->ui->cmb_cull_mode->setCurrentIndex(0);
        else if (mat->cull_mode() == GL_FRONT)
            m_mat_widget->ui->cmb_cull_mode->setCurrentIndex(1);
        else
            m_mat_widget->ui->cmb_cull_mode->setCurrentIndex(2);

        m_editing_res = mat;

        mat_preview_mesh_changed(m_mat_widget->ui->cmb_preview_mesh->currentIndex());
    }
    else
    {
        QMessageBox mb(this);
        mb.setWindowTitle("Preview Error");
        mb.setIcon(QMessageBox::Question);
        mb.setText("Could not load material at all - please report bug");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
        on_m_cancel_btn_pressed();
        return false;
    }
    return true;
    // main camera controls
}

void resource_dialog_prev::mat_from_ui()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;
    cur_mat->set_specular_power(float(m_mat_widget->ui->sb_spec_power->value()));
    cur_mat->set_specular_intensity(float(m_mat_widget->ui->sldr_mat_shininess->value()) / 100.0f);
    cur_mat->set_color_mode(m_mat_widget->ui->cb_color_mode->isChecked());
    cur_mat->enable_culling(m_mat_widget->ui->cb_cull_enable->isChecked());
    cur_mat->enable_wireframe(m_mat_widget->ui->cb_wireframe->isChecked());
    switch (m_mat_widget->ui->cmb_cull_mode->currentIndex())
    {
    case(0):
        cur_mat->set_cull_mode(GL_BACK);
        break;
    case(1):
        cur_mat->set_cull_mode(GL_FRONT);
        break;
    default:
        cur_mat->set_cull_mode(GL_FRONT_AND_BACK);
        break;
    }
}

void resource_dialog_prev::mat_color_tb()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    QColor c(cur_mat->color().r*255,cur_mat->color().g*255,cur_mat->color().b*255,cur_mat->color().a*255);
    QColorDialog cd(c,this);
    cd.setWindowTitle("Choose Material Color");
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    if (cd.exec() == QDialog::Accepted)
        c = cd.currentColor();

    QPixmap px(20, 20);
    px.fill(c);
    m_mat_widget->ui->tb_material_color->setIcon(px);
    m_mat_widget->ui->tb_material_color->setDown(false);
    cur_mat->set_color(fvec4(c.redF(),c.greenF(),c.blueF(),c.alphaF()));
}

void resource_dialog_prev::mat_spec_color_tb()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    QColor c(cur_mat->specular_color().r*255,cur_mat->specular_color().g*255,cur_mat->specular_color().b*255,255);
    QColorDialog cd(c,this);
    cd.setWindowTitle("Choose Specular Color");
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    if (cd.exec() == QDialog::Accepted)
        c = cd.currentColor();

    QPixmap px(20, 20);
    px.fill(c);
    m_mat_widget->ui->tb_spec_color->setIcon(px);
    m_mat_widget->ui->tb_spec_color->setDown(false);
    cur_mat->set_specular_color(fvec3(c.redF(),c.greenF(),c.blueF()));
}

void resource_dialog_prev::mat_btn_shader()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    bbtk.map_view()->make_current();

    nsmaterial_shader * mat_shader = NULL;
    select_res_dialog rd(this);
    nsstring plug,name;
    rd.show_type<nsmaterial_shader>();
    if (rd.exec() == QDialog::Accepted)
    {
        mat_shader = rd.selected_resource<nsmaterial_shader>();
        plug = nse.plugin(mat_shader->plugin_id())->name();
        name = mat_shader->name();
        m_mat_widget->ui->le_shader_plugin->setText(plug.c_str());
        m_mat_widget->ui->le_shader_name->setText(name.c_str());
    }
    else
        return;

    m_ui.m_preview->make_current();
    if (mat_shader != NULL)
    {
        nsplugin * plg = nse.plugin(plug);
        if (plg == NULL)
            plg = nse.create_plugin(plug,false);
        nsmaterial_shader * m_shd = plg->get<nsmaterial_shader>(name);
        if (m_shd == NULL)
        {
            m_shd = static_cast<nsmaterial_shader*>(plg->load(mat_shader->type(), mat_shader->subdir() + mat_shader->name() + mat_shader->extension()));
            m_shd->compile();
            m_shd->link();
            m_shd->init_uniforms();
        }

        cur_mat->set_shader_id(m_shd->full_id());
    }
}

void resource_dialog_prev::mat_btn_diffuse()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    bbtk.map_view()->make_current();

    nstexture * tex_diff = NULL;
    select_res_dialog rd(this);
    nsstring plug,name;
    rd.show_type<nstexture>();
    if (rd.exec() == QDialog::Accepted)
    {
        bbtk.map_view()->make_current();
        tex_diff = rd.selected_resource<nstexture>();
        plug = nse.plugin(tex_diff->plugin_id())->name();
        name = tex_diff->name();
        m_mat_widget->ui->le_diffuse_plugin->setText(plug.c_str());
        m_mat_widget->ui->le_diffuse_name->setText(name.c_str());
    }
    else
        return;

    m_ui.m_preview->make_current();
    if (tex_diff != NULL)
    {
        nsplugin * plg = nse.plugin(plug);
        if (plg == NULL)
            plg = nse.create_plugin(plug,false);

        nsresource * tex = plg->get<nstexture>(name);
        if (tex == NULL)
            tex = plg->load(tex_diff->type(), tex_diff->subdir() + tex_diff->name() + tex_diff->extension());
        cur_mat->set_map_tex_id(nsmaterial::diffuse, tex->full_id(), true);
    }
}

void resource_dialog_prev::mat_btn_normal()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    bbtk.map_view()->make_current();

    nstexture * tex_norm = NULL;
    select_res_dialog rd(this);
    nsstring plug,name;
    rd.show_type<nstexture>();
    if (rd.exec() == QDialog::Accepted)
    {
        bbtk.map_view()->make_current();
        tex_norm = rd.selected_resource<nstexture>();
        plug = nse.plugin(tex_norm->plugin_id())->name();
        name = tex_norm->name();
        m_mat_widget->ui->le_normal_plugin->setText(plug.c_str());
        m_mat_widget->ui->le_normal_name->setText(name.c_str());
    }
    else
        return;

    m_ui.m_preview->make_current();
    if (tex_norm != NULL)
    {
        nsplugin * plg = nse.plugin(plug);
        if (plg == NULL)
            plg = nse.create_plugin(plug,false);

        nsresource * tex = plg->get<nstexture>(name);
        if (tex == NULL)
            tex = plg->load(tex_norm->type(), tex_norm->subdir() + tex_norm->name() + tex_norm->extension());
        cur_mat->set_map_tex_id(nsmaterial::normal, tex->full_id(), true);
    }
}

void resource_dialog_prev::mat_dif_erase()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    cur_mat->set_map_tex_id(nsmaterial::diffuse,uivec2(),true);
    m_mat_widget->ui->le_diffuse_name->clear();
    m_mat_widget->ui->le_diffuse_plugin->clear();
    m_mat_widget->ui->tb_dif_erase->setDown(false);
}

void resource_dialog_prev::mat_norm_erase()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    cur_mat->set_map_tex_id(nsmaterial::normal,uivec2(),true);
    m_mat_widget->ui->le_normal_name->clear();
    m_mat_widget->ui->le_normal_plugin->clear();
    m_mat_widget->ui->tb_norm_erase->setDown(false);
}

void resource_dialog_prev::mat_opac_erase()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    cur_mat->set_map_tex_id(nsmaterial::opacity,uivec2(),true);
    m_mat_widget->ui->le_opacity_name->clear();
    m_mat_widget->ui->le_opacity_plugin->clear();
    m_mat_widget->ui->tb_opac_erase->setDown(false);
}

void resource_dialog_prev::mat_shader_erase()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    cur_mat->set_shader_id(uivec2());
    m_mat_widget->ui->le_shader_name->clear();
    m_mat_widget->ui->le_shader_plugin->clear();
    m_mat_widget->ui->tb_shader_erase->setDown(false);
}

void resource_dialog_prev::mat_btn_opac()
{
    m_ui.m_preview->make_current();
    if (m_editing_res == NULL)
        return;
    nsmaterial * cur_mat = nse.resource<nsmaterial>(m_editing_res->full_id());
    if (cur_mat == NULL)
        return;

    bbtk.map_view()->make_current();

    nstexture * tex_opac = NULL;
    select_res_dialog rd(this);
    nsstring plug,name;
    rd.show_type<nstexture>();
    if (rd.exec() == QDialog::Accepted)
    {
        bbtk.map_view()->make_current();
        tex_opac = rd.selected_resource<nstexture>();
        plug = nse.plugin(tex_opac->plugin_id())->name();
        name = tex_opac->name();
        m_mat_widget->ui->le_opacity_plugin->setText(plug.c_str());
        m_mat_widget->ui->le_opacity_name->setText(name.c_str());
    }
    else
        return;

    m_ui.m_preview->make_current();
    if (tex_opac != NULL)
    {
        nsplugin * plg = nse.plugin(plug);
        if (plg == NULL)
            plg = nse.create_plugin(plug,false);

        nsresource * tex = plg->get<nstexture>(name);
        if (tex == NULL)
            tex = plg->load(tex_opac->type(), tex_opac->subdir() + tex_opac->name() + tex_opac->extension());
        cur_mat->set_map_tex_id(nsmaterial::opacity, tex->full_id(), true);
    }
}

void resource_dialog_prev::mat_preview_mesh_changed(int new_index)
{
    m_ui.m_preview->make_current();
    nsscene *scn = nse.current_scene();
    scn->remove(nse.active(),"preview_plane");
    scn->remove(nse.active(),"preview_sphere");
    scn->remove(nse.active(),"preview_tile");
    scn->remove(nse.active(),"preview_half_tile");
    switch (new_index)
    {
    case(0):
        scn->add(nse.active(),"preview_plane");
        break;
    case(1):
        scn->add(nse.active(),"preview_tile");
        break;
    case(2):
        scn->add(nse.active(),"preview_half_tile");
        break;
    case(3):
        scn->add(nse.active(),"preview_sphere", fvec3(), fquat(), fvec3(0.25,0.25,0.25));
        break;
    case(4):
        scn->add(nse.active(),"preview_sphere", fvec3(), fquat(), fvec3(0.25,0.25,0.25));
        break;
    }
}

bool resource_dialog_prev::set_texture(nstexture * tex_)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_tex_widget);
    m_tex_widget->clear_fields();
    _reset_fields();

    nse.make_current(bbtk.map_view()->glewID());

    nsstring plug_name = nse.active()->name();
    if (tex_ != NULL)
        plug_name = nse.plugin(tex_->plugin_id())->name();

    m_ui.m_preview->make_current();

    nsplugin * plg = nse.create_plugin(plug_name);
    nse.set_active(plg);

    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_free);

    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,0.0f,1.0f);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_ui.m_preview->width(), m_ui.m_preview->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));

    m_ui.sldr_ambient->setSliderPosition(100);
    m_ui.sldr_diffuse->setSliderPosition(0);

    scn->add(dir_light,fvec3(-30,20,20));
    scn->set_camera(camera);
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,0.0f));

    // Setup mesh plane
    nsmesh_plane * plane = plg->create<nsmesh_plane>("2dplane");
    plane->set_dim(fvec2(12,12));

    // Setup material for the plane
    nsmaterial * tex_mat = plg->create<nsmaterial>("2dplane");
    tex_mat->set_color(fvec4(BG_R,BG_G,BG_B,1.0f));
    tex_mat->enable_culling(false);

    // Setup skybox material
    nsmaterial * sb_mat = plg->create<nsmaterial>("skydome");
    sb_mat->set_shader_id(nse.core()->get<nsshader>(DEFAULT_SKYBOX_SHADER)->full_id());
    sb_mat->set_cull_mode(GL_FRONT);

    // Setup the 2d plane preview entity
    nsentity * plane_ent = plg->create<nsentity>("2dplane");
    nsrender_comp * rc = plane_ent->create<nsrender_comp>();
    rc->set_mesh_id(plane->full_id());
    rc->set_material(0, tex_mat->full_id());

    // Setup the skybox preview entity
    nsentity * cubemap_ent = plg->create<nsentity>("skydome");
    rc = cubemap_ent->create<nsrender_comp>();
    rc->set_cast_shadow(false);
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_SKYDOME)->full_id());
    rc->set_material(0, sb_mat->full_id());
    _setup_preview_controls_tex();

    m_ui.m_plugin_le->setText(plug_name.c_str());
    m_starting_plug = plug_name;

    if (tex_ != NULL)
    {
        m_ui.m_name_le->setText(tex_->name().c_str());
        m_ui.m_folder_le->setText(tex_->subdir().c_str());
        m_ui.m_icon_path_le->setText(tex_->icon_path().c_str());

        if (!m_ui.m_icon_path_le->text().isEmpty())
            m_ui.m_icon_lbl->setPixmap(QPixmap(m_ui.m_icon_path_le->text()));

        m_tex_widget->ui->m_type_lbl->setEnabled(false);
        m_tex_widget->ui->m_tex_type_cmb->setEnabled(false);

        m_starting_res = tex_->name();
        m_starting_subdir = tex_->subdir();

        if (tex_->texture_type() == nstexture::tex_cubemap)
        {
            nstex_manager * tm = plg->manager<nstex_manager>();
            QString fname = QString( (tm->res_dir() + tm->local_dir() + tex_->subdir() + tex_->name()).c_str());
            m_tex_widget->ui->m_cm_front_le->setText(fname + "_front.png");
            m_tex_widget->ui->m_cm_back_le->setText(fname + "_back.png");
            m_tex_widget->ui->m_cm_top_le->setText(fname + "_top.png");
            m_tex_widget->ui->m_cm_bottom_le->setText(fname + "_bottom.png");
            m_tex_widget->ui->m_cm_left_le->setText(fname + "_left.png");
            m_tex_widget->ui->m_cm_right_le->setText(fname + "_right.png");
            if (m_tex_widget->ui->m_tex_type_cmb->currentIndex() != 1)
                m_tex_widget->ui->m_tex_type_cmb->setCurrentIndex(1);
            else
                tex_cubemap_triggered();

            if (result() == Rejected)
                return false;
        }
        else
        {
            nstex_manager * tm = plg->manager<nstex_manager>();
            nsstring fname = tm->res_dir() + tm->local_dir() + tex_->subdir() + tex_->name() + tex_->extension();
            m_tex_widget->ui->m_image_file_le->setText(fname.c_str());

            if (m_tex_widget->ui->m_tex_type_cmb->currentIndex() != 0)
                m_tex_widget->ui->m_tex_type_cmb->setCurrentIndex(0);
            else
                tex_tex2d_triggered();

            if (result() == Rejected)
                return false;
        }

    }
    else
    {
        m_tex_widget->ui->m_type_lbl->setEnabled(true);
        m_tex_widget->ui->m_tex_type_cmb->setEnabled(true);
        tex_tex2d_triggered();
        if (result() == Rejected)
            return false;
    }
    return true;
    // main camera controls
}

void resource_dialog_prev::tex_cubemap_triggered()
{
    m_ui.m_preview->make_current();

    nsplugin * plg = nse.active();
    nsscene * scn = plg->current_scene();
    nsentity * dir_light = plg->get<nsentity>(PREV_LIGHT_NAME);
    nsentity * camera = plg->get<nsentity>(PREV_CAM_NAME);
    nsentity * skydome_ent = plg->get<nsentity>("skydome");

    scn->clear();
    scn->set_camera(camera);
    camera->get<nstform_comp>()->set_pos(fvec3());
    camera->get<nstform_comp>()->set_orientation(::orientation(fvec4(1,0,0,-90)));
    camera->get<nscam_comp>()->post_update(true);

    scn->add(dir_light, fvec3(-30,20,20));

    nse.system<nsinput_system>()->push_context("cam_control");
    scn->set_skydome(skydome_ent);

    nsstring_vector svec;
    svec.resize(6);
    svec[0] = m_tex_widget->ui->m_cm_front_le->text().toStdString();
    svec[1] = m_tex_widget->ui->m_cm_back_le->text().toStdString();
    svec[2] = m_tex_widget->ui->m_cm_top_le->text().toStdString();
    svec[3] = m_tex_widget->ui->m_cm_bottom_le->text().toStdString();
    svec[4] = m_tex_widget->ui->m_cm_left_le->text().toStdString();
    svec[5] = m_tex_widget->ui->m_cm_right_le->text().toStdString();

    nsstring start = "./resources/core/textures/default.png";
    ui_vector inds;
    for (uint32 i = 0; i < 6; ++i)
    {
        if (svec[i].empty())
            inds.push_back(i);
        else
            start = svec[i];
    }

    for (uint32 i = 0; i < inds.size(); ++i)
        svec[inds[i]] = start;

    // Check to make sure all same dimensions
    QPixmap pix;
    pix.load(start.c_str());
    QSize basesz = pix.size();

    if (basesz.width() != basesz.height())
    {
        QMessageBox mb(this);
        mb.setText("Cubemaps must have the same width and height");
        mb.exec();
        return;
    }

    for (uint32 i = 0; i < 6; ++i)
    {
        if (svec[i] != start)
        {
            pix.load(svec[i].c_str());
            if (basesz != pix.size())
                return;
        }
    }
    nstex_manager * mgr = plg->manager<nstex_manager>();

    nstexture * tex = plg->get<nstexture>("preview_tex");
    if (tex != NULL)
        plg->destroy<nstexture>("preview_tex");

    tex = mgr->load_cubemap(svec[0],svec[1],svec[2],svec[3],svec[4],svec[5],"preview_tex.cube");
    if (tex != NULL)
    {
        nsmaterial * mat = plg->get<nsmaterial>("skydome");
        mat->set_map_tex_id(nsmaterial::diffuse, tex->full_id(), true);
        m_editing_res = tex;
    }
    else
    {
        QMessageBox mb(this);
        mb.setText("Could not find image file specified!");
        mb.exec();
    }
}

void resource_dialog_prev::tex_tex2d_triggered()
{
    m_ui.m_preview->make_current();

    nsplugin * plg = nse.active();
    nsscene * scn = plg->current_scene();
    nsentity * dir_light = plg->get<nsentity>(PREV_LIGHT_NAME);
    nsentity * camera = plg->get<nsentity>(PREV_CAM_NAME);
    nsentity * plane_ent = plg->get<nsentity>("2dplane");

    scn->clear();
    scn->set_camera(camera);
    camera->get<nstform_comp>()->set_pos(fvec3(0,0,-10));
    camera->get<nstform_comp>()->set_orientation(fquat());
    camera->get<nscam_comp>()->post_update(true);

    scn->add(dir_light, fvec3(-30,20,20));

    nse.system<nsinput_system>()->pop_context();
    scn->add(plane_ent,fvec3(0,0,4));

    nsstring fname = m_tex_widget->ui->m_image_file_le->text().toStdString();
    if (fname.empty())
        return;

    nstexture * tex = plg->get<nstexture>("preview_tex");
    if (tex != NULL)
        plg->destroy<nstexture>("preview_tex");
    tex = plg->load<nstex2d>(fname);

    if (tex != NULL)
    {
        tex->rename("preview_tex");
        nsmaterial * mat = plg->get<nsmaterial>("2dplane");
        mat->set_map_tex_id(nsmaterial::diffuse, tex->full_id(),true);
        m_editing_res = tex;
    }
    else
    {
        QMessageBox mb(this);
        mb.setText("The image you selected has not yet been saved to the plugin folder - it has to be saved in order to edit - would you like to save and edit?");
        mb.setIcon(QMessageBox::Warning);
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        mb.setWindowTitle("Editing Texture");
        int ret = mb.exec();
        if (ret == QMessageBox::Yes)
        {
            bbtk.map_view()->make_current();
            nsplugin * eplg = nse.plugin(m_starting_plug);
            eplg->save<nstex2d>(m_starting_res);
            m_ui.m_preview->make_current();
            tex = plg->load<nstex2d>(fname);
            if (tex == NULL)
            {
                mb.setText("Still could not save texture - please submit an issue report");
                mb.setStandardButtons(QMessageBox::Ok);
                mb.exec();
                on_m_cancel_btn_pressed();
            }
            tex->rename("preview_tex");
            nsmaterial * mat = plg->get<nsmaterial>("2dplane");
            mat->set_map_tex_id(nsmaterial::diffuse, tex->full_id(),true);
            m_editing_res = tex;
        }
        else
        {
            on_m_cancel_btn_pressed();
        }
    }
}

void resource_dialog_prev::_setup_preview_controls_tex()
{
    nsinput_map * imap = nse.active()->create<nsinput_map>("resource_dialog_prev");
    imap->create_context("cam_control");

    nsinput_map::trigger camtilt(
        NSCAM_TILTPAN,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camtilt.add_key_mod(nsinput_map::key_any);
    camtilt.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement, camtilt);

    nsinput_map::trigger cammove(
        NSCAM_MOVE,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos
        );
    cammove.add_key_mod(nsinput_map::key_any);
    cammove.add_mouse_mod(nsinput_map::left_button);
    cammove.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,cammove);

    nsinput_map::trigger camzoom(
        NSCAM_ZOOM,
        nsinput_map::t_pressed,
        nsinput_map::axis_scroll_delta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camzoom.add_key_mod(nsinput_map::key_any);
    imap->add_mouse_trigger("cam_control", nsinput_map::scrolling, camzoom);

    nse.system<nsinput_system>()->set_input_map(imap->full_id());
}

void resource_dialog_prev::_setup_preview_controls_mesh()
{
    nsinput_map * imap = nse.active()->create<nsinput_map>("resource_dialog_prev");
    imap->create_context("cam_control");

    nsinput_map::trigger camtilt(
        NSCAM_TILTPAN,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camtilt.add_key_mod(nsinput_map::key_any);
    camtilt.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement, camtilt);

    nsinput_map::trigger cammove(
        NSCAM_MOVE,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos
        );
    cammove.add_key_mod(nsinput_map::key_any);
    cammove.add_mouse_mod(nsinput_map::left_button);
    cammove.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,cammove);

    nsinput_map::trigger camzoom(
        NSCAM_ZOOM,
        nsinput_map::t_pressed,
        nsinput_map::axis_scroll_delta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camzoom.add_key_mod(nsinput_map::key_any);
    imap->add_mouse_trigger("cam_control", nsinput_map::scrolling, camzoom);

    nsinput_map::trigger selectentity(
        NSSEL_SELECT,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos
        );
    imap->add_mouse_trigger("cam_control", nsinput_map::left_button,selectentity);

    nsinput_map::trigger selectmove(
        NSSEL_MOVE,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmove.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmove);

    nsinput_map::trigger selectmovexy(
        NSSEL_MOVE_XY,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovexy.add_key_mod(nsinput_map::key_x);
    selectmovexy.add_key_mod(nsinput_map::key_y);
    selectmovexy.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovexy);

    nsinput_map::trigger selectmovezy(
        NSSEL_MOVE_ZY,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovezy.add_key_mod(nsinput_map::key_z);
    selectmovezy.add_key_mod(nsinput_map::key_y);
    selectmovezy.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovezy);

    nsinput_map::trigger selectmovezx(
        NSSEL_MOVE_ZX,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovezx.add_key_mod(nsinput_map::key_z);
    selectmovezx.add_key_mod(nsinput_map::key_x);
    selectmovezx.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovezx);

    nsinput_map::trigger selectmovex(
        NSSEL_MOVE_X,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovex.add_mouse_mod(nsinput_map::left_button);
    selectmovex.add_key_mod(nsinput_map::key_x);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovex);

    nsinput_map::trigger selectmovey(
        NSSEL_MOVE_Y,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovey.add_key_mod(nsinput_map::key_y);
    selectmovey.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovey);

    nsinput_map::trigger selectmovez(
        NSSEL_MOVE_Z,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovez.add_key_mod(nsinput_map::key_z);
    selectmovez.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovez);

    nsinput_map::trigger selectmovetoggle(
        NSSEL_MOVE_TOGGLE,
        nsinput_map::t_toggle
        );
    selectmovetoggle.add_key_mod(nsinput_map::key_any);
    imap->add_mouse_trigger("cam_control", nsinput_map::left_button,selectmovetoggle);

    nse.system<nsinput_system>()->set_input_map(imap->full_id());
}

void resource_dialog_prev::on_m_okay_btn_pressed()
{
    m_ui.m_preview->make_current();
    nsplugin * plg = nse.active();
    if (plg == nullptr)
        return;

    if (m_editing_res != NULL)
    {
        nsstring new_name = m_ui.m_name_le->text().toStdString();

        QString subdir = m_ui.m_folder_le->text();
        if (!subdir.isEmpty() && subdir[subdir.size()-1] != '/')
            subdir.append('/');

        // validate our resource stuff
        if (m_starting_res.empty())
        {
            nsstring name = m_ui.m_name_le->text().toStdString();
            nsstring plugin = m_ui.m_plugin_le->text().toStdString();

            if (name.empty())
            {
                QMessageBox mb(this);
                mb.setText("Please choose a name for your resource");
                mb.setWindowTitle("Name Error");
                mb.setIcon(QMessageBox::Warning);
                mb.exec();
                return;
            }

            if (plugin.empty())
            {
                QMessageBox mb(this);
                mb.setText("Please choose a valid plugin");
                mb.setWindowTitle("Name Error");
                mb.setIcon(QMessageBox::Warning);
                mb.exec();
                return;
            }

            bbtk.map_view()->make_current();
            if (nse.resource(m_editing_res->type(), nse.plugin(plugin),name) != NULL)
            {
                QMessageBox mb(this);
                mb.setText(("The " + nse.guid(m_editing_res->type()).substr(2) + " \"" + name + "\" already exists in plugin " + plugin + " - please choose a different name").c_str());
                mb.setWindowTitle("Name Conflict");
                mb.setIcon(QMessageBox::Warning);
                mb.exec();
                return;
            }
            m_ui.m_preview->make_current();
        }

        m_editing_res->set_subdir(subdir.toStdString());
        m_editing_res->rename(new_name);
        nsstring fname = m_editing_res->subdir() + m_editing_res->name() + m_editing_res->extension();
        uint32 res_type_id = m_editing_res->type();

        // remove the original resource from file and from being loaded in to the main window context
        if (!m_starting_res.empty())
        {
            // Make main window's context current
            bbtk.map_view()->make_current();

            nsplugin * eng_plug = nse.plugin(m_starting_plug);
            nsresource * res = eng_plug->get(res_type_id, m_starting_res);
            eng_plug->del(res);

            // Propagate name change so that deleted resource is replaced by new resource
            nse.name_change(uivec2(hash_id(m_starting_plug),hash_id(m_starting_res)), m_editing_res->full_id());

            // Switch back to the preview's context
            m_ui.m_preview->make_current();
        }

        // If the plugin name le is different than the current plugin name, rename the current plugin
        nsstring new_plg_name = m_ui.m_plugin_le->text().toStdString();
        if (plg->name() != new_plg_name)
            plg->rename(new_plg_name);

        nsentity * ent = plg->get<nsentity>("preview_mesh_ent");
        if (ent != NULL)
        {
            nsmesh * msh_v = plg->get<nsmesh>(m_editing_res->id());
            if (msh_v == NULL)
            {
                QMessageBox mb(this);
                mb.setText("A mesh entity exists but there is no mesh - probably a bug in deleting stuff!");
                mb.setWindowTitle("Error");
                mb.exec();
            }
            else
            {
                nstform_comp * tc = ent->get<nstform_comp>();
                if (tc == NULL)
                {
                    QMessageBox mb(this);
                    mb.setText("The preview entity has no friggin tform - this can't be good!");
                    mb.setWindowTitle("Error");
                    mb.exec();
                }
                else
                {
                    if (msh_v->tree()->m_root == NULL)
                    {
                        msh_v->bake_scaling(tc->scaling());
                        msh_v->bake_rotation(tc->orientation());
                        msh_v->bake_translation(tc->wpos());
                    }
                    else
                    {
                        msh_v->bake_node_scaling(tc->scaling());
                        msh_v->bake_node_rotation(tc->orientation());
                        msh_v->bake_node_translation(tc->wpos());
                    }
                }
            }
        }
        // Now save under this plugin name
        plg->save(m_editing_res);

        // Get the icon path from the icon path le and attempt to save the generated icon to file
        // If that fails, set the path to empty
        nsstring icon_pth = m_ui.m_icon_path_le->text().toStdString();
        if (!icon_pth.empty())
        {
            nsfile_os::create_dir(icon_pth);
            if (!m_ui.m_icon_lbl->pixmap()->save(icon_pth.c_str()))
                icon_pth = "";
        }

        // Make main window context current and load the previously saved resource from file
        bbtk.map_view()->make_current();
        nsplugin * new_plug = nse.plugin(new_plg_name);
        if (new_plug == NULL)
        {
            QMessageBox mb(this);
            mb.setText("Could not find plugin file specified!");
            mb.setWindowTitle("Plugin Error");
            mb.exec();
        }
        else
        {
            nsresource * res = new_plug->load(res_type_id, fname);
            if (res == NULL)
            {
                QMessageBox mb(this);
                mb.setText("Could not load resource in to plugin - Some kind of mischeif has crept in to the system!");
                mb.setWindowTitle("Plugin Error");
                mb.exec();
            }

            // Set resources icon path - if the generated icon was unable to save this will just be an empty string
            res->set_icon_path(icon_pth);
        }

        // Make the preview's context current
        m_ui.m_preview->make_current();
    }
    m_last_edit = m_editing_res->full_id();

    // Now destroy all resources in the plugin and reset the editing resource
    nse.set_active(nullptr);
    nse.system<nsselection_system>()->clear();
    nse.destroy_plugin(plg);

    while (nse.plugins()->begin() != nse.plugins()->end())
        nse.destroy_plugin(nse.plugins()->begin()->first);

    m_editing_res = nullptr;
    bbtk.map_view()->update();
    accept();
}

void resource_dialog_prev::on_m_cancel_btn_pressed()
{
    reject();
}

void resource_dialog_prev::on_m_plugin_tb_pressed()
{
    bbtk.map_view()->make_current();
    select_res_dialog get_plug(this);
    get_plug.show_type<nsplugin>();
    if (get_plug.exec() == QDialog::Accepted)
    {
        nsplugin * plg = get_plug.selected_resource<nsplugin>();
        m_ui.m_plugin_le->setText(plg->name().c_str());
    }
    m_ui.m_preview->make_current();
}

void resource_dialog_prev::on_m_folder_tb_pressed()
{
    QString fname = QFileDialog::getExistingDirectory(this,
                                                 "Resource Subdirectory",
                                                 "./resources/" + m_ui.m_plugin_le->text(),
                                                 QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly);
    nsstring to_set = nsres_manager::name_from_filename(fname.toStdString());
    if (!to_set.empty())
        to_set += '/';
    m_ui.m_folder_le->setText(to_set.c_str());
}

void resource_dialog_prev::on_m_icon_path_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    m_ui.m_icon_path_le->setText(fname);
    QPixmap pm;
    if (pm.load(fname))
        m_ui.m_icon_lbl->setPixmap(pm);
}


void resource_dialog_prev::on_m_icon_create_btn_pressed()
{
    nsstring name = m_ui.m_name_le->text().toStdString();

    if (m_editing_res == NULL)
    {
        QMessageBox mb(this);
        mb.setText("The editing resource is NULL - just like the brain of Alex! What did you do???");
        mb.setWindowTitle("Icon Error");
        mb.exec();
        return;
    }

    if (name.empty())
    {
        QMessageBox mb(this);
        mb.setText("Please provide a name for the resource before making the icon");
        mb.setWindowTitle("Icon Error");
        mb.exec();
        return;
    }

    QPixmap pixMap = QPixmap::grabWidget(m_ui.m_preview).scaled(96,96);// = QPixmap::grabWidget(m_ui.m_preview).scaled(70,70);
    pixMap.setMask(pixMap.createMaskFromColor(QColor(int(BG_R*255),int(BG_G*255),int(BG_B*255),0)));

    bbtk.map_view()->make_current();
    nsplugin * plg = nse.plugin(m_ui.m_plugin_le->text().toStdString());
    if (plg == NULL)
    {
        m_ui.m_preview->make_current();
        QMessageBox mb(this);
        mb.setText("Could not find plugin specified - Cannot create icon!");
        mb.setWindowTitle("Icon Error");
        mb.exec();
        return;
    }

    nsstring icon_path = plg->manager<nstex_manager>()->res_dir() +
            plg->manager<nstex_manager>()->local_dir() +
            "icons/" + name + ".png";
    m_ui.m_preview->make_current();
    m_ui.m_icon_lbl->setPixmap(pixMap);
    m_ui.m_icon_path_le->setText(icon_path.c_str());
}

void resource_dialog_prev::mesh_translate()
{
    if (m_editing_res == NULL)
        return;
    m_ui.m_preview->make_current();
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == NULL)
        return;
    nstform_comp * tc = ent->get<nstform_comp>();
    tc->set_pos(fvec3(m_mesh_widget->ui->dsb_offset_x->value(),
                      m_mesh_widget->ui->dsb_offset_y->value(),
                      m_mesh_widget->ui->dsb_offset_z->value()));
}

void resource_dialog_prev::mesh_scale(double new_val)
{
    if (m_editing_res == NULL)
        return;
    m_ui.m_preview->make_current();
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == NULL)
        return;
    nstform_comp * tc = ent->get<nstform_comp>();

    if (m_mesh_widget->ui->cb_constrain_dim->isChecked())
    {
        m_mesh_widget->ui->dsb_scale_x->blockSignals(true);
        m_mesh_widget->ui->dsb_scale_x->setValue(new_val);
        m_mesh_widget->ui->dsb_scale_x->blockSignals(false);

        m_mesh_widget->ui->dsb_scale_y->blockSignals(true);
        m_mesh_widget->ui->dsb_scale_y->setValue(new_val);
        m_mesh_widget->ui->dsb_scale_y->blockSignals(false);

        m_mesh_widget->ui->dsb_scale_z->blockSignals(true);
        m_mesh_widget->ui->dsb_scale_z->setValue(new_val);
        m_mesh_widget->ui->dsb_scale_z->blockSignals(false);
    }
    tc->set_scale(fvec3( m_mesh_widget->ui->dsb_scale_x->value(),
                         m_mesh_widget->ui->dsb_scale_y->value(),
                         m_mesh_widget->ui->dsb_scale_z->value()));
}

void resource_dialog_prev::mesh_rotate(int)
{
    if (m_editing_res == NULL)
        return;
    m_ui.m_preview->make_current();
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == NULL)
        return;
    nstform_comp * tc = ent->get<nstform_comp>();
    fvec3::EulerOrder eo = fvec3::EulerOrder(m_mesh_widget->ui->cmb_euler_order->currentIndex());

    fmat4 rotmat = ::rotation_mat4(fvec3(m_mesh_widget->ui->dsb_rot_x->value(),m_mesh_widget->ui->dsb_rot_y->value(),m_mesh_widget->ui->dsb_rot_z->value()),eo,false);
    tc->set_orientation(fquat().from(rotmat));
}

void resource_dialog_prev::preview_updated()
{
    if (m_editing_res == NULL)
        return;
    m_ui.m_preview->make_current();
    nsmesh * msh = nse.resource<nsmesh>(m_editing_res->full_id());
    if (msh == NULL)
        return;
    nsentity * ent = nse.active()->get<nsentity>("preview_mesh_ent");
    if (ent == NULL)
        return;

    nstform_comp * tc = ent->get<nstform_comp>();

    m_mesh_widget->ui->dsb_offset_x->blockSignals(true);
    m_mesh_widget->ui->dsb_offset_x->setValue(tc->wpos().x);
    m_mesh_widget->ui->dsb_offset_x->blockSignals(false);

    m_mesh_widget->ui->dsb_offset_y->blockSignals(true);
    m_mesh_widget->ui->dsb_offset_y->setValue(tc->wpos().y);
    m_mesh_widget->ui->dsb_offset_y->blockSignals(false);

    m_mesh_widget->ui->dsb_offset_z->blockSignals(true);
    m_mesh_widget->ui->dsb_offset_z->setValue(tc->wpos().z);
    m_mesh_widget->ui->dsb_offset_z->blockSignals(false);

}

bool resource_dialog_prev::set_entity(nsentity * ent_)
{
    if (ent_ == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Internal error - must set valid ent. You gotta wonder - who writes these messages anyways??");
        mb.exec();
        return false;
    }

    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_ent_widget);
 //   m_mesh_widget->clear_fields();
    _reset_fields();

    // make main window context current and get the passed in meshes owning plugin name
    nse.make_current(bbtk.map_view()->glewID());
    nsstring plug_name = nse.plugin(ent_->plugin_id())->name();

    // indicate that we are editing an already loaded mesh by filling in these fields
    m_starting_res = ent_->name();
    m_starting_subdir = ent_->subdir();
    m_starting_plug = plug_name;

    // Now make the preview context current
    m_ui.m_preview->make_current();

    // Create a plugin in this context with the same name and set it to active
    nsplugin * plg = nse.create_plugin(plug_name);
    nse.set_active(plg);
    if (plg == NULL)
    {
        QMessageBox mb(this);
        mb.setWindowTitle("Preview Error");
        mb.setText("Could not load preview plugin - try again");
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        on_m_cancel_btn_pressed();
        return false;
    }

    // Now lets set up the mesh in the preview context
    nsentity * ent = plg->load<nsentity>(ent_->subdir() + ent_->name() + ent_->extension());

    // If the mesh is still null it means the mesh could not be loaded from file which most
    // likely means the mesh has not been saved yet - the user created one and proceeded to
    // edit before saving for example. This, in general, should be allowed if the user is willing
    // to save the mesh.
    if (ent == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Cannot edit unsaved resource - would you like to save to file?");
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int ret = mb.exec();
        if (ret == QMessageBox::Yes)
        {
            bbtk.map_view()->make_current();
            nsplugin * eplg = nse.plugin(plug_name);
            eplg->save<nsentity>(ent_->name());
            m_ui.m_preview->make_current();
            ent = plg->load<nsentity>(ent->subdir() + ent->name() + ent->extension());
        }
        else
        {
            on_m_cancel_btn_pressed();
            return false;
        }
    }
    ent->set_icon_path(ent_->icon_path());
    m_editing_res = ent;
    _set_ent_widget_fields(ent, plg);
    return true;
}

bool resource_dialog_prev::set_entity(const nsstring & model_fname)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_ent_widget);
 //   m_mesh_widget->clear_fields();
    _reset_fields();

    // make main window context current and get the passed in meshes owning plugin name
    nse.make_current(bbtk.map_view()->glewID());
    nsstring plug_name = nse.active()->name();

    // Now make the preview context current
    m_ui.m_preview->make_current();

    // This is a mesh that has not been loaded in to the engine yet
    m_starting_res = "";
    m_starting_subdir = "";
    m_starting_plug = plug_name;

    // Create a plugin in this context with the same name and set it to active
    nsplugin * plg = nse.create_plugin(plug_name);
    nse.set_active(plg);

    // Now lets set up the mesh in the preview context
    nsentity * ent = plg->load_model("new_ent", model_fname, false);
    if (ent == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Could not find mesh specified with path " + QString(model_fname.c_str()));
        mb.exec();
        on_m_cancel_btn_pressed();
        return false;
    }
    m_editing_res = ent;
    _set_ent_widget_fields(ent, plg);
    return true;
}

void resource_dialog_prev::_set_ent_widget_fields(nsentity * ent, nsplugin * plg)
{
    m_ui.m_preview->make_current();

    // Create the scene for viewing the preview and set it to current
    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_focus);
    nse.system<nscamera_system>()->set_sensitivity(DEFAULT_CAM_SENSITIVITY*0.02,nscamera_system::sens_strafe);
    nse.system<nscamera_system>()->set_zoom(DEFAULT_CAM_ZOOM_FACTOR*0.03f);

    // Create our directional light and camera and add it to the scene
    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,0.8f,0.1f);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_ui.m_preview->width(), m_ui.m_preview->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
    scn->add(dir_light, fvec3(0,0,-20.0f));
    scn->add(camera, fvec3(0.0f,-4.0f,-4.0f), ::orientation(fvec3(-70.0f,0.0f,0.0f),fvec3::XYZ));
    scn->set_camera(camera);

    on_cb_shadows_toggled(false);

    m_ui.sldr_diffuse->setSliderPosition(80);
    m_ui.sldr_ambient->setSliderPosition(10);

    // Set the background color to light blue - this color will be used later to mask out the background
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,0.0f));
    scn->add(ent);

    nsentity * base_tile = plg->create_tile("preview_tile", "diffuseGrass.png", "normalGrass.png", fvec3(),4.0f, 0.1f, fvec3(),true );
    scn->add_gridded(base_tile, ivec3(16,16,1), nstile_grid::world(ivec3(-8,-8,1)));
    base_tile->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, !m_mesh_widget->ui->cb_tile_layer->isChecked());


    m_ui.m_plugin_le->setText(plg->name().c_str());
    m_ui.m_name_le->setText(ent->name().c_str());
    m_ui.m_folder_le->setText(ent->subdir().c_str());
    m_ui.m_icon_path_le->setText(ent->icon_path().c_str());
    if (!m_ui.m_icon_path_le->text().isEmpty())
        m_ui.m_icon_lbl->setPixmap(QPixmap(m_ui.m_icon_path_le->text()));

    _setup_preview_controls_mesh();
    nse.system<nsinput_system>()->push_context("cam_control");
    m_ent_widget->set_editing_ent(ent);
    m_ent_widget->set_comp_widget(hash_id("nsrender_comp"));
}
