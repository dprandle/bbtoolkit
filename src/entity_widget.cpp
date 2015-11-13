#include <nsengine.h>
#include <nsentity.h>
#include <nsplugin.h>
#include <nscamera_system.h>
#include <nsscene.h>
#include <nscam_comp.h>
#include <nstile_grid.h>

#include <toolkit.h>
#include <entity_widget.h>
#include <ui_entity_widget.h>
#include <preview.h>
#include <resource_dialog_prev.h>

// widgets
#include <anim_comp_widget.h>
#include <cam_comp_widget.h>
#include <light_comp_widget.h>
#include <occupy_comp_widget.h>
#include <particle_comp_widget.h>
#include <render_comp_widget.h>
#include <sel_comp_widget.h>
#include <terrain_comp_widget.h>
#include <tform_comp_widget.h>
#include <tile_brush_comp_widget.h>
#include <tile_comp_widget.h>

entity_widget::entity_widget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::entity_widget),
    m_editing_ent(nullptr),
    m_prev(nullptr)
{
    m_ui->setupUi(this);
    m_type_map[hash_id("nsanim_comp")] = new anim_comp_widget;
    m_type_map[hash_id("nscam_comp")] = new cam_comp_widget;
    m_type_map[hash_id("nslight_comp")] = new light_comp_widget;
    m_type_map[hash_id("nsoccupy_comp")] = new occupy_comp_widget;
    m_type_map[hash_id("nsparticle_comp")] = new particle_comp_widget;
    m_type_map[hash_id("nsrender_comp")] = new render_comp_widget;
    m_type_map[hash_id("nssel_comp")] = new sel_comp_widget;
    m_type_map[hash_id("nsterrain_comp")] = new terrain_comp_widget;
    m_type_map[hash_id("nstform_comp")] = new tform_comp_widget;
    m_type_map[hash_id("nstile_brush_comp")] = new tile_brush_comp_widget;
    m_type_map[hash_id("nstile_comp")] = new tile_comp_widget;
}

component_widget * entity_widget::comp_widget(uint32 type_id)
{
    auto fiter = m_type_map.find(type_id);
    if (fiter == m_type_map.end())
        return nullptr;
    return fiter->second;
}

void entity_widget::set_comp_widget(uint32 comp_type)
{
    component_widget * cw = comp_widget(comp_type);
    if (cw == nullptr)
        return;
    m_ui->sa_data->takeWidget();
    m_ui->sa_data->setWidget(cw);
    _ui_comp_title(comp_type);
}

entity_widget::~entity_widget()
{
    delete m_ui;
}

void entity_widget::done()
{
    m_editing_ent = NULL;
    auto cw_iter = m_type_map.begin();
    while (cw_iter != m_type_map.end())
    {
        cw_iter->second->set_entity(m_editing_ent);
        ++cw_iter;
    }
}

void entity_widget::_ui_comp_title(uint32 cid)
{
    nsstring guid_ = hash_to_guid(cid).substr(2);
    guid_[0] = toupper(guid_[0]);
    for (uint32 i = 0; i < guid_.size()-1; ++i)
    {
        if (guid_[i] == '_')
        {
            guid_[i] = ' ';
            guid_[i+1] = toupper(guid_[i+1]);
        }
    }
    m_ui->lbl_component->setText(guid_.c_str());
}

void entity_widget::set_entity(nsentity * ent_)
{
    m_editing_ent = ent_;
    m_prev->make_current();
    nsplugin * plg = nse.active();

    // Create the scene for viewing the preview and set it to current
    nsscene * scn = plg->create<nsscene>("preview_map");
    nse.set_current_scene(scn, true, false);

    nse.system<nscamera_system>()->set_mode(nscamera_system::mode_focus);
    nse.system<nscamera_system>()->set_sensitivity(DEFAULT_CAM_SENSITIVITY*0.02,nscamera_system::sens_strafe);
    nse.system<nscamera_system>()->set_zoom(DEFAULT_CAM_ZOOM_FACTOR*0.03f);

    // Create our directional light and camera and add it to the scene
    nsentity * dir_light = plg->create_dir_light(PREV_LIGHT_NAME,DEF_LT_DIFF,DEF_LT_AMB);
    nsentity * camera = plg->create_camera(PREV_CAM_NAME,60.0f, uivec2(m_prev->width(), m_prev->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));
    scn->add(dir_light, fvec3(0,0,-20.0f));
    scn->add(camera, fvec3(0.0f,-4.0f,-4.0f), ::orientation(fvec3(-70.0f,0.0f,0.0f),fvec3::XYZ));
    scn->set_camera(camera);

    // Set the background color to light blue - this color will be used later to mask out the background
    scn->set_bg_color(fvec4(BG_R,BG_G,BG_B,0.0f));
    scn->add(m_editing_ent);

    nsentity * base_tile = plg->create_tile("preview_tile", "diffuseGrass.png", "normalGrass.png", fvec3(),4.0f, 0.1f, fvec3(),true );
    scn->add_gridded(base_tile, ivec3(16,16,1), nstile_grid::world(ivec3(-8,-8,1)));
    base_tile->get<nstform_comp>()->set_hidden_state(nstform_comp::hide_all, false);

    auto cw_iter = m_type_map.begin();
    while (cw_iter != m_type_map.end())
    {
        cw_iter->second->set_entity(m_editing_ent);
        ++cw_iter;
    }
}

void entity_widget::init(Preview * prev_)
{
    m_prev = prev_;
    auto cw_iter = m_type_map.begin();
    while (cw_iter != m_type_map.end())
    {
        cw_iter->second->init(m_prev);
        ++cw_iter;
    }
}

void entity_widget::on_tb_prev_pressed()
{
    component_widget * cw = static_cast<component_widget*>(m_ui->sa_data->widget());
    auto fiter = m_type_map.find(cw->type());

    do {
        --fiter;
        if (fiter == m_type_map.begin())
            fiter = --m_type_map.end();
    } while (!m_editing_ent->has(fiter->first));

    m_ui->sa_data->takeWidget();
    m_ui->sa_data->setWidget(fiter->second);
    _ui_comp_title(fiter->second->type());
}

void entity_widget::on_tb_next_pressed()
{
    component_widget * cw = static_cast<component_widget*>(m_ui->sa_data->widget());
    auto fiter = m_type_map.find(cw->type());

    do {
        ++fiter;
        if (fiter == m_type_map.end())
            fiter = m_type_map.begin();
    } while (!m_editing_ent->has(fiter->first));

    m_ui->sa_data->takeWidget();
    m_ui->sa_data->setWidget(fiter->second);
    _ui_comp_title(fiter->second->type());
}

void entity_widget::on_tb_back_pressed()
{
    emit set_comp_sel();
}

nsentity * entity_widget::editing_ent()
{
    return m_editing_ent;
}
