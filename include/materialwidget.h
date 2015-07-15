#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <resourcewidget.h>
#include <ui_materialwidget.h>

class NSMaterial;

class MaterialWidget : public ResourceWidget
{
	Q_OBJECT
	
  public:
	
    MaterialWidget(QWidget * parent = NULL);
    virtual ~MaterialWidget();

	void setResource(NSResource * res);
	NSResource * resource();
	nsstring defaultResName();
							 	
  private:
	NSMaterial * mMat;
	Ui::MaterialWidget mUI;
};


#endif
