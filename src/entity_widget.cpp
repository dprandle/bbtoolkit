#include <nsengine.h>
#include <nsentity.h>
#include <nsplugin.h>

#include <toolkit.h>
#include <entity_widget.h>
#include <ui_entity_widget.h>
#include <preview.h>

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
    ui(new Ui::entity_widget),
    m_editing_ent(NULL),
    m_prev(NULL)
{
    ui->setupUi(this);
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
    if (cw == NULL)
        return;
    ui->sa_data->takeWidget();
    ui->sa_data->setWidget(cw);
}

entity_widget::~entity_widget()
{
    delete ui;
}

void entity_widget::set_editing_ent(nsentity * ent_)
{
    m_editing_ent = ent_;

    // Make sure all of the resources are loaded for each component
    bbtk.map_view()->make_current();
    uivec3_vector resources = ent_->resources();
    std::map<nsstring, nsresource*> res_map;
    for (uint32 i = 0; i < resources.size(); ++i)
    {
        nsplugin * plg = nse.plugin(resources[i].x);
        //nsresource * res = plg->get()
    }
}

void entity_widget::set_preview(Preview * prev_)
{
    m_prev = prev_;
}

nsentity * entity_widget::editing_ent()
{
    return m_editing_ent;
}
