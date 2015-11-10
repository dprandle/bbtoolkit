#ifndef LIGHT_COMP_WIDGET_H
#define LIGHT_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class light_comp_widget;
}

class light_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit light_comp_widget(QWidget *parent = 0);
    ~light_comp_widget();

private:
    Ui::light_comp_widget *ui;
};

#endif // LIGHT_COMP_WIDGET_H
