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
#include <nstex_manager.h>

#include <resource_dialog_prev.h>
#include <texture_widget.h>
#include <ui_texture_widget.h>
#include <QMessageBox>
#include <toolkit.h>
#include <QFileDialog>

resource_dialog_prev::resource_dialog_prev(QWidget * parent):
    QDialog(parent),
    m_editing_res(NULL),
    m_tex_widget(new texture_widget(parent))
{
    m_ui.setupUi(this);
    connect(m_tex_widget, SIGNAL(cubemap_triggered()), this, SLOT(tex_cubemap_triggered()));
    connect(m_tex_widget, SIGNAL(tex2d_triggered()), this, SLOT(tex_tex2d_triggered()));
}

void resource_dialog_prev::init()
{
    m_ui.m_preview->init();
    m_tex_widget->init();
}

void resource_dialog_prev::set_mesh(nsmesh * mesh_)
{
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

void resource_dialog_prev::set_texture(nstexture * tex_)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_tex_widget);
    m_tex_widget->clear_fields();
    _reset_fields();

    nse.make_current(bbtk.map_view()->glewID());

    nsstring plug_name = "resource_dialog_prev";
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
    scn->set_bg_color(fvec3(0.7,0.7,0.9));

    // Setup mesh plane
    nsmesh_plane * plane = plg->create<nsmesh_plane>("2dplane");
    plane->set_dim(fvec2(12,12));

    // Setup material for the plane
    nsmaterial * tex_mat = plg->create<nsmaterial>("2dplane");
    tex_mat->set_color(fvec4(0.2,1.0,0.1,1.0));
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
    _setup_preview_controls();

    if (tex_ != NULL)
    {
        m_ui.m_name_le->setText(tex_->name().c_str());
        m_ui.m_folder_le->setText(tex_->subdir().c_str());
        m_ui.m_plugin_le->setText(plug_name.c_str());
        m_ui.m_icon_path_le->setText(tex_->icon_path().c_str());

        m_starting_res = tex_->name();
        m_starting_plug = plug_name;
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
        else if (tex_->texture_type() == nstexture::tex_1d)
        {
            m_tex_widget->ui->m_random_cb->setChecked(true);
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
        tex_tex2d_triggered();

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

void resource_dialog_prev::_setup_preview_controls()
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

void resource_dialog_prev::on_m_okay_btn_pressed()
{
    m_ui.m_preview->make_current();
    nsplugin * plg = nse.active();
    if (plg == nullptr)
        return;

    if (m_editing_res != NULL)
    {
        nsstring new_name = m_ui.m_name_le->text().toStdString();
        m_editing_res->set_subdir(m_ui.m_folder_le->text().toStdString());
        m_editing_res->rename(new_name);
        nsstring fname = m_editing_res->subdir() + m_editing_res->name() + m_editing_res->extension();
        uint32 res_type_id = m_editing_res->type();

        // Make main window current and remove the old resource from fil
        bbtk.map_view()->make_current();
        nsplugin * eng_plug = nse.plugin(m_starting_plug);
        nsresource * res = eng_plug->get(res_type_id, m_starting_res);
        eng_plug->del(res);

        // Propagate name change
        nse.name_change(uivec2(hash_id(m_starting_plug),hash_id(m_starting_res)), m_editing_res->full_id());

        // Make this current and save to file
        m_ui.m_preview->make_current();
        plg->save(m_editing_res);

        // Make engine current again and load from fname
        bbtk.map_view()->make_current();
        eng_plug->load(res_type_id, fname);

        // Finally make this current again
        m_ui.m_preview->make_current();
    }

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

}

void resource_dialog_prev::on_m_folder_tb_pressed()
{
    QString fname = QFileDialog::getExistingDirectory(this,
                                                 "Resource Subdirectory",
                                                 "./resources/" + m_ui.m_plugin_le->text(),
                                                 QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly);
    nsstring to_set = nsres_manager::name_from_filename(fname.toStdString());
    m_ui.m_folder_le->setText(to_set.c_str());
    m_ui.m_folder_btn->setDown(false);
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
    m_ui.m_icon_path_tb->setDown(false);
}
