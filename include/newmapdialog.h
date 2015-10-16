#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

// Inlcudes
#include <ui_newmap.h>

class NSEngine;
class Toolkit;

class NewMapDialog : public QDialog
{
	Q_OBJECT
public:
	NewMapDialog(QWidget * parent = NULL);
	~NewMapDialog();

	void init(Toolkit * pTK);

	public slots:
	void onChooseSkybox();
	void onChooseTile();
	void onNewSkybox();
	void onNewTile();
	void onColorChange();
	void onCreate();
	void onCancel();
	void onCameraCB(bool);
	void onDirLightCB(bool);
	void onChooseCamera();
	void onNewCamera();
	void onChooseDirlight();
	void onNewDirlight();

private:
	void _setColorStyle();

	QColor prevC;

	Ui_NewMapDialog mNewMapUI;
	Toolkit * mTK;
};
#endif