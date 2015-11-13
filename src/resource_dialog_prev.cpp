#include <nsengine.h>
#include <nsplugin.h>
#include <nsinput_map.h>
#include <nscamera_system.h>
#include <nsinput_system.h>
#include <nscam_comp.h>
#include <nsentity.h>
#include <nsscene.h>
#include <nsmesh.h>
#include <nsmaterial.h>
#include <nsrender_comp.h>
#include <nstexture.h>
#include <nsrender_system.h>
#include <nsshader.h>
#include <nssel_comp.h>
#include <nstex_manager.h>
#include <nsselection_system.h>
#include <nsselection_system.h>
#include <nsfile_os.h>
#include <nslight_comp.h>
#include <nsplugin_manager.h>

#include <toolkit.h>
#include <select_res_dialog.h>
#include <resource_dialog_prev.h>
#include <mesh_widget.h>

#include <texture_widget.h>
#include <ui_texture_widget.h>

#include <material_widget.h>
#include <entity_widget.h>
#include <select_res_dialog.h>
#include <component_selection_widget.h>

#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QTreeWidget>
#include <QPixmap>
#include <QBitmap>
#include <QSpacerItem>
#include <set>

resource_dialog_prev::resource_dialog_prev(QWidget * parent):
    QDialog(parent),
    m_editing_res(NULL),
    m_tex_widget(new texture_widget()),
    m_mesh_widget(new mesh_widget()),
    m_mat_widget(new material_widget()),
    m_ent_widget(new entity_widget()),
    m_comp_sel(new component_selection_widget())
{
    m_ui.setupUi(this);
    connect(m_tex_widget, SIGNAL(edit_res_change(nsresource*)), this, SLOT(edit_res_changed(nsresource*)));
    connect(m_ent_widget, SIGNAL(set_comp_sel()), this, SLOT(comp_sel_widget()));
    connect(m_comp_sel, SIGNAL(edit_component(uint32)), this, SLOT(entity_edit_component(uint32)));
}

void resource_dialog_prev::init()
{
    m_ui.m_preview->init();
    m_ent_widget->init(m_ui.m_preview);
    m_mat_widget->init(m_ui.m_preview);
    m_mesh_widget->init(m_ui.m_preview);
    m_comp_sel->init(m_ui.m_preview);
    m_tex_widget->init(m_ui.m_preview);
}

uivec2 resource_dialog_prev::get_editing_res()
{
    return m_last_edit;
}

bool resource_dialog_prev::set_mesh(const nsstring & model_fname)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_mesh_widget);
    _reset_fields();

    if (!_setup_resource_ui(NULL))
        return false;

    // Now lets set up the mesh in the preview context
    nsplugin * plg = nse.active();
    nsmesh * msh = plg->load_model_mesh(model_fname, false);
    if (msh == NULL)
    {
        mbox("Mesh Error", "Could not find mesh specified with path " + model_fname);
        on_m_cancel_btn_pressed();
        return false;
    }
    m_editing_res = msh;

    // Indicate that an mesh from the main context is being edited
    _setup_preview_controls_full();
    nse.system<nsinput_system>()->push_context("cam_control");

    m_mesh_widget->set_mesh(msh);
    return true;
}


bool resource_dialog_prev::set_mesh(nsmesh * mesh_)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_mesh_widget);
    _reset_fields();

    if (!_setup_resource_ui(mesh_))
        return false;

    // Now lets set up the mesh in the preview context
    nsplugin * plg = nse.active();
    nsmesh * msh = plg->load<nsmesh>(mesh_->subdir() + mesh_->name() + mesh_->extension());

    if (msh == NULL)
    {
        int ret = mboxqb("Unsaved Resource", "Cannot edit unsaved resource - would you like to save to file?", QMessageBox::Yes | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
        {
            bbtk.map()->make_current();
            nsplugin * eplg = nse.plugin(m_starting_plug);
            eplg->save<nsmesh>(mesh_->name());
            m_ui.m_preview->make_current();
            msh = plg->load<nsmesh>(mesh_->subdir() + mesh_->name() + mesh_->extension());
        }
        else
        {
            on_m_cancel_btn_pressed();
            return false;
        }
    }

    m_editing_res = msh;
    _setup_preview_controls_full();
    nse.system<nsinput_system>()->push_context("cam_control");
    m_mesh_widget->set_mesh(msh);
    return true;
}

