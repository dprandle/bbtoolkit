#include <listwidgetcust.h>
#include <qevent.h>
#include <toolkit.h>
#include <newtiledialog.h>

ListWidgetCust::ListWidgetCust(QWidget * pParent) :
QListWidget(pParent)
{
}

void ListWidgetCust::init()
{}

void ListWidgetCust::mousePressEvent(QMouseEvent * pEvent)
{
	if (!indexAt(pEvent->pos()).isValid())
		clearSelection();
	QListWidget::mousePressEvent(pEvent);
    bbtk.map_view()->setFocus();
}
