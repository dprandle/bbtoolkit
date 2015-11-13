#include <nsengine.h>
#include <include/tform_comp_widget.h>
#include <ui_tform_comp_widget.h>

tform_comp_widget::tform_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::tform_comp_widget)
{
    ui->setupUi(this);
}

tform_comp_widget::~tform_comp_widget()
{
    delete ui;
}

uint32 tform_comp_widget::type()
{
    return hash_id("nstform_comp");
}