void resource_dialog_prev::_reset_fields()
{
    m_ui.m_name_le->clear();
    m_ui.m_icon_path_le->clear();
    m_ui.m_folder_le->clear();
    m_ui.m_plugin_le->clear();
    m_ui.m_icon_lbl->clear();
    m_starting_plug.clear();
    m_starting_res.clear();
    m_starting_subdir.clear();
    m_editing_res = NULL;
    setResult(QDialog::Accepted);
}

bool resource_dialog_prev::_setup_resource_ui(nsresource * incoming)
{
    std::map<nsplugin*,std::set<nsresource*>> dependencies;
    // make main window context current and get the passed in meshes owning plugin name
    nse.make_current(bbtk.map()->glew_id());
    if (incoming != NULL)
    {
        m_starting_plug = nse.plugin(incoming->plugin_id())->name();
        m_starting_res = incoming->name();
        m_starting_subdir = incoming->subdir();
        m_ui.m_icon_path_le->setText(incoming->icon_path().c_str());
        m_ui.m_icon_lbl->setPixmap(QPixmap(incoming->icon_path().c_str()));

        // Add dependencies
        uivec3_vector dep_vec = incoming->resources();
        for (uint32 i = 0; i < dep_vec.size(); ++i)
        {
            nsplugin * plg = nse.plugin(dep_vec[i].x);
            if (plg != NULL)
            {
                nsresource * res = plg->get(dep_vec[i].z, dep_vec[i].y);
                if (res != NULL)
                    dependencies[plg].insert(res);
            }
        }
    }
    else
    {
        m_starting_plug = nse.active()->name();
        m_starting_res = "";
        m_starting_subdir = "";
    }

    // Setup resource preview dialog to match the mesh
    m_ui.m_plugin_le->setText(m_starting_plug.c_str());
    m_ui.m_name_le->setText(m_starting_res.c_str());
    m_ui.m_folder_le->setText(m_starting_subdir.c_str());

    m_ui.m_preview->make_current();
    nsplugin * plg = nse.create_plugin(m_starting_plug);
    if (plg == NULL)
    {
        mbox("Preview Error", "Could not load preview plugin");
        on_m_cancel_btn_pressed();
        return false;
    }

    // Load all resources that we depend on for editing
    auto plug_iter = dependencies.begin();
    while (plug_iter != dependencies.end())
    {
        nsplugin * pre_plug = nse.plugin(plug_iter->first->name());
        if (pre_plug == NULL)
        {
            pre_plug = nse.create_plugin(plug_iter->first->name());
            dprint("Loading " + m_starting_res + " dependencies: Created plugin with name " + pre_plug->name());
        }
        else
        {
            dprint("Loading " + m_starting_res + " dependencies: Found plugin with name " + pre_plug->name());
        }

        auto res_iter = plug_iter->second.begin();
        while (res_iter != plug_iter->second.end())
        {
            nsresource * res = pre_plug->get((*res_iter)->type(), (*res_iter)->id());
            if (res == NULL)
            {
                res = pre_plug->load((*res_iter)->type(),(*res_iter)->subdir() + (*res_iter)->name() + (*res_iter)->extension());
                if (res == NULL)
                {
                    int ret = mboxb("Dependency Error", "This resource has an unresolved dependency - the " + hash_to_guid((*res_iter)->type()).substr(2) + " with name " + (*res_iter)->name() + " is loaded but has not been saved to file. Would you like to save now?", QMessageBox::Yes | QMessageBox::Cancel);
                    if (ret == QMessageBox::Yes)
                    {
                        bbtk.map()->make_current();
                        nsplugin * ct1_plg = nse.plugin(plug_iter->first->name());
                        ct1_plg->save((*res_iter));
                        m_ui.m_preview->make_current();
                        res = pre_plug->load((*res_iter)->type(), (*res_iter)->subdir() + (*res_iter)->name() + (*res_iter)->extension());
                        dprint("Loading " + m_starting_res + " dependencies: Saved and loaded " + hash_to_guid(res->type()) + " with name " + res->name());
                    }
                    else
                    {
                        on_m_cancel_btn_pressed();
                        return false;
                    }
                }
                else
                {
                    dprint("Loading " + m_starting_res + " dependencies: Loaded " + hash_to_guid(res->type()) + " with name " + res->name());
                }
            }
            else
            {
                dprint("Loading " + m_starting_res + " dependencies: Found " + hash_to_guid(res->type()) + " with name " + res->name());
            }
            ++res_iter;
        }
        ++plug_iter;
    }

    nse.set_active(plg);
    return true;
}

