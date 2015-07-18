/*!
\file toolkit.cpp

\brief Definition file for Toolkit class

This file contains all of the neccessary definitions for the Toolkit class.

\author Daniel Randle
\date December 11 2013
\copywrite Earth Banana Games 2013
*/
#include <toolkitdef.h>
#include <nsinputsystem.h>
#include <toolkit.h>
#include <mapview.h>
#include <outputview.h>
#include <qdockwidget.h>
#include <qmessagebox.h>
#include <nsengine.h>
#include <nsdebug.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qdir.h>
#include <nsinputmanager.h>
#include <nsglobal.h>
#include <addnewbrushdialog.h>
#include <qmenu.h>
#include <nsshadermanager.h>
#include <nsbuildsystem.h>
#include <nstilebrushcomp.h>
#include <managemapsdialog.h>
#include <qwidgetaction.h>
#include <managepluginsdialog.h>
#include <nsentitymanager.h>
#include <nsscenemanager.h>
#include <brushmenuwidget.h>
#include <nsselectionsystem.h>
#include <nspluginmanager.h>
#include <nstilegrid.h>
#include <nscamerasystem.h>
#include <qtimer.h>
#include <camerasettingsdialog.h>
#include <nsparticlecomp.h>
#include <qprogressdialog.h>
#include <nstexmanager.h>
#include <nsmatmanager.h>
#include <nsmaterial.h>
#include <nstexture.h>
#include <nsrendersystem.h>
#include <entityeditordialog.h>
#include <nsmath.h>
#include <resourcebrowser.h>

Toolkit::Toolkit(QWidget *parent) :
QMainWindow(parent),
mBrushHeight(new QSpinBox()),
mCurrentLayer(new QSpinBox()),
mLayerCB(new QComboBox()),
mBrushMenu(new QMenu(this)),
mBrushMenuWidget(new BrushMenuWidget(this)),
mCamSettings(new CameraSettingsDialog(this)),
mResourceBrowser(new ResourceBrowser()),
mGridX(new QSpinBox()),
mGridY(new QSpinBox()),
mEntD(new EntityEditorDialog(this)),
mLayersAboveHidden(0),
spinBoxVal(0),
mPrevLayerText(LAYER_ABOVE_TEXT)
{
	mUI.setupUi(this);
	setWindowIcon(QIcon(":/Toolkit/bbicon_256px.ico"));
}

MapView * Toolkit::mapView()
{
	return mUI.mMapView;
}

OutputView * Toolkit::outputView()
{
	return mUI.mOutputView;
}

void Toolkit::loadPluginFiles(const QDir & startingDir)
{
	if (startingDir.exists())
	{
		foreach(QFileInfo info, startingDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs))
		{
			if (info.isFile())
				nsengine.loadPlugin(info.absoluteFilePath().toStdString(), false);
			if (info.isDir())
				loadPluginFiles(info.dir());
		}
	}
}

