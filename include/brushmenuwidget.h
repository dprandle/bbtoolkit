#ifndef BRUSHMENUWIDGET_H
#define BRUSHMENUWIDGET_H

// Inlcudes
#include <ui_brushmenuwidget.h>
#include <qwidget.h>
#include <nsglobal.h>
class NSEngine;
class Toolkit;
class QToolBar;

class BrushMenuWidget : public QWidget
{
	
	Q_OBJECT

public:

	BrushMenuWidget(QWidget * parent = NULL);
	~BrushMenuWidget();

	NSEntity * selectedItem();

	void init(Toolkit * pTK);
	void setupLW();
	void setSelectedItem(NSEntity * ent);

	public slots:
	void onDeleteBrush();
	void onEditBrush();
	void onDoubleClick();
	void onNewBrush();
	void onItemChanged(QListWidgetItem*);

signals:
	void brushChange(QListWidgetItem*);
	void brushDoubleClick();

private:
	Ui_BrushMenuWidget mUI;
	Toolkit * mTK;
	QToolBar * mTB;
};
#endif
