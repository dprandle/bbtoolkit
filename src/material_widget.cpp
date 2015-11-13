
#include <nsmaterial.h>
#include <nsshader.h>
#include <nstexture.h>
#include <nsscene.h>
#include <nscamera_system.h>
#include <nscam_comp.h>
#include <nsplugin.h>
#include <nsrender_system.h>
#include <nsrender_comp.h>

#include <toolkit.h>
#include <resource_dialog_prev.h>
#include <preview.h>
#include <material_widget.h>
#include <ui_material_widget.h>
#include <select_res_dialog.h>

#include <QLineEdit>
#include <QColorDialog>

material_widget::material_widget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::material_widget),
    m_edit_mat(nullptr),
    m_prev(nullptr)
{
    m_ui->setupUi(this);
}

material_widget::~material_widget()
{
    delete m_ui;
}

void material_widget::init(Preview * prev_)
{
    m_prev = prev_;
}

void material_widget::done()
{
    m_edit_mat = NULL;
}

void material_widget::set_material(nsmaterial * mat_)
{
    m_edit_mat = mat_;

    m_prev->make_current();
    nsplugin * plg = nse.active();

    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_free);

    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,DEF_LT_DIFF,DEF_LT_AMB);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_prev->width(), m_prev->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));

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
    rc->set_material(0, m_edit_mat->full_id());

    // Setup the sphere preview entity
    nsentity * sphere_ent = plg->create<nsentity>("preview_sphere");
    rc = sphere_ent->create<nsrender_comp>();
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_SKYDOME)->full_id());
    rc->set_material(0, m_edit_mat->full_id());

    // Setup the tile preview entity
    nsentity * tile_ent = plg->create<nsentity>("preview_tile");
    rc = tile_ent->create<nsrender_comp>();
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_FULL_TILE)->full_id());
    rc->set_material(0, m_edit_mat->full_id());

    // Setup the half tile preview entity
    nsentity * tile_half_ent = plg->create<nsentity>("preview_half_tile");
    rc = tile_half_ent->create<nsrender_comp>();
    rc->set_mesh_id(nse.core()->get<nsmesh>(MESH_HALF_TILE)->full_id());
    rc->set_material(0, m_edit_mat->full_id());

    m_ui->cb_color_mode->setChecked(m_edit_mat->color_mode());
    m_ui->cb_cull_enable->setChecked(m_edit_mat->culling());
    m_ui->sldr_mat_shininess->setValue(m_edit_mat->specular_intensity()*100.0f);
    m_ui->sb_spec_power->setValue(int(m_edit_mat->specular_power()));

    QColor color(m_edit_mat->color().r*255,m_edit_mat->color().g*255,m_edit_mat->color().b*255,m_edit_mat->color().a*255);
    QPixmap px(20, 20);
    px.fill(color);
    m_ui->tb_material_color->setIcon(px);

    QColor scolor(m_edit_mat->specular_color().r*255,m_edit_mat->specular_color().g*255,m_edit_mat->specular_color().b*255,255);
    QPixmap spx(20, 20);
    spx.fill(scolor);
    m_ui->tb_spec_color->setIcon(spx);

    if (m_edit_mat->cull_mode() == GL_BACK)
        m_ui->cmb_cull_mode->setCurrentIndex(0);
    else if (m_edit_mat->cull_mode() == GL_FRONT)
        m_ui->cmb_cull_mode->setCurrentIndex(1);
    else
        m_ui->cmb_cull_mode->setCurrentIndex(2);

    on_cmb_preview_mesh_currentIndexChanged(m_ui->cmb_preview_mesh->currentIndex());

    // Check the textures
    nstexture * diff_tex = nse.resource<nstexture>(m_edit_mat->map_tex_id(nsmaterial::diffuse));
    if (diff_tex != NULL)
    {
        m_ui->le_diffuse_name->setText(diff_tex->name().c_str());
        m_ui->le_diffuse_plugin->setText(nse.plugin(diff_tex->plugin_id())->name().c_str());
    }

    nstexture * norm_tex = nse.resource<nstexture>(m_edit_mat->map_tex_id(nsmaterial::normal));
    if (norm_tex != NULL)
    {
        m_ui->le_normal_name->setText(norm_tex->name().c_str());
        m_ui->le_normal_plugin->setText(nse.plugin(norm_tex->plugin_id())->name().c_str());
    }

    nstexture * opac_tex = nse.resource<nstexture>(m_edit_mat->map_tex_id(nsmaterial::opacity));
    if (opac_tex != NULL)
    {
        m_ui->le_opacity_name->setText(opac_tex->name().c_str());
        m_ui->le_opacity_plugin->setText(nse.plugin(opac_tex->plugin_id())->name().c_str());
    }

    nstexture * height_tex = nse.resource<nstexture>(m_edit_mat->map_tex_id(nsmaterial::height));
    if (height_tex != NULL)
    {
        m_ui->le_height_name->setText(height_tex->name().c_str());
        m_ui->le_height_plugin->setText(nse.plugin(height_tex->plugin_id())->name().c_str());
    }

    nsshader * shdr = nse.resource<nsshader>(m_edit_mat->shader_id());
    if (shdr != NULL)
    {
        m_ui->le_shader_name->setText(shdr->name().c_str());
        m_ui->le_shader_plugin->setText(nse.plugin(shdr->plugin_id())->name().c_str());

        if (!shdr->compiled(nsshader::vertex_shader))
            shdr->compile(nsshader::vertex_shader);
        if (!shdr->compiled(nsshader::geometry_shader))
            shdr->compile(nsshader::geometry_shader);
        if (!shdr->compiled(nsshader::fragment_shader))
            shdr->compile(nsshader::fragment_shader);
        if (!shdr->linked())
        {
            shdr->link();
            shdr->init_uniforms();
        }
    }

}

