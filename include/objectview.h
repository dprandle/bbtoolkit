#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

// Inlcudes
#include <ui_objectview.h>
#include <toolkitdef.h>
#include <nsglobal.h>

class Toolkit;
class NSEngine;
class EntityEditorDialog;

class ObjectView : public QMainWindow
{
	Q_OBJECT
public:
	ObjectView(QWidget * parent = NULL);
	~ObjectView();

	void init(Toolkit * pTK, EntityEditorDialog * pEntityEditorDialog);

	void refresh();

	public slots:

	void onActionNew();
	void onActionDelete();
	void onActionEdit();
	void onItemPressed(QListWidgetItem* pItem);

private:
	Ui_ObjectView mUI;
	Toolkit * mTK;
	EntityEditorDialog * mEntD;
};
#endif