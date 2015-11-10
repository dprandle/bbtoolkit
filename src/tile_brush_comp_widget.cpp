#include <include/tile_brush_comp_widget.h>
#include <ui_tile_brush_comp_widget.h>

tile_brush_comp_widget::tile_brush_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::tile_brush_comp_widget)
{
    ui->setupUi(this);
}

tile_brush_comp_widget::~tile_brush_comp_widget()
{
    delete ui;
}
