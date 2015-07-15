#ifndef ENTITYEDITORDIALOG_H
#define ENTITYEDITORDIALOG_H

// Inlcudes
#include <ui_entityeditordialog.h>
#include <toolkitdef.h>
#include <nsglobal.h>

class NSEngine;
class Preview;
class Toolkit;
class NSEntity;

class EntityEditorDialog : public QDialog
{
	Q_OBJECT
public:
	enum CompWidgetIndex
	{
		None,
		Animation,
		Camera,
		Input,
		Light, 
		Occupy,
		Particle,
		Render,
		Selection,
		Transform,
		Tilebrush,
		Tile
	};

	EntityEditorDialog(QWidget * parent = NULL);
	~EntityEditorDialog();

	void clear();

	NSEntity * entity();

	void init(Toolkit * pTK);

	void setEntity(NSEntity * pEnt);

	public slots:

	void onRemoveComp();
	void onPrevDefault();
	void onOkay();
	void onNameChange();
	void onGenerateIcon();
	void onDiffuseChange(int);
	void onCompItemPressed(QListWidgetItem *);
	void onColorChange();
	void onCancel();
	void onAmbientChange(int);
	void onAddComp();
	void onChooseIcon();
	void onIdle();

private:
	Ui::EntityEditorDialog mUI;
	Toolkit * mTK;
	NSEntity * mEnt;
	QMap<QString, CompWidgetIndex> mTypeToIndex;
};
#endif