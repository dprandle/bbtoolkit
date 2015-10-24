#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <qwidget.h>
#include <nsstring.h>

class nsresource;

class ResourceWidget : public QWidget
{
  public:
    ResourceWidget(QWidget * parent = NULL):QWidget(parent){}
    virtual ~ResourceWidget() {}

    virtual void setResource(nsresource * res)=0;
    virtual nsresource * resource()=0;
	virtual nsstring defaultResName()=0;
};


#endif
