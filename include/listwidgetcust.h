#include <qlistwidget.h>

#ifndef CUSTOMLIST_H
#define CUSTOMLIST_H
class Toolkit;
class NSEngine;

class ListWidgetCust : public QListWidget
{
	Q_OBJECT
public:
	ListWidgetCust(QWidget * pParent = NULL);

	void init(Toolkit * pTK);

protected:

	void mousePressEvent(QMouseEvent * pEvent);

private:
	Toolkit * mTK;
};
#endif