nsmaterial * material_widget::material()
{
    return m_edit_mat;
}

void material_widget::on_btn_diffuse_pressed()
{
    _btn_texture(nsmaterial::diffuse, m_ui->le_diffuse_plugin, m_ui->le_diffuse_name);
}

void material_widget::on_btn_normal_pressed()
{
    _btn_texture(nsmaterial::normal, m_ui->le_normal_plugin, m_ui->le_normal_name);
}

void material_widget::on_btn_opacity_pressed()
{
    _btn_texture(nsmaterial::opacity, m_ui->le_opacity_plugin, m_ui->le_opacity_name);
}

void material_widget::on_btn_height_pressed()
{
    _btn_texture(nsmaterial::height, m_ui->le_height_plugin, m_ui->le_height_name);
}

void material_widget::_btn_texture(uint32 map_type, QLineEdit * plg_, QLineEdit * name_)
{
    nstexture * loaded_tex = NULL;
    nsstring plug,name;

    select_res_dialog rd(this);
    rd.show_type<nstexture>();
    if (rd.exec() == QDialog::Accepted)
    {
        bbtk.map()->make_current();
        loaded_tex = rd.selected_resource<nstexture>();
        plug = nse.plugin(loaded_tex->plugin_id())->name();
        name = loaded_tex->name();
        plg_->setText(plug.c_str());
        name_->setText(name.c_str());
    }
    else
        return;

    m_prev->make_current();
    if (loaded_tex != NULL)
    {
        nsplugin * plg = nse.plugin(plug);
        if (plg == NULL)
            plg = nse.create_plugin(plug, false);

        nsresource * tex = plg->get<nstexture>(name);
        if (tex == NULL)
            tex = plg->load(loaded_tex->type(), loaded_tex->subdir() + loaded_tex->name() + loaded_tex->extension());
        m_edit_mat->set_map_tex_id(nsmaterial::map_type(map_type), tex->full_id(), true);
    }
}

void material_widget::on_btn_shader_pressed()
{
    nsmaterial_shader * mat_shader = NULL;
    nsstring plug,name;

    select_res_dialog rd(this);
    rd.show_type<nsmaterial_shader>();
    if (rd.exec() == QDialog::Accepted)
    {
        bbtk.map()->make_current();
        mat_shader = rd.selected_resource<nsmaterial_shader>();
        plug = nse.plugin(mat_shader->plugin_id())->name();
        name = mat_shader->name();
        m_ui->le_shader_plugin->setText(plug.c_str());
        m_ui->le_shader_name->setText(name.c_str());
    }
    else
        return;

    m_prev->make_current();
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

        m_edit_mat->set_shader_id(m_shd->full_id());
    }
}

