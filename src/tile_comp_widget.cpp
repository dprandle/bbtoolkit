#include <nsengine.h>
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

uint32 tile_comp_widget::type()
{
    return hash_id("nstile_comp");
}
