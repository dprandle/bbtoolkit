/*!
\file toolkit.h

\brief Header file for Toolkit class

This file contains all of the neccessary declarations for the Toolkit class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/

#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <QMainWindow>
#include <ui_toolkit.h>
#include <qdir.h>
#include <qlist.h>

#define LAYER_ABOVE_TEXT "Above"
#define LAYER_BELOW_TEXT "Below"
#define LAYER_ALL_TEXT "All Except"
#define LAYER_ONLY_TEXT "Only"

class QMessageBox;
class MapView;
class NSEngine;
class OutputView;
class QSpinBox;
class QCheckBox;
class QLabel;
class QMenu;
class QIcon;
class BrushMenuWidget;
class CameraSettingsDialog;
class TileView;
class ObjectView;
class EntityEditorDialog;
class ResourceBrowser;

class Toolkit : public QMainWindow
{
	Q_OBJECT

public:
	Toolkit(QWidget *parent = 0);
	~Toolkit();

	void closeEvent(QCloseEvent *pEvent);

	MapView * mapView();

	OutputView * outputView();

	ObjectView * objectView();

	TileView *  tileView();

	CameraSettingsDialog * camSettings();

	void loadPluginFiles(const QDir & startingDir);

	void refreshViews();

	void init();

	void updateUI();

public slots:
	void onSelect(bool);
	void onSelectArea(bool);
	void onTileBrush(bool);
	void onObjectBrush(bool);
	void onEraserBrush(bool);
	void onNew();
	void onLoad();
	void onSave();
	void onUnlock(bool);
	void onReplaceTile();
	void onDelSelection();
	void onNewBrush();
	void onSwitchMaps();
	void onLayerIndexChange(const QString &);
	void onLayerMode(bool);
	void onStampMode(bool);
	void onHideSelection();
	void onUnhideSelection();
	void onCamCenter(bool);
	void onCamFront(bool);
	void onObjectCenter(bool);
	void onCamIso(bool);
	void onCamTop(bool);
	void onRedo();
	void onUndo();
	void onSetCurrentBrush(NSEntity *);
	void onBrushChange(QListWidgetItem*);
	void onBrushDoubleClick();
	void onBrushHeightChange(int);
	void onMirrorMode(bool);
	void onChangeLayer(int);
	void onCameraSettings();
	void onShowAllHidden(bool);
	void onMirrorCenterChange(int);
	void onDebugView(bool);
	void onToggleLighting(bool);
	void onToggleEarlyZ(bool);
	void onToggleXFB(bool);
	void onResourceBrowser();

private:

	void _disableSideTBActions();

	Ui_Toolkit mUI;

	QSpinBox * mBrushHeight;
	QSpinBox * mCurrentLayer;
	QSpinBox * mGridX;
	QSpinBox * mGridY;
	QComboBox * mLayerCB;
	QMenu * mBrushMenu;
	BrushMenuWidget * mBrushMenuWidget;
	CameraSettingsDialog * mCamSettings;
	ResourceBrowser * mResourceBrowser;
	EntityEditorDialog * mEntD;
	nsint mLayersAboveHidden;
	nsint spinBoxVal;
	QString mPrevLayerText;
};

#endif // TOOLKIT_H