void resource_dialog_prev::on_sldr_diffuse_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nslight_comp * lc = prev_lt->get<nslight_comp>();
    lc->set_intensity(float(m_ui.sldr_diffuse->value()) / 100.0f, float(m_ui.sldr_ambient->value()) / 100.0f);
}

void resource_dialog_prev::on_cb_shadows_toggled(bool enable)
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nslight_comp * lc = prev_lt->get<nslight_comp>();
    lc->set_cast_shadows(enable);
}

void resource_dialog_prev::on_sldr_ambient_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nslight_comp * lc = prev_lt->get<nslight_comp>();
    lc->set_intensity(float(m_ui.sldr_diffuse->value()) / 100.0f, float(m_ui.sldr_ambient->value()) / 100.0f);
}

void resource_dialog_prev::on_sldr_horiz_plane_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nstform_comp * tc = prev_lt->get<nstform_comp>();
    tc->enable_parent(true);
    tc->set_parent(rotation_mat4(fvec3(60.0f - m_ui.slider_vert_plane->value() * 0.6f,
                                       0.0f,
                                       m_ui.sldr_horiz_plane->value()*3.6f - 180.0f),
                                 fvec3::ZYX));
}

void resource_dialog_prev::on_slider_vert_plane_valueChanged()
{
    m_ui.m_preview->make_current();
    nsentity * prev_lt = nse.active()->get<nsentity>(PREV_LIGHT_NAME);
    if (prev_lt == NULL)
        return;
    nstform_comp * tc = prev_lt->get<nstform_comp>();
    tc->enable_parent(true);
    tc->set_parent(rotation_mat4(fvec3(60.0f - m_ui.slider_vert_plane->value() * 0.6f,
                                       0.0f,
                                       m_ui.sldr_horiz_plane->value()*3.6f - 180.0f),
                                 fvec3::ZYX));
}

void resource_dialog_prev::reject()
{
    m_ui.m_preview->make_current();
    nsplugin * plg = nse.active();
    nse.system<nsselection_system>()->clear();
    nse.system<nsinput_system>()->clear_contexts();
    if (plg == nullptr)
        return;
    nse.set_active(nullptr);
    while (nse.plugins()->begin() != nse.plugins()->end())
        nse.destroy_plugin(nse.plugins()->begin()->first);
    m_last_edit = uivec2();
    m_editing_res = nullptr;
    QDialog::reject();
}

bool resource_dialog_prev::set_material(nsmaterial * mat_)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_mat_widget);
    _reset_fields();

    if (!_setup_resource_ui(mat_))
        return false;

    nsplugin * plg = nse.active();
    nsmaterial * mat;
    if (mat_ != NULL)
    {
        mat = plg->load<nsmaterial>(mat_->subdir() + mat_->name() + mat_->extension());
        if (mat == NULL)
        {
            int ret = mboxqb("Unsaved Resource","Cannot edit unsaved resource - would you like to save to file?",QMessageBox::Yes | QMessageBox::Cancel);
            if (ret == QMessageBox::Yes)
            {
                bbtk.map()->make_current();
                nsplugin * eplg = nse.plugin(m_starting_plug);
                eplg->save<nsmaterial>(mat_->name());
                m_ui.m_preview->make_current();
                mat = plg->load<nsmaterial>(mat_->subdir() + mat_->name() + mat_->extension());
            }
            else
            {
                on_m_cancel_btn_pressed();
                return false;
            }
        }
        mat->set_icon_path(mat_->icon_path());
    }
    else
        mat = plg->create<nsmaterial>("preview_mat");

    m_editing_res = mat;
    _setup_preview_controls_full();
    nse.system<nsinput_system>()->push_context("cam_control");
    m_mat_widget->set_material(mat);
    return true;
}

bool resource_dialog_prev::set_texture(nstexture * tex_)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_tex_widget);
    m_tex_widget->clear_fields();
    _reset_fields();

    if (!_setup_resource_ui(tex_))
        return false;

    _setup_preview_controls_limited();
    m_tex_widget->set_texture(tex_);
    return true;
    // main camera controls
}

void resource_dialog_prev::edit_res_changed(nsresource * res)
{
    m_editing_res = res;
}