void Toolkit::init()
{
	mUI.mMapView->init(this);

	mEntD->init(this);
	mEntD->setModal(false);
	mUI.mOutputView->init(this);
	mUI.mTileView->init(this, mEntD);
	mUI.mObjectView->init(this, mEntD);
	mBrushMenuWidget->init(this);
	mResourceBrowser->init(this);
	
	QWidgetAction * wA = new QWidgetAction(this);
	wA->setDefaultWidget(mBrushMenuWidget);
	mBrushMenu->addAction(wA);
	mBrushHeight->setRange(1, 16);
	mLayerCB->addItem(LAYER_ABOVE_TEXT);
	mLayerCB->addItem(LAYER_BELOW_TEXT);
	mLayerCB->addItem(LAYER_ALL_TEXT);
	mLayerCB->addItem(LAYER_ONLY_TEXT);


	QAction * act = mBrushMenu->menuAction();

	mGridX->setRange(-256, 256);
	mGridY->setRange(-256, 256);
	mGridX->setValue(0);
	mGridY->setValue(0);
	
	mUI.mTopToolbar->insertWidget(mUI.actionUnlock, new QLabel("Brush "));
	mUI.mTopToolbar->insertAction(mUI.actionUnlock, act);
	mUI.mTopToolbar->insertWidget(mUI.actionUnlock, new QLabel("Height "));
	mUI.mTopToolbar->insertWidget(mUI.actionUnlock, mBrushHeight);
	mUI.mTopToolbar->insertWidget(mUI.actionLayerMode, new QLabel("Hide "));
	mUI.mTopToolbar->insertWidget(mUI.actionLayerMode, mLayerCB);
	mUI.mTopToolbar->insertWidget(mUI.actionLayerMode, new QLabel("Layer "));
	mUI.mTopToolbar->insertWidget(mUI.actionLayerMode, mCurrentLayer);
	mUI.mTopToolbar->insertWidget(mUI.actionMirrorMode, new QLabel("Mirror Center: "));
	mUI.mTopToolbar->insertWidget(mUI.actionMirrorMode, new QLabel("x"));
	mUI.mTopToolbar->insertWidget(mUI.actionMirrorMode, mGridX);
	mUI.mTopToolbar->insertWidget(mUI.actionMirrorMode, new QLabel("y"));
	mUI.mTopToolbar->insertWidget(mUI.actionMirrorMode, mGridY);

	QAction * deb = new QAction(QIcon(":/VecTB/Resources/Textures/Icons/Toolbars/Debug_Icon.png"), "Debug View", this);
	QAction * ez = new QAction(QIcon(":/VecTB/Resources/Textures/Icons/Toolbars/EarlyZ.png"), "Toggle EarlyZ", this);
	deb->setCheckable(true);
	ez->setCheckable(true);

	mUI.mTopToolbar->addAction(deb);
	mUI.mTopToolbar->addAction(ez);

	connect(deb, SIGNAL(toggled(bool)), this, SLOT(onDebugView(bool)));
	connect(ez, SIGNAL(toggled(bool)), this, SLOT(onToggleEarlyZ(bool)));

	

	mCurrentLayer->setRange(-256, 256);
	
	connect(mBrushMenuWidget, SIGNAL(brushChange(QListWidgetItem*)), this, SLOT(onBrushChange(QListWidgetItem*)));
	connect(mBrushMenuWidget, SIGNAL(brushDoubleClick()), this, SLOT(onBrushDoubleClick()));
	connect(mBrushHeight, SIGNAL(valueChanged(int)), this, SLOT(onBrushHeightChange(int)));
	connect(mCurrentLayer, SIGNAL(valueChanged(int)), this, SLOT(onChangeLayer(int)));
	connect(mLayerCB, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onLayerIndexChange(const QString &)));
	connect(mGridX, SIGNAL(valueChanged(int)), this, SLOT(onMirrorCenterChange(int)));
	connect(mGridY, SIGNAL(valueChanged(int)), this, SLOT(onMirrorCenterChange(int)));

	setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
	show();
	onLoad();
}

void Toolkit::onCameraSettings()
{
	mCamSettings->showIt();
}

Toolkit::~Toolkit()
{
}

CameraSettingsDialog * Toolkit::camSettings()
{
	return mCamSettings;
}

