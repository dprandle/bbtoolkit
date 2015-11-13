#ifndef CAM_COMP_WIDGET_H
#define CAM_COMP_WIDGET_H

#include <component_widget.h>

namespace Ui {
class cam_comp_widget;
}

class cam_comp_widget : public component_widget
{
    Q_OBJECT

public:
    explicit cam_comp_widget(QWidget *parent = 0);
    ~cam_comp_widget();

    uint32 type();

private:
    Ui::cam_comp_widget *ui;
};

#endif // CAM_COMP_WIDGET_H
