#ifndef MESH_WIDGET_H
#define MESH_WIDGET_H

#include <component_widget.h>

namespace Ui {
class mesh_widget;
}

class mesh_widget : public component_widget
{
    Q_OBJECT

public:
    explicit mesh_widget(QWidget *parent = 0);
    ~mesh_widget();

    Ui::mesh_widget *ui;
};

#endif // MESH_WIDGET_H
