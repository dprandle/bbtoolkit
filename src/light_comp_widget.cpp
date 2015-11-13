#include <nsengine.h>
#include <light_comp_widget.h>
#include <ui_light_comp_widget.h>

light_comp_widget::light_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::light_comp_widget)
{
    ui->setupUi(this);
}

light_comp_widget::~light_comp_widget()
{
    delete ui;
}

uint32 light_comp_widget::type()
{
    return hash_id("nslight_comp");
}
