#ifndef RESOURCEWIDGET_H
#define RESOURCEWIDGET_H

#include <qwidget.h>
#include <nsglobal.h>

class NSResource;

class ResourceWidget : public QWidget
{
  public:
    ResourceWidget(QWidget * parent = NULL):QWidget(parent){}
    virtual ~ResourceWidget() {};

	virtual void setResource(NSResource * res)=0;
	virtual NSResource * resource()=0;
	virtual nsstring defaultResName()=0;
};


#endif
