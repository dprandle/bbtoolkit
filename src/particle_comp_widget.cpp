#include <particle_comp_widget.h>
#include <ui_particle_comp_widget.h>

particle_comp_widget::particle_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::particle_comp_widget)
{
    ui->setupUi(this);
}

particle_comp_widget::~particle_comp_widget()
{
    delete ui;
}
