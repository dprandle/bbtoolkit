// engine stuff
#include <nsunordered_map.h>
#include <nsplugin_manager.h>
#include <nsentity.h>
#include <nsscene.h>
#include <nsmaterial.h>
#include <nsshader.h>
#include <nstexture.h>
#include <nsanim_set.h>
#include <nsinput_map.h>
#include <nsmesh.h>

#include <toolkit.h>
#include <resource_browser.h>
#include <resource_dialog.h>
#include <resource_dialog_prev.h>
#include <resource_dialog_prev_lighting.h>

resource_browser::resource_browser(QWidget * parent) :
	QMainWindow(parent)
{
    m_ui.setupUi(this);
}

resource_browser::~resource_browser()
{
}

void resource_browser::init()
{
    m_default_icons.emplace(type_to_hash(nsentity),":/ResourceIcons/icons/default_entity.png");
    m_default_icons.emplace(type_to_hash(nsshader), ":/ResourceIcons/icons/default_shader.png");
    m_default_icons.emplace(type_to_hash(nsmaterial), ":/ResourceIcons/icons/default_material.png");
    m_default_icons.emplace(type_to_hash(nstexture), ":/ResourceIcons/icons/default_texture.png");
    m_default_icons.emplace(type_to_hash(nsanim_set), ":/ResourceIcons/icons/default_anim.png");
    m_default_icons.emplace(type_to_hash(nsmesh), ":/ResourceIcons/icons/default_mesh.png");
    m_default_icons.emplace(type_to_hash(nsinput_map), ":/ResourceIcons/icons/default_entity.png");

    auto item = m_ui.m_res_type_lw->findItems("Entities",Qt::MatchExactly).first();
    item->setData(Qt::UserRole, type_to_hash(nsentity));

    item = m_ui.m_res_type_lw->findItems("Input Maps",Qt::MatchExactly).first();
    item->setData(Qt::UserRole, type_to_hash(nsinput_map));

    item = m_ui.m_res_type_lw->findItems("Meshes",Qt::MatchExactly).first();
    item->setData(Qt::UserRole, type_to_hash(nsmesh));

    item = m_ui.m_res_type_lw->findItems("Materials",Qt::MatchExactly).first();
    item->setData(Qt::UserRole, type_to_hash(nsmaterial));

    item = m_ui.m_res_type_lw->findItems("Animations",Qt::MatchExactly).first();
    item->setData(Qt::UserRole, type_to_hash(nsanim_set));

    item = m_ui.m_res_type_lw->findItems("Textures",Qt::MatchExactly).first();
    item->setData(Qt::UserRole, type_to_hash(nstexture));

    item = m_ui.m_res_type_lw->findItems("Shaders",Qt::MatchExactly).first();
    item->setData(Qt::UserRole, type_to_hash(nsshader));
}

void resource_browser::change_resource_type()
{
    auto item = m_ui.m_res_type_lw->selectedItems().first();
    if (item == NULL)
        return;
    show_type(item->data(Qt::UserRole).toUInt());
}

void resource_browser::show_type(uint32 guid_)
{
    m_ui.m_res_lw->clear();

    auto plg_iter = nse.plugins()->begin();

    while (plg_iter != nse.plugins()->end())
    {
        nsplugin * plg = nse.plugin(plg_iter->first);
        auto manager = plg->manager(nse.manager_id(guid_));

        if (manager == NULL)
        {
            ++plg_iter;
            continue;
        }

        auto res_iter = manager->begin();
        while (res_iter != manager->end())
        {
            nsresource * res = manager->get(res_iter->first);
            QListWidgetItem * lw_item = new QListWidgetItem(res->name().c_str());

            QString icon_path = res->icon_path().c_str();

            if (!icon_path.isEmpty())
                lw_item->setIcon(QIcon(icon_path));
            else
                lw_item->setIcon(QIcon(m_default_icons[guid_].c_str()));

            m_ui.m_res_lw->addItem(lw_item);
            ++res_iter;
        }
        ++plg_iter;
    }
}

void resource_browser::on_a_create_entity_triggered()
{
    if (bbtk.res_dialog_prev_lighting()->exec() == QDialog::Accepted)
    {

    }
    else
    {

    }
}

void resource_browser::on_a_create_map_triggered()
{
    if (bbtk.res_dialog()->exec() == QDialog::Accepted)
    {

    }
    else
    {

    }
}

void resource_browser::on_a_create_plugin_triggered()
{
    if (bbtk.res_dialog()->exec() == QDialog::Accepted)
    {

    }
    else
    {

    }
}

void resource_browser::on_a_create_shader_triggered()
{
    if (bbtk.res_dialog()->exec() == QDialog::Accepted)
    {

    }
    else
    {

    }
}

void resource_browser::on_a_create_material_triggered()
{
    if (bbtk.res_dialog_prev_lighting()->exec() == QDialog::Accepted)
    {

    }
    else
    {

    }
}

void resource_browser::on_a_create_texture_triggered()
{
    bbtk.res_dialog_prev()->set_texture();
    if (bbtk.res_dialog_prev()->exec() == QDialog::Accepted)
    {
    }
    else
    {

    }
}

void resource_browser::on_a_create_animation_triggered()
{
    if (bbtk.res_dialog_prev()->exec() == QDialog::Accepted)
    {

    }
    else
    {

    }
}

void resource_browser::on_a_create_mesh_triggered()
{
    if (bbtk.res_dialog_prev()->exec() == QDialog::Accepted)
    {

    }
    else
    {

    }
}

void resource_browser::on_a_edit_resource_triggered()
{
	
}

void resource_browser::on_a_create_resource_triggered()
{
	
}

void resource_browser::on_a_del_resource_triggered()
{
	
}
