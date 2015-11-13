#ifndef SEL_COMP_WIDGET_H
#define SEL_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class sel_comp_widget;
}

class sel_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit sel_comp_widget(QWidget *parent = 0);
    ~sel_comp_widget();

    uint32 type();

private:
    Ui::sel_comp_widget *ui;
};

#endif // SEL_COMP_WIDGET_H
