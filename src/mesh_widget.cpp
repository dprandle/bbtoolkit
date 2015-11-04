#include "include/mesh_widget.h"
#include "ui_mesh_widget.h"

mesh_widget::mesh_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mesh_widget)
{
    ui->setupUi(this);
}

mesh_widget::~mesh_widget()
{
    delete ui;
}
