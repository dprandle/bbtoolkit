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
#include <nsfile_os.h>

#include <QPixmap>
#include <QBitmap>
#include <resource_dialog_prev.h>
#include <texture_widget.h>
#include <mesh_widget.h>
#include <ui_mesh_widget.h>
#include <ui_texture_widget.h>
#include <QMessageBox>
#include <edit_submesh_data_widget.h>
#include <ui_edit_submesh_data_widget.h>
#include <toolkit.h>
#include <select_res_dialog.h>
#include <QFileDialog>

resource_dialog_prev::resource_dialog_prev(QWidget * parent):
    QDialog(parent),
    m_editing_res(NULL),
    m_tex_widget(new texture_widget()),
    m_mesh_widget(new mesh_widget())
{
    m_ui.setupUi(this);
    connect(m_tex_widget, SIGNAL(cubemap_triggered()), this, SLOT(tex_cubemap_triggered()));
    connect(m_tex_widget, SIGNAL(tex2d_triggered()), this, SLOT(tex_tex2d_triggered()));
    connect(m_mesh_widget->ui->cb_wireframe, SIGNAL(toggled(bool)), this, SLOT(mesh_wireframe_toggled(bool)));
    connect(m_mesh_widget->ui->btn_verts, SIGNAL(pressed()), this, SLOT(mesh_show_verts()));
}

void resource_dialog_prev::init()
{
    m_ui.m_preview->init();
    m_tex_widget->init();
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

void resource_dialog_prev::set_mesh(const nsstring & model_fname)
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
        rejected();
        return;
    }
    m_editing_res = msh;
    _set_mesh_widget_fields(msh, plg);
}

void resource_dialog_prev::mesh_wireframe_toggled(bool new_val)
{
    m_ui.m_preview->make_current();
    nse.active()->get<nsmaterial>("preview_mat")->enable_wireframe(new_val);
}

void resource_dialog_prev::set_mesh(nsmesh * mesh_)
{
    if (mesh_ == NULL)
    {
        QMessageBox mb(this);
        mb.setText("Internal error - must set valid mesh. You gotta wonder - who writes these messages anyways??");
        mb.exec();
        rejected();
        return;
    }

    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_mesh_widget);
 //   m_mesh_widget->clear_fields();
    _reset_fields();

    // make main window context current and get the passed in meshes owning plugin name
    nse.make_current(bbtk.map_view()->glewID());
    nsstring plug_name = nse.plugin(mesh_->plugin_id())->name();

    // Now make the preview context current
    m_ui.m_preview->make_current();

    // indicate that we are editing an already loaded mesh by filling in these fields
    m_starting_res = mesh_->name();
    m_starting_subdir = mesh_->subdir();
    m_starting_plug = plug_name;

    // Create a plugin in this context with the same name and set it to active
    nsplugin * plg = nse.create_plugin(plug_name);
    nse.set_active(plg);

    // Now lets set up the mesh in the preview context
    nsmesh * msh = plg->load<nsmesh>(mesh_->subdir() + mesh_->name() + mesh_->extension());
    msh->set_icon_path(mesh_->icon_path());

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
            rejected();
            return;
        }
    }

    m_editing_res = msh;
    _set_mesh_widget_fields(msh, plg);
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
}


/* MAKE SURE PREVIEW CONTEXT IS SET PREVIEW CALLING THIS FUNCTION!*/
void resource_dialog_prev::_set_mesh_widget_fields(nsmesh * msh, nsplugin * plg)
{
    // Create the scene for viewing the preview and set it to current
    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    // For the mesh we want focus mode
    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_focus);
    nse.system<nscamera_system>()->set_sensitivity(DEFAULT_CAM_SENSITIVITY*0.1,nscamera_system::sens_strafe);
    nse.system<nscamera_system>()->set_zoom(DEFAULT_CAM_ZOOM_FACTOR*0.1f);

    // Create our directional light and camera and add it to the scene
    nsentity * dir_light = plg->create_dir_light("preview_dir_light",0.1f,0.7f);
    nsentity * camera = plg->create_camera("preview_cam",60.0f, uivec2(m_ui.m_preview->width(), m_ui.m_preview->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
    scn->add(dir_light,fvec3(-30,20,20));
    scn->set_camera(camera);

    // Set the background color to light blue - this color will be used later to mask out the background
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,1.0f));

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

    // Setup the center point of the mesh

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
    _update_submesh_info(msh, 0);
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

void resource_dialog_prev::set_texture(nstexture * tex_)
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

    nsentity * dir_light = plg->create_dir_light("preview_dir_light",0.0f,1.0f);
    nsentity * camera = plg->create_camera("preview_cam",60.0f, uivec2(m_ui.m_preview->width(), m_ui.m_preview->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));

    scn->add(dir_light,fvec3(-30,20,20));
    scn->set_camera(camera);
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,1.0f));

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
        }

    }
    else
    {
        m_tex_widget->ui->m_type_lbl->setEnabled(true);
        m_tex_widget->ui->m_tex_type_cmb->setEnabled(true);
        tex_tex2d_triggered();
    }

    // main camera controls
}

void resource_dialog_prev::tex_cubemap_triggered()
{
    m_ui.m_preview->make_current();

    nsplugin * plg = nse.active();
    nsscene * scn = plg->current_scene();
    nsentity * dir_light = plg->get<nsentity>("preview_dir_light");
    nsentity * camera = plg->get<nsentity>("preview_cam");
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
            {
                QMessageBox mb(this);
                mb.setText("All images in the cubemap must be the same size");
                mb.exec();
                return;
            }
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
        mat->set_map_tex_id(nsmaterial::diffuse, tex->full_id());
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
    nsentity * dir_light = plg->get<nsentity>("preview_dir_light");
    nsentity * camera = plg->get<nsentity>("preview_cam");
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
        mb.setText("Could not find image file specified!");
        mb.exec();
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

    // Now destroy all resources in the plugin and reset the editing resource
    nse.set_active(nullptr);
    nse.destroy_plugin(plg);
    m_editing_res = nullptr;
    accept();
}

void resource_dialog_prev::on_m_cancel_btn_pressed()
{
    m_ui.m_preview->make_current();
    nsplugin * plg = nse.active();
    if (plg == nullptr)
        return;
    nse.set_active(nullptr);
    nse.destroy_plugin(plg);
    m_editing_res = nullptr;
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
    nsstring to_set = nsres_manager::name_from_filename(fname.toStdString()) + '/';
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

    QPixmap pixMap = QPixmap::grabWidget(m_ui.m_preview).scaled(70,70);// = QPixmap::grabWidget(m_ui.m_preview).scaled(70,70);
    pixMap.setMask(pixMap.createMaskFromColor(QColor(int(BG_R*255),int(BG_G*255),int(BG_B*255))));

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
