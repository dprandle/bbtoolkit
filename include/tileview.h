#ifndef TILEVIEW_H
#define TILEVIEW_H

// Inlcudes
#include <ui_tileview.h>
#include <toolkitdef.h>
#include <nsglobal.h>

class NSEngine;
class EntityEditorDialog;

class TileView : public QMainWindow
{
	Q_OBJECT
public:
	TileView(QWidget * parent = NULL);
	~TileView();

	Ui_TileView * getUI();
	void init(Toolkit * pTK, EntityEditorDialog * pEntityDialogEditor);
	void reset();
	void refresh();

public slots:

	void onActionNew();

	void onActionDelete();
	void onActionEdit();
	void onSelectionChanged();

private:
	Ui_TileView mUI;
	EntityEditorDialog * mEntD;
	Toolkit * mTK;
};
#endif