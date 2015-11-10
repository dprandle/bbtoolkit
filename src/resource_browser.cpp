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
#include <nsrender_comp.h>

#include <toolkit.h>
#include <resource_browser.h>
#include <resource_dialog.h>
#include <resource_dialog_prev.h>
#include <resource_dialog_prev_lighting.h>
#include <import_resources_dialog.h>
#include <ui_import_resources_dialog.h>
#include <QMessageBox>
#include <QCheckBox>

resource_browser::resource_browser(QWidget * parent) :
    QMainWindow(parent),
    m_import_res_dialog(new import_resources_dialog(this))
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
    bbtk.res_dialog_prev()->set_material(NULL);
    bbtk.res_dialog_prev()->exec();
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
    m_import_res_dialog->ui->cb_mesh->setChecked(true);
    m_import_res_dialog->ui->cb_materials->setChecked(false);
    m_import_res_dialog->ui->cb_anim_set->setChecked(false);
    if (m_import_res_dialog->exec() == QDialog::Accepted)
    {
        bool load_mesh = m_import_res_dialog->ui->cb_mesh->isChecked();
        bool load_mats = m_import_res_dialog->ui->cb_materials->isChecked();
        bool load_anim = m_import_res_dialog->ui->cb_anim_set->isChecked();
        nsstring fname = m_import_res_dialog->ui->le_model_fname->text().toStdString();

        if (!bbtk.res_dialog_prev()->set_mesh(fname))
            return;

        if (load_mats)
        {
            bbtk.map_view()->make_current();
            if (!nse.active()->load_model_mats(fname, false))
            {
                QMessageBox mb(this);
                mb.setIcon(QMessageBox::Question);
                mb.setText("Could not load materials from model specified with path " + QString(fname.c_str()) + " - continue loading?");
                mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                int ret = mb.exec();
                if (ret == QMessageBox::No)
                    return;
            }
        }
        if (load_anim)
        {
            bbtk.map_view()->make_current();
            if (!nse.active()->load_model_anim(fname, false))
            {
                QMessageBox mb(this);
                mb.setIcon(QMessageBox::Question);
                mb.setText("Could not load animation from model specified with path " + QString(fname.c_str()) + " - continue loading?");
                mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                int ret = mb.exec();
                if (ret == QMessageBox::No)
                    return;
            }
        }
        bbtk.res_dialog_prev()->exec();
    }
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
        if (bbtk.res_dialog_prev()->set_entity(res))
            bbtk.res_dialog_prev()->exec();
    }
    else if (res_type_id == type_to_hash(nsinput_map))
    {
        nsinput_map * res = nse.resource<nsinput_map>(resid);
        bbtk.res_dialog()->exec();
    }
    else if (res_type_id == type_to_hash(nsmesh))
    {
        nsmesh * res = nse.resource<nsmesh>(resid);
        std::vector<fvec3_vector> vert_copy(res->count());
        fmat4 node_copy;
        if (res->tree()->m_root == NULL)
        {
            for (uint32 i = 0; i < res->count(); ++i)
            {
                nsmesh::submesh * sub = res->sub(i);
                vert_copy[i].resize(sub->m_verts.size());
                std::copy(sub->m_verts.begin(), sub->m_verts.end(),vert_copy[i].begin());
            }
        }
        else
            node_copy = res->tree()->m_root->m_world_tform;

        if (!bbtk.res_dialog_prev()->set_mesh(res))
            return;

        if (bbtk.res_dialog_prev()->exec() == QDialog::Accepted)
        {
            bbtk.map_view()->make_current();
            nsmesh * fin = nse.resource<nsmesh>(bbtk.res_dialog_prev()->get_editing_res());
            if (fin == NULL)
            {
                QMessageBox mb2(this);
                mb2.setIcon(QMessageBox::Critical);
                mb2.setText("Okay was pressed, but nothing was returned - error code: ALEX_44_IS_44_DUMB_1112");
                mb2.exec();
                return;
            }
            // Check to see if the mesh verts changed..
            bool change = false;
            if (fin->tree()->m_root == NULL)
            {
                for (uint32 i = 0; i < fin->count(); ++i)
                {
                    nsmesh::submesh * subm = fin->sub(i);
                    for (uint32 j = 0; j < subm->m_verts.size(); ++j)
                        change = change || subm->m_verts[j] != vert_copy[i][j];
                }
            }
            else
                change = change || fin->tree()->m_root->m_world_tform != node_copy;

            if (change)
            {
                auto iter = nse.plugins()->begin();
                while (iter != nse.plugins()->end())
                {
                    nsplugin * plg = nse.plugin(iter->first);
                    nsentity_manager * nsem = plg->manager<nsentity_manager>();
                    auto iter2 = nsem->begin();
                    while (iter2 != nsem->end())
                    {
                        nsentity * ent = plg->get<nsentity>(iter2->first);
                        nsoccupy_comp * oc = ent->get<nsoccupy_comp>();
                        nsrender_comp * rc = ent->get<nsrender_comp>();
                        if (oc != NULL && rc != NULL)
                        {
                            if (fin->full_id() == rc->mesh_id())
                            {
                                QMessageBox mb2(this);
                                mb2.setIcon(QMessageBox::Critical);
                                mb2.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                                mb2.setText((nsstring("You have changed a mesh which is being referenced by the entity \"") + ent->name() + nsstring(" \" - would you like to auto-recalculate the occupied spaces")).c_str());
                                int res = mb2.exec();
                                if (res == QMessageBox::Yes)
                                {
                                    nsscene * cur_scn = nse.current_scene();
                                    if (cur_scn->entity(ent->full_id()) != NULL)
                                    {
                                        QMessageBox mb3(this);
                                        mb3.setIcon(QMessageBox::Critical);
                                        mb3.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort);
                                        mb3.setText((nsstring("The entity \"") + ent->name() + nsstring("\" is in the current map - it must be removed to continue. Would you like to remove it and retry or abort?")).c_str());
                                        int res = mb3.exec();
                                        if (res == QMessageBox::Retry)
                                            cur_scn->remove(ent);
                                        else
                                            return;
                                    }
                                    oc->build(fin->aabb());
                                }
                            }
                        }
                        ++iter2;
                    }
                    ++iter;
                }
            }
        }
    }
    else if (res_type_id == type_to_hash(nsmaterial))
    {
        nsmaterial * res = nse.resource<nsmaterial>(resid);
        if (bbtk.res_dialog_prev()->set_material(res))
            bbtk.res_dialog_prev()->exec();
    }
    else if (res_type_id == type_to_hash(nsanim_set))
    {
        nsanim_set * res = nse.resource<nsanim_set>(resid);
        bbtk.res_dialog()->exec();
    }
    else if (res_type_id == type_to_hash(nstexture))
    {
        nstexture * res = nse.resource<nstexture>(resid);
        if (!bbtk.res_dialog_prev()->set_texture(res))
            return;
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
            mb.setIcon(QMessageBox::Critical);
            mb2.setText("Could not delete resource - Error code : 123Alex44is999Dumb");
            mb2.exec();
        }
        show_type(res_type_id);
    }
}
