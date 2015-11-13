#ifndef COMPONENT_WIDGET_H
#define COMPONENT_WIDGET_H

#include <nstypes.h>
#include <QWidget>

class nsentity;
class Preview;

class component_widget : public QWidget
{
public:
    component_widget(QWidget * parent = NULL);
    virtual ~component_widget();

    virtual uint32 type()=0;

    void init(Preview * prev_);

    virtual void set_entity(nsentity * ent_);

    nsentity * entity();

protected:

    nsentity * m_edit_ent;

    Preview * m_prev;

};

#endif // COMPONENT_WIDGET_H
