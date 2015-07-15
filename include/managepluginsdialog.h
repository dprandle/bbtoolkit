#ifndef MANAGEPLUGINSDIALOG_H
#define MANAGEPLUGINSDIALOG_H

// Inlcudes
#include <ui_managepluginsdialog.h>
#include <ui_newplugindialog.h>
#include <qvector.h>
#include <set>
#include <nsmath.h>
#include <nsglobal.h>
#include <qcolor.h>

class NSEngine;
class Toolkit;
class NSPlugin;
class NSPluginManager;

void AddParents(NSPlugin * pCurPlug, QVector<NSPlugin*> & pPlugsToLoad, std::set<nsstring> & pUnloadedPlugs, NSPluginManager * pPlugs);

class ManagePluginsDialog : public QDialog
{
	Q_OBJECT

public:
	ManagePluginsDialog(QWidget * parent = NULL);
	~ManagePluginsDialog();

	void init(Toolkit * pTK);
	void addPluginsToTreeWidget();

	/*! 
	Recursive function which will check all parents of the plugin before checking the plugin
	*/
	void check(QTreeWidgetItem * pItem);

	/*!
	Recursive function which will uncheck all children of the plugin before unchecking the plugin
	*/
	void uncheck(QTreeWidgetItem * pItem);

	public slots:

	void onNewPlugin();
	void onDeletePlugin();
	void onSelectionChange();
	void onSetActive();
	void onDetails();
	void onEditPlugin();
	void onNewPluginAccept();
	void onNewPluginReject();
	void onOkay();
	void onCancel();
	void onItemChanged(QTreeWidgetItem*,int);
	void onColorChange();

private:
	nsbool _itemHasCheckedChildren(QTreeWidgetItem* item);
	void _setColorStyle();

	Ui_ManagePluginsDialog mUI;
	Toolkit * mTK;
	QDialog * mNewPlugDialog;
	Ui_NewPluginDialog mNewPlugUI;
	Qt::CheckState mPressedState;
	QString mInitialActive;
	QColor prevC;
};
#endif