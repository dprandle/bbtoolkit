#include <material_widget.h>
#include <ui_material_widget.h>

material_widget::material_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::material_widget)
{
    ui->setupUi(this);
}

material_widget::~material_widget()
{
    delete ui;
}
