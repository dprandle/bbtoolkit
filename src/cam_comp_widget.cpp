#include <cam_comp_widget.h>
#include <ui_cam_comp_widget.h>

cam_comp_widget::cam_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::cam_comp_widget)
{
    ui->setupUi(this);
}

cam_comp_widget::~cam_comp_widget()
{
    delete ui;
}