void resource_dialog_prev::_setup_preview_controls_limited()
{
    nsinput_map * imap = nse.active()->create<nsinput_map>("resource_dialog_prev");
    imap->create_context("cam_control");

    nsinput_map::trigger camtilt(
        NSCAM_TILTPAN,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camtilt.add_key_mod(nsinput_map::key_any);
    camtilt.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement, camtilt);

    nsinput_map::trigger cammove(
        NSCAM_MOVE,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos
        );
    cammove.add_key_mod(nsinput_map::key_any);
    cammove.add_mouse_mod(nsinput_map::left_button);
    cammove.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,cammove);

    nsinput_map::trigger camzoom(
        NSCAM_ZOOM,
        nsinput_map::t_pressed,
        nsinput_map::axis_scroll_delta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camzoom.add_key_mod(nsinput_map::key_any);
    imap->add_mouse_trigger("cam_control", nsinput_map::scrolling, camzoom);

    nse.system<nsinput_system>()->set_input_map(imap->full_id());
}

void resource_dialog_prev::_setup_preview_controls_full()
{
    nsinput_map * imap = nse.active()->create<nsinput_map>("resource_dialog_prev");
    imap->create_context("cam_control");

    nsinput_map::trigger camtilt(
        NSCAM_TILTPAN,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camtilt.add_key_mod(nsinput_map::key_any);
    camtilt.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement, camtilt);

    nsinput_map::trigger cammove(
        NSCAM_MOVE,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos
        );
    cammove.add_key_mod(nsinput_map::key_any);
    cammove.add_mouse_mod(nsinput_map::left_button);
    cammove.add_mouse_mod(nsinput_map::right_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,cammove);

    nsinput_map::trigger camzoom(
        NSCAM_ZOOM,
        nsinput_map::t_pressed,
        nsinput_map::axis_scroll_delta | nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos);
    camzoom.add_key_mod(nsinput_map::key_any);
    imap->add_mouse_trigger("cam_control", nsinput_map::scrolling, camzoom);

    nsinput_map::trigger selectentity(
        NSSEL_SELECT,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xpos | nsinput_map::axis_mouse_ypos
        );
    imap->add_mouse_trigger("cam_control", nsinput_map::left_button,selectentity);

    nsinput_map::trigger selectmove(
        NSSEL_MOVE,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmove.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmove);

    nsinput_map::trigger selectmovexy(
        NSSEL_MOVE_XY,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovexy.add_key_mod(nsinput_map::key_x);
    selectmovexy.add_key_mod(nsinput_map::key_y);
    selectmovexy.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovexy);

    nsinput_map::trigger selectmovezy(
        NSSEL_MOVE_ZY,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovezy.add_key_mod(nsinput_map::key_z);
    selectmovezy.add_key_mod(nsinput_map::key_y);
    selectmovezy.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovezy);

    nsinput_map::trigger selectmovezx(
        NSSEL_MOVE_ZX,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovezx.add_key_mod(nsinput_map::key_z);
    selectmovezx.add_key_mod(nsinput_map::key_x);
    selectmovezx.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovezx);

    nsinput_map::trigger selectmovex(
        NSSEL_MOVE_X,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovex.add_mouse_mod(nsinput_map::left_button);
    selectmovex.add_key_mod(nsinput_map::key_x);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovex);

    nsinput_map::trigger selectmovey(
        NSSEL_MOVE_Y,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovey.add_key_mod(nsinput_map::key_y);
    selectmovey.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovey);

    nsinput_map::trigger selectmovez(
        NSSEL_MOVE_Z,
        nsinput_map::t_pressed,
        nsinput_map::axis_mouse_xdelta | nsinput_map::axis_mouse_ydelta
        );
    selectmovez.add_key_mod(nsinput_map::key_z);
    selectmovez.add_mouse_mod(nsinput_map::left_button);
    imap->add_mouse_trigger("cam_control", nsinput_map::movement,selectmovez);

    nsinput_map::trigger selectmovetoggle(
        NSSEL_MOVE_TOGGLE,
        nsinput_map::t_toggle
        );
    selectmovetoggle.add_key_mod(nsinput_map::key_any);
    imap->add_mouse_trigger("cam_control", nsinput_map::left_button,selectmovetoggle);

    nse.system<nsinput_system>()->set_input_map(imap->full_id());
}

