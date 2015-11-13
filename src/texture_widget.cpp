#include <nsengine.h>
#include <nstexture.h>
#include <nsscene.h>
#include <nsentity.h>
#include <nscamera_system.h>
#include <nscam_comp.h>
#include <nsmesh.h>
#include <nstex_manager.h>
#include <nsshader.h>
#include <nsrender_system.h>
#include <nsinput_system.h>
#include <nsplugin.h>
#include <nsmaterial.h>
#include <nsrender_comp.h>

#include <resource_dialog_prev.h>
#include <preview.h>
#include <texture_widget.h>
#include <ui_texture_widget.h>
#include <toolkit.h>
#include <QFileDialog>
#include <QMessageBox>

texture_widget::texture_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::texture_widget),
    m_edit_tex(NULL),
    m_prev(NULL)
{
    ui->setupUi(this);
}

texture_widget::~texture_widget()
{
    delete ui;
}

void texture_widget::init(Preview * prev_)
{
    m_prev = prev_;
}

void texture_widget::done()
{
    m_edit_tex = NULL;
}

void texture_widget::set_texture(nstexture * tex_)
{
    m_edit_tex = tex_;
    m_prev->make_current();
    nsplugin * plg = nse.active();

    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_free);

    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,DEF_LT_DIFF,DEF_LT_AMB);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_prev->width(), m_prev->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));

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

    if (tex_ != NULL)
    {
        ui->m_type_lbl->setEnabled(false);
        ui->m_tex_type_cmb->setEnabled(false);

        if (tex_->texture_type() == nstexture::tex_cubemap)
        {
            nstex_manager * tm = plg->manager<nstex_manager>();
            QString fname = QString( (tm->res_dir() + tm->local_dir() + tex_->subdir() + tex_->name()).c_str());
            ui->m_cm_front_le->setText(fname + "_front.png");
            ui->m_cm_back_le->setText(fname + "_back.png");
            ui->m_cm_top_le->setText(fname + "_top.png");
            ui->m_cm_bottom_le->setText(fname + "_bottom.png");
            ui->m_cm_left_le->setText(fname + "_left.png");
            ui->m_cm_right_le->setText(fname + "_right.png");

            if (ui->m_tex_type_cmb->currentIndex() != 1)
                ui->m_tex_type_cmb->setCurrentIndex(1);
            else
                _load_cubemap();
        }
        else
        {
            nstex_manager * tm = plg->manager<nstex_manager>();
            nsstring fname = tm->res_dir() + tm->local_dir() + tex_->subdir() + tex_->name() + tex_->extension();
            ui->m_image_file_le->setText(fname.c_str());

            if (ui->m_tex_type_cmb->currentIndex() != 0)
                ui->m_tex_type_cmb->setCurrentIndex(0);
            else
                _load_tex2d();
        }

    }
    else
    {
        ui->m_type_lbl->setEnabled(true);
        ui->m_tex_type_cmb->setEnabled(true);
        _load_tex2d();
    }
}

void texture_widget::clear_fields()
{
    ui->m_cm_front_le->clear();
    ui->m_cm_back_le->clear();
    ui->m_cm_left_le->clear();
    ui->m_cm_right_le->clear();
    ui->m_cm_top_le->clear();
    ui->m_cm_bottom_le->clear();
    ui->m_image_file_le->clear();
}

void texture_widget::on_m_tex_type_cmb_currentIndexChanged(int new_index)
{
    bool set_cm = new_index == 1;

    ui->m_cm_gb->setEnabled(set_cm);
    ui->m_image_file_le->setEnabled(!set_cm);
    ui->m_image_file_lbl->setEnabled(!set_cm);
    ui->m_image_file_tb->setEnabled(!set_cm);


    if (new_index == 0)
        _load_tex2d();
    else
        _load_cubemap();
}

void texture_widget::on_m_image_file_tb_pressed()
{

    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_image_file_le->setText(fname);
    ui->m_image_file_tb->setDown(false);
    _load_tex2d();
}

void texture_widget::on_m_cm_front_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_front_le->setText(fname);
    ui->m_cm_front_tb->setDown(false);
    _load_cubemap();
}

void texture_widget::on_m_cm_back_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_back_le->setText(fname);
    ui->m_cm_back_tb->setDown(false);
    _load_cubemap();
}

void texture_widget::on_m_cm_right_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_right_le->setText(fname);
    ui->m_cm_right_tb->setDown(false);
    _load_cubemap();
}

void texture_widget::on_m_cm_left_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_left_le->setText(fname);
    ui->m_cm_left_tb->setDown(false);
    _load_cubemap();
}

void texture_widget::on_m_cm_top_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_top_le->setText(fname);
    ui->m_cm_top_tb->setDown(false);
    _load_cubemap();
}

void texture_widget::on_m_cm_bottom_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    ui->m_cm_bottom_le->setText(fname);
    ui->m_cm_bottom_tb->setDown(false);
    _load_cubemap();
}

