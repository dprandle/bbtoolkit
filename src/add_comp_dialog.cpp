#include <nsengine.h>

#include <add_comp_dialog.h>
#include <ui_add_comp_dialog.h>

add_comp_dialog::add_comp_dialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::add_comp_dialog)
{
    m_ui->setupUi(this);
}

add_comp_dialog::~add_comp_dialog()
{
    delete m_ui;
}

uint32 add_comp_dialog::selected_comp()
{
    return m_comp_type;
}

void add_comp_dialog::on_tb_anim_pressed()
{
    m_comp_type = hash_id("nsanim_comp");
    accept();
}

void add_comp_dialog::on_tb_cam_pressed()
{
    m_comp_type = hash_id("nscam_comp");
    accept();
}

void add_comp_dialog::on_tb_light_pressed()
{
    m_comp_type = hash_id("nslight_comp");
    accept();
}

void add_comp_dialog::on_tb_occupy_pressed()
{
    m_comp_type = hash_id("nsoccupy_comp");
    accept();
}

void add_comp_dialog::on_tb_render_pressed()
{
    m_comp_type = hash_id("nsrender_comp");
    accept();
}

void add_comp_dialog::on_tb_tile_pressed()
{
    m_comp_type = hash_id("nstile_comp");
    accept();
}

void add_comp_dialog::on_tb_tile_brush_pressed()
{
    m_comp_type = hash_id("nstile_brush_comp");
    accept();
}

void add_comp_dialog::on_tb_terrain_pressed()
{
    m_comp_type = hash_id("nsterrain_comp");
    accept();
}

void add_comp_dialog::on_tb_sel_pressed()
{
    m_comp_type = hash_id("nssel_comp");
    accept();
}

void add_comp_dialog::on_tb_particle_pressed()
{
    m_comp_type = hash_id("nsparticle_comp");
    accept();
}
