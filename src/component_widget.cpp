#include <component_widget.h>

component_widget::component_widget(QWidget * parent):
    QWidget(parent)
{

}

component_widget::~component_widget()
{

}

void component_widget::set_entity(nsentity * ent_)
{
    m_edit_ent = ent_;
}

nsentity * component_widget::entity()
{
    return m_edit_ent;
}

void component_widget::init(Preview * prev_)
{
    m_prev = prev_;
}
