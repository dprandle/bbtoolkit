#include <render_comp_widget.h>
#include <ui_render_comp_widget.h>

render_comp_widget::render_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::render_comp_widget)
{
    ui->setupUi(this);
}

render_comp_widget::~render_comp_widget()
{
    delete ui;
}