void resource_dialog_prev::on_m_okay_btn_pressed()
{
    m_ui.m_preview->make_current();
    nsplugin * plg = nse.active();
    if (plg == nullptr)
        return;

    if (m_editing_res != NULL)
    {
        nsstring new_name = m_ui.m_name_le->text().toStdString();

        QString subdir = m_ui.m_folder_le->text();
        if (!subdir.isEmpty() && subdir[subdir.size()-1] != '/')
            subdir.append('/');

        // validate our resource stuff
        if (m_starting_res.empty())
        {
            nsstring name = m_ui.m_name_le->text().toStdString();
            nsstring plugin = m_ui.m_plugin_le->text().toStdString();

            if (name.empty())
            {
                mbox("Name Error", "Please choose a name for your resource");
                return;
            }

            if (plugin.empty())
            {
                mbox("Name Error", "Please choose a valid plugin");
                return;
            }

            bbtk.map()->make_current();
            if (nse.resource(m_editing_res->type(), nse.plugin(plugin),name) != NULL)
            {
                mbox("Name Conflict", "The " + nse.guid(m_editing_res->type()).substr(2) + " \"" + name + "\" already exists in plugin " + plugin + " - please choose a different name");
                return;
            }
            m_ui.m_preview->make_current();
        }

        m_mesh_widget->done();
        m_ent_widget->done();
        m_mat_widget->done();
        m_tex_widget->done();

        m_editing_res->set_subdir(subdir.toStdString());
        m_editing_res->rename(new_name);
        nsstring fname = m_editing_res->subdir() + m_editing_res->name() + m_editing_res->extension();
        uint32 res_type_id = m_editing_res->type();

        // remove the original resource from file and from being loaded in to the main window context
        if (!m_starting_res.empty())
        {
            // Make main window's context current
            bbtk.map()->make_current();

            nsplugin * eng_plug = nse.plugin(m_starting_plug);
            nsresource * res = eng_plug->get(res_type_id, m_starting_res);
            eng_plug->del(res);

            // Propagate name change so that deleted resource is replaced by new resource
            nse.name_change(uivec2(hash_id(m_starting_plug),hash_id(m_starting_res)), m_editing_res->full_id());

            // Switch back to the preview's context
            m_ui.m_preview->make_current();
        }

        // If the plugin name le is different than the current plugin name, rename the current plugin
        nsstring new_plg_name = m_ui.m_plugin_le->text().toStdString();
        if (plg->name() != new_plg_name)
            plg->rename(new_plg_name);

        // Now save under this plugin name
        plg->save(m_editing_res);

        // Get the icon path from the icon path le and attempt to save the generated icon to file
        // If that fails, set the path to empty
        nsstring icon_pth = m_ui.m_icon_path_le->text().toStdString();
        if (!icon_pth.empty())
        {
            nsfile_os::create_dir(icon_pth);
            if (!m_ui.m_icon_lbl->pixmap()->save(icon_pth.c_str()))
                icon_pth = "";
        }

        // Make main window context current and load the previously saved resource from file
        bbtk.map()->make_current();
        nsplugin * new_plug = nse.plugin(new_plg_name);
        if (new_plug == NULL)
            mbox("Plugin Error", "Could not find plugin file specified!");
        else
        {
            nsresource * res = new_plug->load(res_type_id, fname);
            if (res == NULL)
                mbox("Plugin Error", "Could not load resource in to plugin - Some kind of mischeif has crept in to the system!");

            // Set resources icon path - if the generated icon was unable to save this will just be an empty string
            res->set_icon_path(icon_pth);
        }

        // Make the preview's context current
        m_ui.m_preview->make_current();
    }
    m_last_edit = m_editing_res->full_id();

    // Now destroy all resources in the plugin and reset the editing resource
    nse.set_active(nullptr);
    nse.system<nsselection_system>()->clear();
    nse.system<nsinput_system>()->clear_contexts();
    nse.destroy_plugin(plg);

    while (nse.plugins()->begin() != nse.plugins()->end())
        nse.destroy_plugin(nse.plugins()->begin()->first);

    m_editing_res = nullptr;
    bbtk.map()->update();
    accept();
}

void resource_dialog_prev::on_m_cancel_btn_pressed()
{
    reject();
}

void resource_dialog_prev::on_m_plugin_tb_pressed()
{
    bbtk.map()->make_current();
    select_res_dialog get_plug(this);
    get_plug.show_type<nsplugin>();
    if (get_plug.exec() == QDialog::Accepted)
    {
        nsplugin * plg = get_plug.selected_resource<nsplugin>();
        m_ui.m_plugin_le->setText(plg->name().c_str());
    }
    m_ui.m_preview->make_current();
}

