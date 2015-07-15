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

void MaterialWidget::setResource(NSResource * res)
{
	mMat = dynamic_cast<NSMaterial*>(res);
}

NSResource * MaterialWidget::resource()
{
	return  mMat;
}

nsstring MaterialWidget::defaultResName()
{
	return "New Material";
}
