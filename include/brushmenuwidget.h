#ifndef BRUSHMENUWIDGET_H
#define BRUSHMENUWIDGET_H

// Inlcudes
#include <ui_brushmenuwidget.h>
#include <qwidget.h>
class nsengine;
class Toolkit;
class QToolBar;
class nsentity;

class BrushMenuWidget : public QWidget
{
	
	Q_OBJECT

public:

	BrushMenuWidget(QWidget * parent = NULL);
	~BrushMenuWidget();

	nsentity * selectedItem();

    void init();
	void setupLW();
	void setSelectedItem(nsentity * ent);

public slots:
	void onDeleteBrush();
	void onEditBrush();
	void onDoubleClick();
	void onNewBrush();
    void onSelectionChanged();

signals:
	void brushChange(QListWidgetItem*);
	void brushDoubleClick();

private:
    QToolBar * mTB;
	Ui_BrushMenuWidget mUI;
};
#endif