void material_widget::on_tb_material_color_pressed()
{
    QColor c(m_edit_mat->color().r*255,m_edit_mat->color().g*255,m_edit_mat->color().b*255,m_edit_mat->color().a*255);
    QColorDialog cd(c,this);
    cd.setWindowTitle("Choose Material Color");
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    if (cd.exec() == QDialog::Accepted)
        c = cd.currentColor();

    QPixmap px(20, 20);
    px.fill(c);
    m_ui->tb_material_color->setIcon(px);
    m_ui->tb_material_color->setDown(false);
    m_edit_mat->set_color(fvec4(c.redF(),c.greenF(),c.blueF(),c.alphaF()));
}

void material_widget::on_tb_spec_color_pressed()
{
    QColor c(m_edit_mat->specular_color().r*255,m_edit_mat->specular_color().g*255,m_edit_mat->specular_color().b*255,255);
    QColorDialog cd(c,this);
    cd.setWindowTitle("Choose Specular Color");
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    if (cd.exec() == QDialog::Accepted)
        c = cd.currentColor();

    QPixmap px(20, 20);
    px.fill(c);
    m_ui->tb_spec_color->setIcon(px);
    m_ui->tb_spec_color->setDown(false);
    m_edit_mat->set_specular_color(fvec3(c.redF(),c.greenF(),c.blueF()));
}


void material_widget::on_cb_color_mode_toggled(bool val_)
{
    m_edit_mat->set_color_mode(val_);
}

void material_widget::on_cb_cull_enable_toggled(bool val_)
{
    m_edit_mat->enable_culling(val_);
}

void material_widget::on_cb_wireframe_toggled(bool val_)
{
    m_edit_mat->enable_wireframe(val_);
}

void material_widget::on_cmb_cull_mode_currentIndexChanged(int val_)
{
    switch (val_)
    {
    case(0):
        m_edit_mat->set_cull_mode(GL_BACK);
        break;
    case(1):
        m_edit_mat->set_cull_mode(GL_FRONT);
        break;
    default:
        m_edit_mat->set_cull_mode(GL_FRONT_AND_BACK);
        break;
    }
}

void material_widget::on_sldr_mat_shininess_valueChanged(int val_)
{
    m_edit_mat->set_specular_intensity(float(val_) / 100.0f);
}

void material_widget::on_sb_spec_power_valueChanged(int val_)
{
    m_edit_mat->set_specular_power(float(val_));
}

void material_widget::on_tb_norm_erase_pressed()
{
    m_edit_mat->set_map_tex_id(nsmaterial::normal,uivec2(),true);
    m_ui->le_normal_name->clear();
    m_ui->le_normal_plugin->clear();
    m_ui->tb_norm_erase->setDown(false);
}

void material_widget::on_tb_dif_erase_pressed()
{
    m_edit_mat->set_map_tex_id(nsmaterial::diffuse,uivec2(),true);
    m_ui->le_diffuse_name->clear();
    m_ui->le_diffuse_plugin->clear();
    m_ui->tb_dif_erase->setDown(false);
}

void material_widget::on_tb_opac_erase_pressed()
{
    m_edit_mat->set_map_tex_id(nsmaterial::opacity,uivec2(),true);
    m_ui->le_opacity_name->clear();
    m_ui->le_opacity_plugin->clear();
    m_ui->tb_opac_erase->setDown(false);
}

void material_widget::on_tb_height_erase_pressed()
{
    m_edit_mat->set_map_tex_id(nsmaterial::height,uivec2(),true);
    m_ui->le_height_name->clear();
    m_ui->le_height_plugin->clear();
    m_ui->tb_height_erase->setDown(false);
}

void material_widget::on_tb_shader_erase_pressed()
{
    m_edit_mat->set_shader_id(uivec2());
    m_ui->le_shader_name->clear();
    m_ui->le_shader_plugin->clear();
    m_ui->tb_shader_erase->setDown(false);
}

void material_widget::on_cmb_preview_mesh_currentIndexChanged(int new_index)
{
    m_prev->make_current();
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
