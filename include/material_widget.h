#ifndef MATERIAL_WIDGET_H
#define MATERIAL_WIDGET_H

#include <component_widget.h>

namespace Ui {
class material_widget;
}

class material_widget : public component_widget
{
    Q_OBJECT

public:
    explicit material_widget(QWidget *parent = 0);
    ~material_widget();

    Ui::material_widget *ui;
};

#endif // MATERIAL_WIDGET_H
