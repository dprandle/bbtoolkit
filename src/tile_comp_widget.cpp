#include <tile_comp_widget.h>
#include <ui_tile_comp_widget.h>

tile_comp_widget::tile_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::tile_comp_widget)
{
    ui->setupUi(this);
}

tile_comp_widget::~tile_comp_widget()
{
    delete ui;
}