void Toolkit::closeEvent(QCloseEvent *pEvent)
{
	NSPlugin * activePlug = nsengine.active();

	if (activePlug != NULL)
	{
		QMessageBox mb(QMessageBox::Warning, "Unsaved Changed", "There are unsaved changes. Would you like to save before exiting?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
		int ret = mb.exec();
		if (ret == QMessageBox::Cancel)
		{
			pEvent->ignore();
			return;
		}
		bool saveChanges = (ret == QMessageBox::Yes);
		if (saveChanges)
			onSave();
		pEvent->accept();
	}
}

/*!
Update call which happens every frame
If there is a slow down be sure to comment this out and check if the slowdown still occurs - I have found that this function
is very sensative and things like posting to the status bar here will significantly slow program operations
*/
void Toolkit::updateUI()
{
	bool chk = mUI.actionEraserBrush->isChecked() || mUI.actionObjectBrush->isChecked() || mUI.actionTileBrush->isChecked();
	mUI.actionNewBrush->setEnabled(nsengine.system<NSSelectionSystem>()->brushValid() && !chk);
	mUI.actionSwapTiles->setEnabled(nsengine.system<NSSelectionSystem>()->tileSwapValid() && !chk && !mUI.mTileView->getUI()->mListWidget->selectedItems().isEmpty());
	mUI.actionDeleteSelection->setEnabled(!nsengine.system<NSSelectionSystem>()->empty() && !chk);

	mCurrentLayer->setEnabled(mUI.actionLayerMode->isChecked() || mUI.actionEraserBrush->isChecked() || mUI.actionObjectBrush->isChecked() || mUI.actionTileBrush->isChecked());

	NSScene * curScene = nsengine.currentScene();
	NSPlugin * activePlug = nsengine.active();
	QString winTitle = "Build and Battle Toolkit [Active Plugin: ";

	if (activePlug != NULL)
	{
		winTitle += QString(activePlug->name().c_str());
	}
	
	winTitle += "] [Scene: ";

	mUI.mCamToolbar->setEnabled(curScene != NULL);
	if (curScene != NULL)
	{
		winTitle += QString(curScene->name().c_str()) + ".map";

		uivec3 selObj = nsengine.system<NSSelectionSystem>()->center();
		NSEntity * ent = curScene->entity(selObj.xy());

		if (ent != NULL && (!mUI.actionLayerMode->isChecked()||nsengine.system<NSBuildSystem>()->enabled()))
		{
			NSTFormComp * tComp = ent->get<NSTFormComp>();
			if (tComp != NULL)
			{
				nsint layer = NSTileGrid::grid(tComp->wpos(selObj.z)).z;
				nsint tmpVal = spinBoxVal;
				mCurrentLayer->blockSignals(true);
				mCurrentLayer->setValue(layer*-1);
				mCurrentLayer->blockSignals(false);
				spinBoxVal = layer*-1;
				if (mUI.actionLayerMode->isChecked() && tmpVal != spinBoxVal)
					onLayerMode(true);
			}
		}
		nsengine.system<NSBuildSystem>()->setLayer(spinBoxVal*-1);

		if (ent != NULL)
		{
			NSTFormComp * tComp = ent->get<NSTFormComp>();
			mUI.actionEnableXFB->setEnabled(true);
			mUI.actionEnableXFB->blockSignals(true);
			mUI.actionEnableXFB->setChecked(tComp->transformFeedback());
			mUI.actionEnableXFB->blockSignals(false);
		}
		else
		{
			mUI.actionEnableXFB->blockSignals(true);
			mUI.actionEnableXFB->setChecked(false);
			mUI.actionEnableXFB->blockSignals(false);
			mUI.actionEnableXFB->setEnabled(false);
		}
		// Show selected center in taskbar
	}

	winTitle += "]";
	if (winTitle != windowTitle())
		setWindowTitle(winTitle);
}

void Toolkit::_disableSideTBActions()
{
	mUI.actionSelect->setChecked(false);
	mUI.actionSelectArea->setChecked(false);
	mUI.actionTileBrush->setChecked(false);
	mUI.actionObjectBrush->setChecked(false);
	mUI.actionEraserBrush->setChecked(false);
}

void Toolkit::onSelect(bool pVal)
{
	if (pVal)
	{
		_disableSideTBActions();
		nsengine.system<NSBuildSystem>()->enable(false, fvec2());
	}
	mUI.actionSelect->setChecked(true);
	mUI.mMapView->setFocus();
}

void Toolkit::onSelectArea(bool pVal)
{
	if (pVal)
	{
		_disableSideTBActions();
		nsengine.system<NSBuildSystem>()->enable(false, fvec2());
		
	}
	mUI.actionSelectArea->setChecked(true);
	mUI.mMapView->setFocus();
}

void Toolkit::onToggleLighting(bool pChange)
{
	nsengine.system<NSRenderSystem>()->enableLighting(pChange);
}

void Toolkit::onToggleEarlyZ(bool pEnable)
{
	nsengine.system<NSRenderSystem>()->enableEarlyZ(pEnable);
}

void Toolkit::onTileBrush(bool pVal)
{
	if (pVal)
	{
		_disableSideTBActions();

		nsengine.system<NSBuildSystem>()->setBrushMode(NSBuildSystem::Tile);
		nsengine.system<NSBuildSystem>()->setMode(NSBuildSystem::Build);
		nsengine.system<NSBuildSystem>()->setActiveBrushColor(fvec4(0.0f, 1.0f, 0.0f, 0.8f));
		
		auto items = mUI.mTileView->getUI()->mListWidget->selectedItems();
		if (items.isEmpty())
			nsengine.system<NSBuildSystem>()->setBuildEnt(NULL);
		else
		{
			uivec2 id(items.first()->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), items.first()->data(VIEW_WIDGET_ITEM_ENT).toUInt());
			nsengine.system<NSBuildSystem>()->setBuildEnt(nsengine.resource<NSEntity>(id));
		}

		QPoint p = mUI.mMapView->mapFromGlobal(QCursor::pos());
		nsfloat normXPos = nsfloat(p.x()) / nsfloat(mUI.mMapView->width());
		nsfloat normYPos = 1.0f - nsfloat(p.y()) / nsfloat(mUI.mMapView->height());
		nsengine.system<NSBuildSystem>()->enable(true, fvec2(normXPos, normYPos));
	}
	mUI.actionTileBrush->setChecked(true);
	mUI.mMapView->setFocus();
}

