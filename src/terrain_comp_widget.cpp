#include <terrain_comp_widget.h>
#include <ui_terrain_comp_widget.h>

terrain_comp_widget::terrain_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::terrain_comp_widget)
{
    ui->setupUi(this);
}

terrain_comp_widget::~terrain_comp_widget()
{
    delete ui;
}
