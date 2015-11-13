#include <nsengine.h>
#include <nsmesh.h>
#include <nsentity.h>
#include <nsrender_comp.h>
#include <nsplugin.h>

#include <render_comp_widget.h>
#include <select_res_dialog.h>
#include <preview.h>
#include <toolkit.h>
#include <ui_render_comp_widget.h>

render_comp_widget::render_comp_widget(QWidget *parent) :
    component_widget(parent),
    m_ui(new Ui::render_comp_widget)
{
    m_ui->setupUi(this);
}

render_comp_widget::~render_comp_widget()
{
    delete m_ui;
}

uint32 render_comp_widget::type()
{
    return hash_id("nsrender_comp");
}

void render_comp_widget::set_entity(nsentity * ent_)
{
    component_widget::set_entity(ent_);
    nsmesh * msh = nse.resource<nsmesh>(m_edit_ent->get<nsrender_comp>()->mesh_id());
    if (msh != NULL)
    {
        m_ui->le_mesh_name->setText(msh->name().c_str());
        m_ui->le_mesh_plugin->setText(nse.plugin(msh->plugin_id())->name().c_str());
    }
}

void render_comp_widget::on_btn_mesh_pressed()
{
    nsmesh * loaded_mesh = NULL;
    nsstring plug,name;

    select_res_dialog rd(this);
    rd.show_type<nsmesh>();
    if (rd.exec() == QDialog::Accepted)
    {
        bbtk.map()->make_current();
        loaded_mesh = rd.selected_resource<nsmesh>();
        plug = nse.plugin(loaded_mesh->plugin_id())->name();
        name = loaded_mesh->name();
        m_ui->le_mesh_plugin->setText(plug.c_str());
        m_ui->le_mesh_name->setText(name.c_str());
    }
    else
        return;

    m_prev->make_current();
    if (loaded_mesh != NULL)
    {
        nsplugin * plg = nse.plugin(plug);
        if (plg == NULL)
            plg = nse.create_plugin(plug, false);

        nsresource * res = plg->get<nsmesh>(name);
        if (res == NULL)
            res = plg->load(loaded_mesh->type(), loaded_mesh->subdir() + loaded_mesh->name() + loaded_mesh->extension());
        m_edit_ent->get<nsrender_comp>()->set_mesh_id(res->full_id());
    }
}

void render_comp_widget::on_cb_cast_shadows(bool)
{

}

void render_comp_widget::on_tb_clear_mesh()
{

}

void render_comp_widget::on_tb_del_mat()
{

}

void render_comp_widget::on_tb_edit_mat()
{

}

void render_comp_widget::on_tb_add_mat()
{

}