void Toolkit::onChangeLayer(int pVal)
{
	bool toggle = mUI.actionLayerMode->isChecked();

	if (toggle)
		onLayerMode(false);

	nsint delta = pVal - spinBoxVal;
	spinBoxVal = pVal;

	if (nsengine.system<NSBuildSystem>()->enabled())
		nsengine.system<NSSelectionSystem>()->changeLayer(delta);

	mUI.mMapView->setFocus();

	if (toggle)
		onLayerMode(toggle);
}

void Toolkit::onObjectBrush(bool pVal)
{
	if (pVal)
	{
		_disableSideTBActions();

		nsengine.system<NSBuildSystem>()->setBrushMode(NSBuildSystem::Object);
		nsengine.system<NSBuildSystem>()->setMode(NSBuildSystem::Build);
		nsengine.system<NSBuildSystem>()->setActiveBrushColor(fvec4(0.0f, 1.0f, 0.0f, 0.8f));

		QPoint p = mUI.mMapView->mapFromGlobal(QCursor::pos());
		nsfloat normXPos = nsfloat(p.x()) / nsfloat(mUI.mMapView->width());
		nsfloat normYPos = 1.0f - nsfloat(p.y()) / nsfloat(mUI.mMapView->height());

		auto items = mUI.mTileView->getUI()->mListWidget->selectedItems();
		if (items.isEmpty())
			nsengine.system<NSBuildSystem>()->setBuildEnt(NULL);
		else
		{
			uivec2 id(items.first()->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), items.first()->data(VIEW_WIDGET_ITEM_ENT).toUInt());
			nsengine.system<NSBuildSystem>()->setBuildEnt(nsengine.resource<NSEntity>(id));
		}

		nsengine.system<NSBuildSystem>()->enable(true, fvec2(normXPos, normYPos));
	}
	mUI.actionObjectBrush->setChecked(true);
	mUI.mMapView->setFocus();
}

void Toolkit::onEraserBrush(bool pVal)
{
	if (pVal)
	{
		_disableSideTBActions();

		nsengine.system<NSBuildSystem>()->setBrushMode(NSBuildSystem::Tile);
		nsengine.system<NSBuildSystem>()->setMode(NSBuildSystem::Erase);
		nsengine.system<NSBuildSystem>()->setActiveBrushColor(fvec4(1.0f, 0.0f, 0.0f, 0.8f));
		nsengine.system<NSBuildSystem>()->setBuildEnt(NULL);

		QPoint p = mUI.mMapView->mapFromGlobal(QCursor::pos());
		nsfloat normXPos = nsfloat(p.x()) / nsfloat(mUI.mMapView->width());
		nsfloat normYPos = 1.0f - nsfloat(p.y()) / nsfloat(mUI.mMapView->height());

		nsengine.system<NSBuildSystem>()->enable(true, fvec2(normXPos, normYPos));
	}
	mUI.actionEraserBrush->setChecked(true);
	mUI.mMapView->setFocus();
}

void Toolkit::onMirrorMode(bool pEnable)
{
	NSBuildSystem * bsys = nsengine.system<NSBuildSystem>();
	bool enabled = bsys->enabled();

	if (enabled)
		bsys->toggle(fvec2());

	nsengine.system<NSBuildSystem>()->enableMirror(pEnable);

	if (enabled)
		bsys->toggle(fvec2());

	mUI.mMapView->setFocus();
}

