#include <materialwidget.h>
#include <nsmaterial.h>


MaterialWidget::MaterialWidget(QWidget * parent):
	ResourceWidget(parent),
	mMat(NULL)
{
	mUI.setupUi(this);
}

MaterialWidget::~MaterialWidget()
{
	
}

void MaterialWidget::setResource(nsresource * res)
{
	mMat = dynamic_cast<nsmaterial*>(res);
}

nsresource * MaterialWidget::resource()
{
	return  mMat;
}

nsstring MaterialWidget::defaultResName()
{
	return "New Material";
}
