#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <resourcewidget.h>
#include <ui_materialwidget.h>

class nsmaterial;
class nsresource;

class MaterialWidget : public ResourceWidget
{
	Q_OBJECT
	
  public:
	
    MaterialWidget(QWidget * parent = NULL);
    virtual ~MaterialWidget();

    void setResource(nsresource * res);
    nsresource * resource();
	nsstring defaultResName();
							 	
  private:
	nsmaterial * mMat;
	Ui::MaterialWidget mUI;
};


#endif
