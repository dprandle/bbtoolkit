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
#include <select_res_dialog.h>

select_res_dialog::select_res_dialog(QWidget * parent):
    QDialog(parent)
{
    m_ui.setupUi(this);
    m_default_icons.emplace(type_to_hash(nsentity),":/ResourceIcons/icons/default_entity.png");
    m_default_icons.emplace(type_to_hash(nsshader), ":/ResourceIcons/icons/default_shader.png");
    m_default_icons.emplace(type_to_hash(nsmaterial), ":/ResourceIcons/icons/default_material.png");
    m_default_icons.emplace(type_to_hash(nstexture), ":/ResourceIcons/icons/default_texture.png");
    m_default_icons.emplace(type_to_hash(nsanim_set), ":/ResourceIcons/icons/default_anim.png");
    m_default_icons.emplace(type_to_hash(nsmesh), ":/ResourceIcons/icons/default_mesh.png");
    m_default_icons.emplace(type_to_hash(nsinput_map), ":/ResourceIcons/icons/default_entity.png");
    m_default_icons.emplace(type_to_hash(nsplugin), ":/ResourceIcons/icons/default_entity.png");
}

select_res_dialog::~select_res_dialog()
{

}

void select_res_dialog::show_type(uint32 guid_)
{
//    bbtk.map()->make_current();
//    m_ui.m_lw->clear();

//    nsstring restype = hash_to_guid(guid_).substr(2);
//    restype[0] = toupper(restype[0]);

//    for (uint32 i = 0; i < restype.size()-1; ++i)
//    {
//        if (restype[i] == '_')
//        {
//            restype[i] = ' ';
//            restype[i+1] = toupper(restype[i+1]);
//        }
//    }

//    setWindowTitle(("Select " + restype).c_str());

//    // show core resources
//    auto manager = nse.core()->manager(nse.manager_id(guid_));
//    if (manager != NULL)
//    {
//        auto res_iter = manager->begin();
//        while (res_iter != manager->end())
//        {
//            nsresource * res = manager->get(res_iter->first);
//            QListWidgetItem * lw_item = new QListWidgetItem(res->name().c_str());

//            QString icon_path = res->icon_path().c_str();

//            if (!icon_path.isEmpty())
//                lw_item->setIcon(QIcon(icon_path));
//            else
//                lw_item->setIcon(QIcon(m_default_icons[guid_].c_str()));

//            lw_item->setData(Qt::UserRole, res->plugin_id());
//            lw_item->setData(Qt::UserRole+1, res->id());
//            m_ui.m_lw->addItem(lw_item);
//            ++res_iter;
//        }
//    }

//    auto plg_iter = nse.plugins()->begin();
//    while (plg_iter != nse.plugins()->end())
//    {
//        nsplugin * plg = nse.plugin(plg_iter->first);

//        if (guid_ == type_to_hash(nsplugin))
//        {
//            QListWidgetItem * lw_item = new QListWidgetItem(plg->name().c_str());
//            QString icon_path = plg->icon_path().c_str();

//            if (!icon_path.isEmpty())
//                lw_item->setIcon(QIcon(icon_path));
//            else
//                lw_item->setIcon(QIcon(m_default_icons[guid_].c_str()));

//            lw_item->setData(Qt::UserRole, plg->plugin_id());
//            lw_item->setData(Qt::UserRole+1, plg->id());
//            m_ui.m_lw->addItem(lw_item);
//        }
//        else
//        {
//            auto manager = plg->manager(nse.manager_id(guid_));

//            if (manager == NULL)
//            {
//                ++plg_iter;
//                continue;
//            }

//            auto res_iter = manager->begin();
//            while (res_iter != manager->end())
//            {
//                nsresource * res = manager->get(res_iter->first);
//                QListWidgetItem * lw_item = new QListWidgetItem(res->name().c_str());

//                QString icon_path = res->icon_path().c_str();

//                if (!icon_path.isEmpty())
//                    lw_item->setIcon(QIcon(icon_path));
//                else
//                    lw_item->setIcon(QIcon(m_default_icons[guid_].c_str()));

//                lw_item->setData(Qt::UserRole, res->plugin_id());
//                lw_item->setData(Qt::UserRole+1, res->id());
//                m_ui.m_lw->addItem(lw_item);
//                ++res_iter;
//            }
//        }
//        ++plg_iter;
//    }
}

nsasset * select_res_dialog::selected_resource(uint32 guid_)
{
//    auto items = m_ui.m_lw->selectedItems();
//    if (items.empty())
//        return NULL;
//    auto item = items.first();
//    if (item == NULL)
//        return NULL;

//    uivec2 res_id(item->data(Qt::UserRole).toUInt(), item->data(Qt::UserRole+1).toUInt());
//    if (res_id.x == 0)
//        return nse.plugin(res_id.y);
//    return nse.resource(guid_,nse.plugin(res_id.x),res_id.y);
    return nullptr;
}
