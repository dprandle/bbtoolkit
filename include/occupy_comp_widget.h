#ifndef OCCUPY_COMP_WIDGET_H
#define OCCUPY_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class occupy_comp_widget;
}

class occupy_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit occupy_comp_widget(QWidget *parent = 0);
    ~occupy_comp_widget();

private:
    Ui::occupy_comp_widget *ui;
};

#endif // OCCUPY_COMP_WIDGET_H