void Toolkit::onMirrorCenterChange(int pNewVal)
{
	ivec3 grid(mGridX->value(), mGridY->value());
	fvec3 pos = NSTileGrid::world(grid);

	bool tmp = nsengine.system<NSBuildSystem>()->enabled();
	if (tmp)
		nsengine.system<NSBuildSystem>()->toggle(fvec2());

	nsengine.system<NSBuildSystem>()->setCenter(pos);

	if (tmp)
		nsengine.system<NSBuildSystem>()->toggle(fvec2());

	mUI.mMapView->setFocus();
}

void Toolkit::onNew()
{
	mUI.mMapView->setFocus();
}

void Toolkit::onLoad()
{
	if (!mUI.actionSelect->isChecked())
		onSelect(true);

	ManagePluginsDialog mPlugs(this);
	mPlugs.init(this);

	if (mPlugs.exec() == QDialog::Accepted)
	{

	}
	mUI.mMapView->setFocus();
}

void Toolkit::onBrushHeightChange(int pHeight)
{
	NSEntity * ent = mBrushMenuWidget->selectedItem();

	if (ent == NULL)
		return;

	NSTileBrushComp * comp = ent->get<NSTileBrushComp>();

	if (comp != NULL)
		comp->setHeight(pHeight);

	mUI.mMapView->setFocus();
}

void Toolkit::onShowAllHidden(bool pState)
{
	NSScene * scene = nsengine.currentScene();
	if (scene == NULL)
		return;

	nspentityset ents = scene->entities();
	auto iter = ents.begin();
	while (iter != ents.end())
	{
		NSTFormComp * tComp = (*iter)->get<NSTFormComp>();
		tComp->setHiddenState(NSTFormComp::Show, pState);
		++iter;
	}
}

void Toolkit::onSwitchMaps()
{
	ManageMapsDialog mMaps(this);
	mMaps.init(this);

	if (mMaps.exec() == QDialog::Accepted)
	{

	}
	mUI.mMapView->setFocus();
}

void Toolkit::onSave()
{
	NSBuildSystem::BrushMode tmpBrushMode = nsengine.system<NSBuildSystem>()->brushMode();
	NSBuildSystem::Mode tmpMode = nsengine.system<NSBuildSystem>()->mode();
	fvec4 tmpCol = nsengine.system<NSBuildSystem>()->activeBrushColor();
	NSEntity * tmpEnt = nsengine.system<NSBuildSystem>()->buildent();

	bool toggle = mUI.actionLayerMode->isChecked();
	int tmpCurLayer = spinBoxVal;
	if (toggle)
		onLayerMode(false);

	if (!mUI.actionSelect->isChecked())
		nsengine.system<NSBuildSystem>()->enable(false, fvec2());
	
	nsengine.save(nsengine.active());

	if (!mUI.actionSelect->isChecked())
	{
		nsengine.system<NSBuildSystem>()->setBrushMode(tmpBrushMode);
		nsengine.system<NSBuildSystem>()->setMode(tmpMode);
		nsengine.system<NSBuildSystem>()->setActiveBrushColor(tmpCol);
		nsengine.system<NSBuildSystem>()->setBuildEnt(tmpEnt);
		nsengine.system<NSBuildSystem>()->enable(true, fvec2());
		spinBoxVal = 0;
		onChangeLayer(tmpCurLayer);
	}

	if (toggle)
		onLayerMode(toggle);

	mUI.mMapView->setFocus();
}

void Toolkit::onToggleXFB(bool pEnable)
{
	NSScene * curScene = nsengine.currentScene();
	if (curScene != NULL)
	{
		uivec3 selObj = nsengine.system<NSSelectionSystem>()->center();
		NSEntity * ent = curScene->entity(selObj.xy());
		if (ent != NULL)
		{
			NSTFormComp * tComp = ent->get<NSTFormComp>();
			tComp->enableTransformFeedback(pEnable);
		}
	}
}

void Toolkit::onUnlock(bool pVal)
{
	nsengine.system<NSBuildSystem>()->enableOverwrite(!pVal);
	mUI.mMapView->setFocus();
}

ObjectView * Toolkit::objectView()
{
	return mUI.mObjectView;
}

TileView *  Toolkit::tileView()
{
	return mUI.mTileView;
}

