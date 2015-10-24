#ifndef TILEVIEW_H
#define TILEVIEW_H

// Inlcudes
#include <ui_tileview.h>
#include <toolkitdef.h>

class nsengine;
class EntityEditorDialog;

class TileView : public QMainWindow
{
	Q_OBJECT
public:
	TileView(QWidget * parent = NULL);
	~TileView();

    Ui_TileView * ui();
    void init();
	void reset();
	void refresh();

public slots:

	void onActionNew();

	void onActionDelete();
	void onActionEdit();
	void onSelectionChanged();

private:
    Ui_TileView m_ui;
};
#endif
