#ifndef COMPONENT_SELECTION_WIDGET_H
#define COMPONENT_SELECTION_WIDGET_H

#include <QWidget>
#include <nstypes.h>
#include <nsstring.h>
#include <map>

namespace Ui {
class component_selection_widget;
}

class nsentity;
class Preview;

class component_selection_widget : public QWidget
{
    Q_OBJECT

public:
    explicit component_selection_widget(QWidget *parent = 0);
    ~component_selection_widget();

    void init(Preview * prev_);

    void set_entity(nsentity * ent);

    void refresh();

public slots:

    void on_tb_add_comp_pressed();

    void on_tb_del_comp_pressed();

    void on_tb_edit_comp_pressed();

signals:
    void edit_component(uint32 id);

private:

    std::map<uint32, nsstring> m_icons;

    nsentity * m_edit_ent;

    Ui::component_selection_widget *m_ui;

    Preview * m_prev;

};

#endif // COMPONENT_SELECTIONI_WIDGET_H
