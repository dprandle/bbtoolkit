#ifndef RENDER_COMP_WIDGET_H
#define RENDER_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class render_comp_widget;
}

class render_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit render_comp_widget(QWidget *parent = 0);
    ~render_comp_widget();

private:
    Ui::render_comp_widget *ui;
};

#endif // RENDER_COMP_WIDGET_H
