#ifndef MESH_WIDGET_H
#define MESH_WIDGET_H

#include <QWidget>

namespace Ui {
class mesh_widget;
}

class mesh_widget : public QWidget
{
    Q_OBJECT

public:
    explicit mesh_widget(QWidget *parent = 0);
    ~mesh_widget();

    Ui::mesh_widget *ui;
};

#endif // MESH_WIDGET_H