void Toolkit::onReplaceTile()
{
	auto selItems = mUI.mTileView->getUI()->mListWidget->selectedItems();
	if (selItems.isEmpty())
		return;
	QListWidgetItem * item = selItems.first();
	if (item == NULL)
		return;

	uivec2 id(item->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), item->data(VIEW_WIDGET_ITEM_ENT).toUInt());
	NSEntity * newTile = nsengine.resource<NSEntity>(id);
	if (newTile == NULL)
		return;

	nsengine.system<NSSelectionSystem>()->tileswap(newTile);
	mUI.mMapView->setFocus();
}

void Toolkit::onResourceBrowser()
{
	mResourceBrowser->show();
}

void Toolkit::refreshViews()
{
	mUI.mTileView->refresh();
	mUI.mObjectView->refresh();
	mBrushMenuWidget->setupLW();
}

void Toolkit::onDebugView(bool pNewVal)
{
	nsengine.system<NSRenderSystem>()->enableDebugDraw(pNewVal);
}

void Toolkit::onDelSelection()
{
	nsengine.system<NSSelectionSystem>()->del();
	mUI.mMapView->setFocus();
}

void Toolkit::onNewBrush()
{
	AddNewBrushDialog brushD(this);

	brushD.init(this);
	brushD.setFromEngineSelection();

	if (brushD.exec() == QDialog::Accepted)
	{

	}
	mUI.mMapView->setFocus();
}

void Toolkit::onSetCurrentBrush(NSEntity * pBrush)
{
	if (pBrush == NULL)
		return;

	mBrushMenuWidget->setupLW();
	nsengine.system<NSBuildSystem>()->setTileBrush(pBrush);

	if (mUI.actionEraserBrush->isChecked())
		onEraserBrush(true);
	else if (mUI.actionTileBrush->isChecked())
		onTileBrush(true);

	if (!pBrush->iconPath().empty())
		mBrushMenu->setIcon(QIcon(pBrush->iconPath().c_str()));

	mBrushMenuWidget->setSelectedItem(pBrush);

	NSTileBrushComp * comp = pBrush->get<NSTileBrushComp>();
	if (comp != NULL)
		comp->setHeight(mBrushHeight->value());
	mUI.mMapView->setFocus();
}

