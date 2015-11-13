#ifndef ANIM_COMP_WIDGET_H
#define ANIM_COMP_WIDGET_H


#include <component_widget.h>

namespace Ui {
class anim_comp_widget;
}


class anim_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit anim_comp_widget(QWidget *parent = 0);
    ~anim_comp_widget();

    uint32 type();

private:
    Ui::anim_comp_widget *ui;
};

#endif // ANIM_COMP_WIDGET_H
