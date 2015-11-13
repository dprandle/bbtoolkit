#ifndef TERRAIN_COMP_WIDGET_H
#define TERRAIN_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class terrain_comp_widget;
}

class terrain_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit terrain_comp_widget(QWidget *parent = 0);
    ~terrain_comp_widget();

    uint32 type();

private:
    Ui::terrain_comp_widget *ui;
};

#endif // TERRAIN_COMP_WIDGET_H