void Toolkit::onBrushChange(QListWidgetItem* pCurrent)
{
	if (pCurrent == NULL)
	{
		mBrushMenu->setIcon(QIcon());
		return;
	}

	uivec2 id(pCurrent->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), pCurrent->data(VIEW_WIDGET_ITEM_ENT).toUInt());
	NSEntity * ent = nsengine.resource<NSEntity>(id);
	if (ent == NULL)
	{
		QMessageBox mb(QMessageBox::Warning, "Error in brush", "The brush with name " + pCurrent->text() + " can not be found for some reason...", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	nsengine.system<NSBuildSystem>()->setTileBrush(ent);
	NSTileBrushComp * comp = ent->get<NSTileBrushComp>();
	if (comp != NULL)
		comp->setHeight(mBrushHeight->value());

	if (mUI.actionEraserBrush->isChecked())
		onEraserBrush(true);
	else if (mUI.actionTileBrush->isChecked())
		onTileBrush(true);

	mBrushMenu->setIcon(pCurrent->icon());
	mUI.mMapView->setFocus();
}

void Toolkit::onLayerIndexChange(const QString & pItem)
{
	if (mUI.actionLayerMode->isChecked())
	{
		mLayerCB->blockSignals(true);
		mLayerCB->setCurrentText(mPrevLayerText);
		onLayerMode(false);
		mLayerCB->setCurrentText(pItem);
		mPrevLayerText = pItem;
		mLayerCB->blockSignals(false);
		onLayerMode(true);
	}
}

void Toolkit::onLayerMode(bool pVal)
{
	NSScene * scene = nsengine.currentScene();
	if (scene == NULL)
		return;

	int val = mCurrentLayer->value();
	if (pVal)
		mLayersAboveHidden = val;
	else
	{
		val = mLayersAboveHidden;
		mLayersAboveHidden = 0;
	}

	if (mLayerCB->currentText() == LAYER_ABOVE_TEXT)
		scene->hideLayersAbove(val, pVal);
	else if (mLayerCB->currentText() == LAYER_BELOW_TEXT)
		scene->hideLayersBelow(val, pVal);
	else if (mLayerCB->currentText() == LAYER_ALL_TEXT)
	{
		scene->hideLayersAbove(val, pVal);
		scene->hideLayersBelow(val, pVal);
	}
	else
		scene->hideLayer(val, pVal);

	mUI.mMapView->setFocus();
}

void Toolkit::onBrushDoubleClick()
{
	mBrushMenu->hide();
	mUI.mMapView->setFocus();
}

void Toolkit::onStampMode(bool pVal)
{
	NSInputMap * input = nsengine.resource<NSInputMap>(nsengine.system<NSInputSystem>()->inputMap());
	NSInputMap::Trigger trigger1(INSERT_OBJECT, NSInputMap::Key_None, NSInputMap::Key_None, NSInputMap::LeftButton);
	NSInputMap::Trigger trigger2(INSERT_OBJECT, NSInputMap::Key_LCtrl, NSInputMap::Key_None, NSInputMap::LeftButton);
	if (pVal)
	{
		input->removeMouseButtonTrigger(BUILD_MODE_CONTEXT, NSInputMap::Movement, trigger1);
		input->removeMouseButtonTrigger(BUILD_MODE_CONTEXT, NSInputMap::Movement, trigger2);
	}
	else
	{
		input->addMouseButtonTrigger(BUILD_MODE_CONTEXT, NSInputMap::Movement, trigger1);
		input->addMouseButtonTrigger(BUILD_MODE_CONTEXT, NSInputMap::Movement, trigger2);
	}
}

void Toolkit::onHideSelection()
{
	NSSelectionSystem * selsys = nsengine.system<NSSelectionSystem>();
	selsys->setHiddenState(NSTFormComp::ObjectHide, true);
}

void Toolkit::onUnhideSelection()
{
	NSSelectionSystem * selsys = nsengine.system<NSSelectionSystem>();
	selsys->setHiddenState(NSTFormComp::ObjectHide, false);
}

void Toolkit::onCamCenter(bool pVal)
{
	if (pVal)
	{
		mUI.actionObjectCenter->setChecked(!pVal);
		nsengine.system<NSCameraSystem>()->setMode(NSCameraSystem::Free);
	}
	mUI.actionCameraCenter->setChecked(true);
	mUI.mMapView->setFocus();
}

void Toolkit::onCamFront(bool pVal)
{
	if (pVal)
	{
		mUI.actionIsoView->setChecked(false);
		mUI.actionTopDown->setChecked(false);

		NSCameraSystem * camSys = nsengine.system<NSCameraSystem>();
		camSys->setView(NSCameraSystem::Front);
	}
	else
	{
		NSCameraSystem * camSys = nsengine.system<NSCameraSystem>();
		camSys->setView(NSCameraSystem::Normal);
	}
	mUI.mMapView->setFocus();
}

void Toolkit::onObjectCenter(bool pVal)
{
	if (pVal)
	{
		mUI.actionCameraCenter->setChecked(!pVal);
		nsengine.system<NSCameraSystem>()->setMode(NSCameraSystem::Focus);
	}
	mUI.actionObjectCenter->setChecked(true);
	mUI.mMapView->setFocus();
}

void Toolkit::onCamIso(bool pVal)
{
	if (pVal)
	{
		mUI.actionFrontView->setChecked(false);
		mUI.actionTopDown->setChecked(false);

		NSCameraSystem * camSys = nsengine.system<NSCameraSystem>();
		camSys->setView(NSCameraSystem::Iso);
	}
	else
	{
		mUI.actionIsoView->setChecked(false);
		mUI.actionTopDown->setChecked(false);

		NSCameraSystem * camSys = nsengine.system<NSCameraSystem>();
		camSys->setView(NSCameraSystem::Normal);
	}
	mUI.mMapView->setFocus();
}

void Toolkit::onCamTop(bool pVal)
{
	if (pVal)
	{
		mUI.actionFrontView->setChecked(false);
		mUI.actionIsoView->setChecked(false);

		NSCameraSystem * camSys = nsengine.system<NSCameraSystem>();
		camSys->setView(NSCameraSystem::Top);
	}
	else
	{
		mUI.actionIsoView->setChecked(false);
		mUI.actionTopDown->setChecked(false);

		NSCameraSystem * camSys = nsengine.system<NSCameraSystem>();
		camSys->setView(NSCameraSystem::Normal);
	}
	mUI.mMapView->setFocus();
}

void Toolkit::onRedo()
{
	mUI.mMapView->setFocus();
}

void Toolkit::onUndo()
{
	mUI.mMapView->setFocus();
}


