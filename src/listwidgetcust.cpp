#include <listwidgetcust.h>
#include <qevent.h>
#include <toolkit.h>
#include <newtiledialog.h>

ListWidgetCust::ListWidgetCust(QWidget * pParent) :
QListWidget(pParent)
{
}

void ListWidgetCust::init(Toolkit * pTK)
{
	mTK = pTK;
}

void ListWidgetCust::mousePressEvent(QMouseEvent * pEvent)
{
	if (!indexAt(pEvent->pos()).isValid())
		clearSelection();
	QListWidget::mousePressEvent(pEvent);
	mTK->mapView()->setFocus();
}