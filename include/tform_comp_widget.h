#ifndef TFORM_COMP_WIDGET_H
#define TFORM_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class tform_comp_widget;
}

class tform_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit tform_comp_widget(QWidget *parent = 0);
    ~tform_comp_widget();

    uint32 type();

private:
    Ui::tform_comp_widget *ui;
};

#endif // TFORM_COMP_WIDGET_H
