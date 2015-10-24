#include <resource_dialog_prev.h>
#include <texture_widget.h>

resource_dialog_prev::resource_dialog_prev(QWidget * parent):
    QDialog::QDialog(parent),
    m_tex_widget(new texture_widget(parent))
{
    m_ui.setupUi(this);
}

void resource_dialog_prev::init()
{

}

void resource_dialog_prev::set_texture()
{
    m_ui.m_scroll_area->takeWidget();
    m_ui.m_scroll_area->setWidget(m_tex_widget);
}