void resource_dialog_prev::on_m_folder_tb_pressed()
{
    QString fname = QFileDialog::getExistingDirectory(this,
                                                 "Resource Subdirectory",
                                                 "./resources/" + m_ui.m_plugin_le->text(),
                                                 QFileDialog::DontUseNativeDialog | QFileDialog::ShowDirsOnly);
    nsstring to_set = nsres_manager::name_from_filename(fname.toStdString());
    if (!to_set.empty())
        to_set += '/';
    m_ui.m_folder_le->setText(to_set.c_str());
}

void resource_dialog_prev::on_m_icon_path_tb_pressed()
{
    QString fname = QFileDialog::getOpenFileName(this,
                                                 "Image File",
                                                 "import/",
                                                 "Image Files (*.png *.jpg *.bmp)",
                                                 0,
                                                 QFileDialog::DontUseNativeDialog);
    m_ui.m_icon_path_le->setText(fname);
    QPixmap pm;
    if (pm.load(fname))
        m_ui.m_icon_lbl->setPixmap(pm);
}


void resource_dialog_prev::on_m_icon_create_btn_pressed()
{
    nsstring name = m_ui.m_name_le->text().toStdString();

    if (m_editing_res == NULL)
    {
        mbox("Icon Error", "The editing resource is NULL - just like the brain of Alex! What did you do???");
        return;
    }

    if (name.empty())
    {
        mbox("Icon Error", "Please provide a name for the resource before making the icon");
        return;
    }

    QPixmap pixMap = QPixmap::grabWidget(m_ui.m_preview).scaled(96,96);// = QPixmap::grabWidget(m_ui.m_preview).scaled(70,70);
    pixMap.setMask(pixMap.createMaskFromColor(QColor(int(BG_R*255),int(BG_G*255),int(BG_B*255),0)));

    bbtk.map()->make_current();
    nsplugin * plg = nse.plugin(m_ui.m_plugin_le->text().toStdString());
    if (plg == NULL)
    {
        m_ui.m_preview->make_current();
        mbox("Icon Error", "Could not find plugin specified - Cannot create icon!");
        return;
    }

    nsstring icon_path = plg->manager<nstex_manager>()->res_dir() +
            plg->manager<nstex_manager>()->local_dir() +
            "icons/" + name + ".png";
    m_ui.m_preview->make_current();
    m_ui.m_icon_lbl->setPixmap(pixMap);
    m_ui.m_icon_path_le->setText(icon_path.c_str());
}

bool resource_dialog_prev::set_entity(nsentity * ent_)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_comp_sel);
    _reset_fields();

    if (!_setup_resource_ui(ent_))
        return false;

    nsplugin * plg = nse.active();
    nsentity * ent = NULL;
    if (ent_ != NULL)
    {
        ent = plg->load<nsentity>(ent_->subdir() + ent_->name() + ent_->extension());
        if (ent == NULL)
        {
            int ret = mboxqb("Unsaved Resource", "Cannot edit unsaved resource - would you like to save to file?", QMessageBox::Yes | QMessageBox::Cancel);
            if (ret == QMessageBox::Yes)
            {
                bbtk.map()->make_current();
                nsplugin * eplg = nse.plugin(m_starting_plug);
                eplg->save<nsentity>(ent_->name());
                m_ui.m_preview->make_current();
                ent = plg->load<nsentity>(ent_->subdir() + ent_->name() + ent_->extension());
            }
            else
            {
                on_m_cancel_btn_pressed();
                return false;
            }
        }
    }
    else
        ent = plg->create<nsentity>("preview_ent");

    m_editing_res = ent;
    _setup_preview_controls_full();
    nse.system<nsinput_system>()->push_context("cam_control");
    m_ent_widget->set_entity(ent);
    m_comp_sel->set_entity(ent);
    return true;
}

bool resource_dialog_prev::set_entity(const nsstring & model_fname)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_comp_sel);
    _reset_fields();

    if (!_setup_resource_ui(NULL))
        return false;

    nsplugin * plg = nse.active();
    nsentity * ent = plg->load_model("new_ent", model_fname, false);
    if (ent == NULL)
    {
        mbox("File loading error", "Could not find mesh specified with path " + model_fname);
        on_m_cancel_btn_pressed();
        return false;
    }
    m_editing_res = ent;
    _setup_preview_controls_full();
    nse.system<nsinput_system>()->push_context("cam_control");
    m_ent_widget->set_entity(ent);
    m_comp_sel->set_entity(ent);
    return true;
}

void resource_dialog_prev::entity_edit_component(uint32 comp_type)
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_ent_widget);
    m_ent_widget->set_comp_widget(comp_type);
}

void resource_dialog_prev::comp_sel_widget()
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_comp_sel);
}
