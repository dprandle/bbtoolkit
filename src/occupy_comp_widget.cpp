#include <nsengine.h>
#include <occupy_comp_widget.h>
#include <ui_occupy_comp_widget.h>

occupy_comp_widget::occupy_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::occupy_comp_widget)
{
    ui->setupUi(this);
}

occupy_comp_widget::~occupy_comp_widget()
{
    delete ui;
}

uint32 occupy_comp_widget::type()
{
    return hash_id("nsoccupy_comp");
}
