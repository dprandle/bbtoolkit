#ifndef ENTITYEDITORDIALOG_H
#define ENTITYEDITORDIALOG_H

// Inlcudes
#include <ui_entityeditordialog.h>
#include <toolkitdef.h>

class nsengine;
class Preview;
class Toolkit;
class nsentity;

class EntityEditorDialog : public QDialog
{
	Q_OBJECT
public:
	enum CompWidgetIndex
	{
		None,
		Animation,
		Camera,
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

	nsentity * entity();

    void init();

	void setEntity(nsentity * pEnt);

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
	nsentity * mEnt;
	QMap<QString, CompWidgetIndex> mTypeToIndex;
};
#endif
