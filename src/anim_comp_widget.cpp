#include <nsengine.h>

#include <anim_comp_widget.h>
#include <ui_anim_comp_widget.h>

anim_comp_widget::anim_comp_widget(QWidget *parent) :
    component_widget(parent),
    ui(new Ui::anim_comp_widget)
{
    ui->setupUi(this);
}

anim_comp_widget::~anim_comp_widget()
{
    delete ui;
}

uint32 anim_comp_widget::type()
{
    return hash_id("nsanim_comp");
}

