#include <previewwidget.h>

PreviewWidget::PreviewWidget(QWidget * parent):
	QWidget(parent)
{
	mUI.setupUi(this);
}

PreviewWidget::~PreviewWidget()
{
	
}

Ui::PreviewWidget & PreviewWidget::ui()
{
	return mUI;
}