void texture_widget::_load_cubemap()
{
    m_prev->make_current();

    nsplugin * plg = nse.active();
    nsscene * scn = plg->current_scene();
    nsentity * dir_light = plg->get<nsentity>(PREV_LIGHT_NAME);
    nsentity * camera = plg->get<nsentity>(PREV_CAM_NAME);
    nsentity * skydome_ent = plg->get<nsentity>("skydome");

    camera->get<nstform_comp>()->set_pos(fvec3());
    camera->get<nstform_comp>()->set_orientation(::orientation(fvec4(1,0,0,-90)));
    camera->get<nscam_comp>()->post_update(true);

    scn->clear();
    scn->set_camera(camera);
    scn->add(dir_light, fvec3(-30,20,20));
    scn->set_skydome(skydome_ent);

    nse.system<nsinput_system>()->clear_contexts();
    nse.system<nsinput_system>()->push_context("cam_control");

    nsstring_vector svec;
    svec.resize(6);
    svec[0] = ui->m_cm_front_le->text().toStdString();
    svec[1] = ui->m_cm_back_le->text().toStdString();
    svec[2] = ui->m_cm_top_le->text().toStdString();
    svec[3] = ui->m_cm_bottom_le->text().toStdString();
    svec[4] = ui->m_cm_left_le->text().toStdString();
    svec[5] = ui->m_cm_right_le->text().toStdString();

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
        mb.setIcon(QMessageBox::Warning);
        mb.setWindowTitle("Cubemap Size");
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

    if (m_edit_tex != NULL)
        plg->destroy<nstexture>(m_edit_tex->name());

    m_edit_tex = mgr->load_cubemap(svec[0],svec[1],svec[2],svec[3],svec[4],svec[5],"preview_tex.cube");
    if (m_edit_tex != NULL)
    {
        nsmaterial * mat = plg->get<nsmaterial>("skydome");
        mat->set_map_tex_id(nsmaterial::diffuse, m_edit_tex->full_id(), true);
        emit edit_res_change(m_edit_tex);
    }
    else
    {
        QMessageBox mb(this);
        mb.setText("Could not find one of the images specified please choose different image");
        mb.setIcon(QMessageBox::Warning);
        mb.setWindowTitle("Image Error");
        mb.exec();
    }
}

void texture_widget::_load_tex2d()
{
    m_prev->make_current();

    nsplugin * plg = nse.active();
    nsscene * scn = plg->current_scene();
    nsentity * dir_light = plg->get<nsentity>(PREV_LIGHT_NAME);
    nsentity * camera = plg->get<nsentity>(PREV_CAM_NAME);
    nsentity * plane_ent = plg->get<nsentity>("2dplane");


    camera->get<nstform_comp>()->set_pos(fvec3(0,0,-10));
    camera->get<nstform_comp>()->set_orientation(fquat());
    camera->get<nscam_comp>()->post_update(true);

    nse.system<nsinput_system>()->clear_contexts();

    scn->clear();
    scn->set_camera(camera);
    scn->add(dir_light, fvec3(-30,20,20));
    scn->add(plane_ent,fvec3(0,0,4));

    nsstring fname = ui->m_image_file_le->text().toStdString();
    if (fname.empty())
        return;

    if (m_edit_tex != NULL)
        plg->destroy<nstexture>(m_edit_tex->name());
    m_edit_tex = plg->load<nstex2d>(fname);

    if (m_edit_tex != NULL)
    {
        nsmaterial * mat = plg->get<nsmaterial>("2dplane");
        mat->set_map_tex_id(nsmaterial::diffuse, m_edit_tex->full_id(),true);
        emit edit_res_change(m_edit_tex);
    }
    else
    {
        QMessageBox mb(this);
        mb.setText("Could not find image specified: \'" + QString(fname.c_str()) + "\'  please choose different image");
        mb.setIcon(QMessageBox::Warning);
        mb.setWindowTitle("Image Error");
        mb.exec();
    }
//    else
//    {
//        QMessageBox mb(this);
//        mb.setText("The image you selected has not yet been saved in plugin - it has to be saved in order to edit - would you like to save and edit?");
//        mb.setIcon(QMessageBox::Warning);
//        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
//        mb.setWindowTitle("Editing Texture");
//        int ret = mb.exec();
//        if (ret == QMessageBox::Yes)
//        {
//            bbtk.map_view()->make_current();
//            nsplugin * eplg = nse.plugin(m_starting_plug);
//            eplg->save<nstex2d>(nsres_manager::name_from_filename(fname));
//            m_prev->make_current();
//            tex = plg->load<nstex2d>(fname);
//            if (tex == NULL)
//            {
//                mb.setText("Could not locate texture ");
//                mb.setStandardButtons(QMessageBox::Ok);
//                mb.setIcon(QMessageBox::Warning);
//                mb.exec();
//                on_m_cancel_btn_pressed();
//            }
//            tex->rename("preview_tex");
//            nsmaterial * mat = plg->get<nsmaterial>("2dplane");
//            mat->set_map_tex_id(nsmaterial::diffuse, tex->full_id(),true);
//            m_editing_res = tex;
 //       }
//    }
}
