#ifndef PARTICLE_COMP_WIDGET_H
#define PARTICLE_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class particle_comp_widget;
}

class particle_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit particle_comp_widget(QWidget *parent = 0);
    ~particle_comp_widget();

    uint32 type();

private:
    Ui::particle_comp_widget *ui;
};

#endif // PARTICLE_COMP_WIDGET_H
