#ifndef NEWTILEDIALOG_H
#define NEWTILEDIALOG_H

// Inlcudes
#include <ui_newtile.h>
#include <toolkitdef.h>

class nsengine;
class Preview;
class Toolkit;
class nsmaterial;

class NewTileDialog : public QDialog
{
	Q_OBJECT
public:
	NewTileDialog(QWidget * parent = NULL);
	~NewTileDialog();

    void init();

	QString getEntityName();
	void reject();

	public slots:
	void onAdd();
	void onCancel();
	void onBrowseNormal();
	void onBrowseDiffuse();
	void onBrowseIcon();
	void onGenerateIcon();
	void onColorChange();
	void onDiffuseSldrChange(int pNewVal);
	void onAmbientSldrChange(int pNewVal);
	void onPreviewDefaults();
	void onNormLEEdit(QString pNewText);
	void onDiffLEEdit(QString pNewText);
	void onIconLEEdit(QString pNewText);
	void onTileSizeChange(QString pText);
	void onMatColorChange();
	void onSpecColorChange();
	void onShininessChange(int);
	void onNameEdit();
	void onNewFilter();

private:
	Ui_NewTileDialog mUI;
	nsmaterial * mTempMat;
	QString mGenFile;

	bool diffuseLoaded;
	bool normalLoaded;
};
#endif
