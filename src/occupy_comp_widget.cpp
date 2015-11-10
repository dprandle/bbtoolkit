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
