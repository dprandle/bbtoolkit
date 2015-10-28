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
#include <QMessageBox>
#include <QCheckBox>

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

    bbtk.res_dialog_prev()->show();
    bbtk.res_dialog_prev()->hide();
    bbtk.res_dialog_prev_lighting()->show();
    bbtk.res_dialog_prev_lighting()->hide();
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
    bbtk.map_view()->make_current();
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

            lw_item->setData(Qt::UserRole, res->plugin_id());
            lw_item->setData(Qt::UserRole+1, res->id());
            m_ui.m_res_lw->addItem(lw_item);
            ++res_iter;
        }
        ++plg_iter;
    }
}

void resource_browser::on_a_create_entity_triggered()
{
    bbtk.res_dialog_prev_lighting()->exec();
}

void resource_browser::on_a_create_input_map_triggered()
{
    bbtk.res_dialog()->exec();
}

void resource_browser::on_a_create_shader_triggered()
{
    bbtk.res_dialog()->exec();
}

void resource_browser::on_a_create_material_triggered()
{
    bbtk.res_dialog_prev_lighting()->exec();
}

void resource_browser::on_a_create_texture_triggered()
{
    bbtk.res_dialog_prev()->set_texture();
    bbtk.res_dialog_prev()->exec();
}

void resource_browser::on_a_create_animation_triggered()
{
    bbtk.res_dialog()->exec();
}

void resource_browser::on_a_create_mesh_triggered()
{
    bbtk.res_dialog_prev()->set_mesh();
    bbtk.res_dialog_prev()->exec();
}

void resource_browser::on_a_edit_resource_triggered()
{
    bbtk.map_view()->make_current();
    auto current_res_type = m_ui.m_res_type_lw->selectedItems().last();
    uint res_type_id = current_res_type->data(Qt::UserRole).toUInt();

    auto selected_items = m_ui.m_res_lw->selectedItems();
    if (selected_items.empty())
        return;
    auto item = selected_items.first();

    uivec2 resid(item->data(Qt::UserRole).toUInt(), item->data(Qt::UserRole+1).toUInt());

    if (res_type_id == type_to_hash(nsentity))
    {
        nsentity * res = nse.resource<nsentity>(resid);
        bbtk.res_dialog_prev_lighting()->exec();
    }
    else if (res_type_id == type_to_hash(nsinput_map))
    {
        nsinput_map * res = nse.resource<nsinput_map>(resid);
        bbtk.res_dialog()->exec();
    }
    else if (res_type_id == type_to_hash(nsmesh))
    {
        nsmesh * res = nse.resource<nsmesh>(resid);
        bbtk.res_dialog_prev()->set_mesh(res);
        bbtk.res_dialog_prev()->exec();
    }
    else if (res_type_id == type_to_hash(nsmaterial))
    {
        nsmaterial * res = nse.resource<nsmaterial>(resid);
        bbtk.res_dialog_prev_lighting()->exec();
    }
    else if (res_type_id == type_to_hash(nsanim_set))
    {
        nsanim_set * res = nse.resource<nsanim_set>(resid);
        bbtk.res_dialog()->exec();
    }
    else if (res_type_id == type_to_hash(nstexture))
    {
        nstexture * res = nse.resource<nstexture>(resid);
        bbtk.res_dialog_prev()->set_texture(res);
        bbtk.res_dialog_prev()->exec();
    }
    else if (res_type_id == type_to_hash(nsshader))
    {
        nsshader * res = nse.resource<nsshader>(resid);
        bbtk.res_dialog()->exec();
    }
    show_type(res_type_id);
}

void resource_browser::on_a_create_resource_triggered()
{
    auto current_res_type = m_ui.m_res_type_lw->selectedItems().last();
    uint res_type_id = current_res_type->data(Qt::UserRole).toUInt();

    if (res_type_id == type_to_hash(nsentity))
    {
        on_a_create_entity_triggered();
    }
    else if (res_type_id == type_to_hash(nsinput_map))
    {
        on_a_create_input_map_triggered();
    }
    else if (res_type_id == type_to_hash(nsmesh))
    {
        on_a_create_mesh_triggered();
    }
    else if (res_type_id == type_to_hash(nsmaterial))
    {
        on_a_create_material_triggered();
    }
    else if (res_type_id == type_to_hash(nsanim_set))
    {
        on_a_create_animation_triggered();
    }
    else if (res_type_id == type_to_hash(nstexture))
    {
        on_a_create_texture_triggered();
    }
    else if (res_type_id == type_to_hash(nsshader))
    {
        on_a_create_shader_triggered();
    }
    show_type(res_type_id);
}

void resource_browser::on_a_del_resource_triggered()
{
    QMessageBox mb(this);
    mb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    QCheckBox cb;
    cb.setText("Remove file from hard drive");
    cb.setChecked(true);
    mb.setCheckBox(&cb);
    bool deleted = false;
    int ret = mb.exec();
    if (ret == QMessageBox::Ok)
    {
        bbtk.map_view()->make_current();
        auto current_res_type = m_ui.m_res_type_lw->selectedItems().last();
        uint res_type_id = current_res_type->data(Qt::UserRole).toUInt();

        auto selected_items = m_ui.m_res_lw->selectedItems();
        if (selected_items.empty())
            return;
        auto item = selected_items.first();
        uivec2 resid(item->data(Qt::UserRole).toUInt(), item->data(Qt::UserRole+1).toUInt());

        nsplugin * plg = nse.plugin(resid.x);
        nsresource * res = nse.resource(res_type_id, plg, resid.y);

        if (cb.isChecked())
            deleted = plg->del(res);
        else
            deleted = plg->destroy(res);

        if (!deleted)
        {
            QMessageBox mb2(this);
            mb2.setText("Could not delete resource - Error code : 123Alex44is999Dumb");
            mb2.exec();
        }
        show_type(res_type_id);
    }
}
