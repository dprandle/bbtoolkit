#include <nsentity.h>
#include <nsengine.h>

#include <preview.h>
#include <component_selection_widget.h>
#include <ui_component_selection_widget.h>
#include <add_comp_dialog.h>

component_selection_widget::component_selection_widget(QWidget *parent) :
    QWidget(parent),
    m_edit_ent(NULL),
    m_ui(new Ui::component_selection_widget),
    m_prev(NULL)
{
    m_ui->setupUi(this);

    m_icons[hash_id("nsanim_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nstile_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nsterrain_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nstile_brush_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nsrender_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nssel_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nscam_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nsparticle_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nsoccupy_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nslight_comp")] = ":/ResourceIcons/icons/default_comp.png";
    m_icons[hash_id("nstform_comp")] = ":/ResourceIcons/icons/default_comp.png";
}

component_selection_widget::~component_selection_widget()
{
    delete m_ui;
}

void component_selection_widget::init(Preview * prev_)
{
    m_prev = prev_;
}

void component_selection_widget::set_entity(nsentity * ent)
{
    m_edit_ent = ent;
    refresh();
}

void component_selection_widget::refresh()
{
    m_ui->lw_comps->clear();
    auto comp_iter = m_edit_ent->begin();
    while (comp_iter != m_edit_ent->end())
    {
        QIcon icon(m_icons[comp_iter->first].c_str());

        nsstring guid_ = hash_to_guid(comp_iter->first).substr(2);
        guid_[0] = toupper(guid_[0]);

        for (uint32 i = 0; i < guid_.size()-1; ++i)
        {
            if (guid_[i] == '_')
            {
                guid_[i] = ' ';
                guid_[i+1] = toupper(guid_[i+1]);
            }
        }

        QListWidgetItem * item = new QListWidgetItem(icon, guid_.c_str());
        item->setData(Qt::UserRole, comp_iter->first);
        m_ui->lw_comps->addItem(item);
        ++comp_iter;
    }
}

void component_selection_widget::on_tb_add_comp_pressed()
{
    m_ui->tb_add_comp->setDown(false);
    add_comp_dialog add_comp_d(this);
    if (add_comp_d.exec() == QDialog::Accepted)
    {
        m_edit_ent->create(add_comp_d.selected_comp());
        refresh();
    }
}

void component_selection_widget::on_tb_del_comp_pressed()
{
    auto items = m_ui->lw_comps->selectedItems();
    if (items.isEmpty())
        return;
    auto item = items.first();

    m_prev->make_current();
    m_edit_ent->destroy(item->data(Qt::UserRole).toUInt());
    m_ui->tb_del_comp->setDown(false);
    refresh();
}

void component_selection_widget::on_tb_edit_comp_pressed()
{
    auto items = m_ui->lw_comps->selectedItems();
    if (items.isEmpty())
        return;
    auto item = items.first();

    m_prev->make_current();
    emit edit_component(item->data(Qt::UserRole).toUInt());
    m_ui->tb_edit_comp->setDown(false);
}
