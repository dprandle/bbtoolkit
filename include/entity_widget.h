#ifndef ENTITY_WIDGET_H
#define ENTITY_WIDGET_H


#include <nsmap.h>
#include <QWidget>
#include <ui_entity_widget.h>

class nsentity;
class Preview;
class component_widget;

class entity_widget : public QWidget
{
    Q_OBJECT

public:
    explicit entity_widget(QWidget *parent = 0);
    ~entity_widget();

    template<class WidgetType>
    WidgetType * comp_widget(uint32 type_id)
    {
        auto fiter = m_type_map.find(type_id);
        if (fiter == m_type_map.end())
            return nullptr;
        return static_cast<WidgetType*>(comp_widget(type_id));
    }

    component_widget * comp_widget(uint32 type_id);

    void set_comp_widget(uint32 comp_type);

    void set_editing_ent(nsentity * ent_);

    void set_preview(Preview * prev_);

    nsentity * editing_ent();

    Ui::entity_widget *ui;

private:

    std::map<uint32,component_widget*> m_type_map;

    nsentity * m_editing_ent;
    Preview * m_prev;
};

#endif // ENTITY_WIDGET_H
