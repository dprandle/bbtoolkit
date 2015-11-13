#include <nsengine.h>
#include <sel_comp_widget.h>
#include <ui_sel_comp_widget.h>

sel_comp_widget::sel_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::sel_comp_widget)
{
    ui->setupUi(this);
}

sel_comp_widget::~sel_comp_widget()
{
    delete ui;
}

uint32 sel_comp_widget::type()
{
    return hash_id("nssel_comp");
}
