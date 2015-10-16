#ifndef MANAGEMAPSDIALOG_H
#define MANAGEMAPSDIALOG_H

// Inlcudes
#include <ui_managemapsdialog.h>
#include <ui_editmapdialog.h>

class Toolkit;
class NewMapDialog;

class ManageMapsDialog : public QDialog
{
	Q_OBJECT
public:
	ManageMapsDialog(QWidget * parent = NULL);
	~ManageMapsDialog();

	void init(Toolkit * pTK);

	public slots:

	/*
	When a item is selected we want to display the maps information in the appropriate fields
	*/
	void onMapItemChanged(QTreeWidgetItem*, int);
	void onMapItemPressed(QTreeWidgetItem*, int);
	void onNewMap();
	void onDeleteMap();
	void onEditMap();
	void onLoad();
	void onCancel();
	void onEditColClick();

private:
	void _rescanPluginMaps();
	Ui::EditMapDialog mEditMapUI;
	Ui_ManageMapsDialog mUI;
	QDialog mEditD;
	Toolkit * mTK;
	NewMapDialog * mNewMapD;
	QColor prevC;
};
#endif