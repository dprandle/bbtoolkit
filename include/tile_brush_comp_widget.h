#ifndef TILE_BRUSH_COMP_WIDGET_H
#define TILE_BRUSH_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class tile_brush_comp_widget;
}

class tile_brush_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit tile_brush_comp_widget(QWidget *parent = 0);
    ~tile_brush_comp_widget();

    uint32 type();

private:
    Ui::tile_brush_comp_widget *ui;
};

#endif // TILE_BRUSH_COMP_WIDGET_H
