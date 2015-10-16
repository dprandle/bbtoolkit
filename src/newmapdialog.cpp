#include <newmapdialog.h>
#include <qmessagebox.h>
#include <newtiledialog.h>
#include <nsengine.h>
#include <nsentitymanager.h>
#include <toolkit.h>
#include <qcolordialog.h>
#include <nsscene.h>
#include <nsscenemanager.h>
#include <nsplugin.h>
#include <nstilecomp.h>
#include <nspluginmanager.h>
#include <ui_selecttiledialog.h>
#include <ui_selectresdialog.h>
#include <listwidgetcust.h>

NewMapDialog::NewMapDialog(QWidget * parent) :
prevC(Qt::white),
QDialog(parent)
{}

NewMapDialog::~NewMapDialog()
{}

void NewMapDialog::init(Toolkit * pTK)
{
	mTK = pTK;
	mNewMapUI.setupUi(this);
	_setColorStyle();
	connect(mNewMapUI.mColorBtn, SIGNAL(clicked()), this, SLOT(onColorChange()));
}

void NewMapDialog::onChooseSkybox()
{
	QDialog srd(this);

	Ui::SelectResDialog ui;
	ui.setupUi(&srd);

	// Go through and add entities to the list widget which have camera component
	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);
		auto entiter = plg->manager<NSEntityManager>()->begin();
		while (entiter != plg->manager<NSEntityManager>()->end())
		{
			NSEntity * curent = plg->get<NSEntity>(entiter->first);
			NSRenderComp * rc = curent->get<NSRenderComp>();
			if (rc != NULL)
			{
				NSMaterial * mat = nsengine.resource<NSMaterial>(rc->materialID(0));
				if (mat != NULL)
				{
					NSTexture * cm = nsengine.resource<NSTexture>(mat->mapTextureID(NSMaterial::Diffuse));
					if (cm != NULL && type_to_hash(*cm) == type_to_hash(NSTexCubeMap))
					{
						QListWidgetItem * item = new QListWidgetItem(curent->name().c_str());
						item->setData(VIEW_WIDGET_ITEM_PLUG, curent->plugid());
						item->setData(VIEW_WIDGET_ITEM_ENT, curent->id());
						ui.mListWidget->addItem(item);
					}
				}
			}
			++entiter;
		}
		++plugiter;
	}

	// Now open up the basic list widget dialog to allow the user to select which
	// camera to use
	if (srd.exec() == QDialog::Accepted)
	{
		auto selitems = ui.mListWidget->selectedItems();
		if (selitems.isEmpty())
			return;
		auto fitem = selitems.first();
		uivec2 skyboxid(fitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), fitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

		NSEntity * skyboxent = nsengine.resource<NSEntity>(skyboxid);
		mNewMapUI.mSkyboxLE->setText(skyboxent->name().c_str());
		mNewMapUI.mSkyboxPluginLE->setText(nsengine.plugin(skyboxent->plugid())->name().c_str());
	}
}

void NewMapDialog::onChooseTile()
{
	QDialog srd(this);

	Ui::SelectResDialog ui;
	ui.setupUi(&srd);

	// Go through and add entities to the list widget which have tile component
	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);
		auto entiter = plg->manager<NSEntityManager>()->begin();
		while (entiter != plg->manager<NSEntityManager>()->end())
		{
			NSEntity * curent = plg->get<NSEntity>(entiter->first);
			if (curent->has<NSTileComp>())
			{
				QListWidgetItem * item = new QListWidgetItem(curent->name().c_str());
				item->setData(VIEW_WIDGET_ITEM_PLUG, curent->plugid());
				item->setData(VIEW_WIDGET_ITEM_ENT, curent->id());
				ui.mListWidget->addItem(item);
			}
			++entiter;
		}
		++plugiter;
	}

	// Now open up the basic list widget dialog to allow the user to select which
	// directional light to use
	if (srd.exec() == QDialog::Accepted)
	{
		auto selitems = ui.mListWidget->selectedItems();
		if (selitems.isEmpty())
			return;
		auto fitem = selitems.first();
		uivec2 tileid(fitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), fitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

		NSEntity * tileent = nsengine.resource<NSEntity>(tileid);
		mNewMapUI.mTileNameLE->setText(tileent->name().c_str());
		mNewMapUI.mTilePluginLE->setText(nsengine.plugin(tileent->plugid())->name().c_str());
	}
}

void NewMapDialog::onNewSkybox()
{
	// TODO: Once new skybox function is written reference it here
}

void NewMapDialog::onCameraCB(bool var)
{
	mNewMapUI.mCamLE->setEnabled(!var);
	mNewMapUI.mCamPluginLE->setEnabled(!var);
	mNewMapUI.mNewCamBtn->setEnabled(!var);
	mNewMapUI.mChooseCamBtn->setEnabled(!var);
	mNewMapUI.mCamNameLbl->setEnabled(!var);
	mNewMapUI.mCamPluginLbl->setEnabled(!var);
}

void NewMapDialog::onDirLightCB(bool var)
{
	mNewMapUI.mDirLightLE->setEnabled(!var);
	mNewMapUI.mLightPluginLE->setEnabled(!var);
	mNewMapUI.mNewDirLightBtn->setEnabled(!var);
	mNewMapUI.mChooseLightBtn->setEnabled(!var);
	mNewMapUI.mLightNameLbl->setEnabled(!var);
	mNewMapUI.mLightPluginLbl->setEnabled(!var);
}

void NewMapDialog::onChooseCamera()
{
	QDialog srd(this);

	Ui::SelectResDialog ui;
	ui.setupUi(&srd);

	// Go through and add entities to the list widget which have camera component
	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);
		auto entiter = plg->manager<NSEntityManager>()->begin();
		while (entiter != plg->manager<NSEntityManager>()->end())
		{
			NSEntity * curent = plg->get<NSEntity>(entiter->first);
			if (curent->has<NSCamComp>())
			{
				QListWidgetItem * item = new QListWidgetItem(curent->name().c_str());
				item->setData(VIEW_WIDGET_ITEM_PLUG, curent->plugid());
				item->setData(VIEW_WIDGET_ITEM_ENT, curent->id());
				ui.mListWidget->addItem(item);
			}
			++entiter;
		}
		++plugiter;
	}

	// Now open up the basic list widget dialog to allow the user to select which
	// camera to use
	if (srd.exec() == QDialog::Accepted)
	{
		auto selitems = ui.mListWidget->selectedItems();
		if (selitems.isEmpty())
			return;
		auto fitem = selitems.first();
		uivec2 camid(fitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), fitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

		NSEntity * cament = nsengine.resource<NSEntity>(camid);
		mNewMapUI.mCamLE->setText(cament->name().c_str());
		mNewMapUI.mCamPluginLE->setText(nsengine.plugin(cament->plugid())->name().c_str());
	}
}

void NewMapDialog::onNewCamera()
{
	//TODO: Once new camera entity function is written write reference to it here
}

void NewMapDialog::onChooseDirlight()
{
	QDialog srd(this);

	Ui::SelectResDialog ui;
	ui.setupUi(&srd);

	// Go through and add entities to the list widget which have light component
	// and light component is of type "DirLight"
	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);
		auto entiter = plg->manager<NSEntityManager>()->begin();
		while (entiter != plg->manager<NSEntityManager>()->end())
		{
			NSEntity * curent = plg->get<NSEntity>(entiter->first);
			NSLightComp * lc = curent->get<NSLightComp>();
			if (lc != NULL && lc->type() == NSLightComp::Directional)
			{
				QListWidgetItem * item = new QListWidgetItem(curent->name().c_str());
				item->setData(VIEW_WIDGET_ITEM_PLUG, curent->plugid());
				item->setData(VIEW_WIDGET_ITEM_ENT, curent->id());
				ui.mListWidget->addItem(item);
			}
			++entiter;
		}
		++plugiter;
	}

	// Now open up the basic list widget dialog to allow the user to select which
	// directional light to use
	if (srd.exec() == QDialog::Accepted)
	{
		auto selitems = ui.mListWidget->selectedItems();
		if (selitems.isEmpty())
			return;
		auto fitem = selitems.first();
		uivec2 lid(fitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), fitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

		NSEntity * lightent = nsengine.resource<NSEntity>(lid);
		mNewMapUI.mDirLightLE->setText(lightent->name().c_str());
		mNewMapUI.mLightPluginLE->setText(nsengine.plugin(lightent->plugid())->name().c_str());
	}
}

void NewMapDialog::onNewDirlight()
{
	// TODO: Once new directional light entity type function is written - (including dialog) - then reference it here
}

void NewMapDialog::onNewTile()
{
	NewTileDialog newTileDialog(this);
	newTileDialog.init(mTK);
	if (newTileDialog.exec() == QDialog::Accepted)
	{
		mTK->refreshViews();
		QString name = newTileDialog.getEntityName();
		mNewMapUI.mTileNameLE->setText(name);
	}
}

void NewMapDialog::onColorChange()
{
	QColor c = QColorDialog::getColor(prevC);
	if (c.isValid())
		prevC = c;
	_setColorStyle();
}

void NewMapDialog::onCreate()
{
	nsstring mapName = mNewMapUI.mMapNameLE->text().toStdString();
	nsstring mapCreator = mNewMapUI.mCreatorLE->text().toStdString();
	nsstring mapNotes = mNewMapUI.mSceneNotesPTE->toPlainText().toStdString();
	nsuint maxPlayers = mNewMapUI.mMaxPlayersSB->value();
	fvec3 color = fvec3(prevC.redF(), prevC.greenF(), prevC.blueF());

	NSPlugin * actPlug = nsengine.active();
	if (actPlug == NULL)
	{
		QMessageBox mb(QMessageBox::Warning, "Plugin Error", "No active plugin detected - cannot create map.", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	if (mapName.empty())
	{
		QMessageBox mb(QMessageBox::Warning, "Map Name Error", "Enter a name for the map", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	NSSceneManager * scenes = actPlug->manager<NSSceneManager>();
	if (scenes->contains(mapName))
	{
		QMessageBox mb(QMessageBox::Warning, "Map Name Error", "There is already a map with this name. Please use a different name.", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	NSEntity * tile = NULL;
	if (mNewMapUI.mLoadGridGB->isChecked())
	{
		nsstring tName = mNewMapUI.mTileNameLE->text().toStdString();
		nsstring plugName = mNewMapUI.mTilePluginLE->text().toStdString();
		tile = nsengine.resource<NSEntity>(plugName, tName);
		if (tile == NULL)
		{
			QMessageBox mb(QMessageBox::Warning, "Tile Name Error", "Please specify a valid tile if you want to load a base grid.", QMessageBox::NoButton, this);
			mb.exec();
			return;
		}
	}

	NSEntity * skyb = NULL;
	if (mNewMapUI.mLoadSkyboxGB->isChecked())
	{
		nsstring sname = mNewMapUI.mSkyboxLE->text().toStdString();
		nsstring plugName = mNewMapUI.mSkyboxPluginLE->text().toStdString();
		skyb = nsengine.resource<NSEntity>(plugName, sname);
		if (skyb == NULL)
		{
			QMessageBox mb(QMessageBox::Warning, "Skybox Name Error", "Please specify a valid skybox if you want to load a skybox with the map", QMessageBox::NoButton, this);
			mb.exec();
			return;
		}
	}

	NSEntity * dirLight = NULL;
	if (!mNewMapUI.mCreateDefaultDirLightCB->isChecked())
	{
		nsstring name = mNewMapUI.mDirLightLE->text().toStdString();
		nsstring plugName = mNewMapUI.mLightPluginLE->text().toStdString();
		dirLight = nsengine.resource<NSEntity>(plugName, name);
		if (dirLight == NULL)
		{
			QMessageBox mb(QMessageBox::Warning, "Directional Light Error", "Please specify a valid directional light if you want to load a custom one", QMessageBox::NoButton, this);
			mb.exec();
			return;
		}
	}
	else
		dirLight = nsengine.active()->createDirLight(mapName + "_dlight", 0.6f, 0.3f);

	NSEntity * scenecam = NULL;
	if (!mNewMapUI.mCreateDefaultCamCB->isChecked())
	{
		nsstring name = mNewMapUI.mCamLE->text().toStdString();
		nsstring plugName = mNewMapUI.mCamPluginLE->text().toStdString();
		scenecam = nsengine.resource<NSEntity>(plugName, name);
		if (scenecam == NULL)
		{
			QMessageBox mb(QMessageBox::Warning, "Camera Error", "Please specify a valid camera if you want to load a custom one", QMessageBox::NoButton, this);
			mb.exec();
			return;
		}
	}
	else
		scenecam = nsengine.active()->createCamera(mapName + "_cam", 60.0f, uivec2(mTK->mapView()->width(), mTK->mapView()->height()), fvec2(DEFAULT_Z_NEAR, DEFAULT_Z_FAR));

	NSScene * scene = scenes->create(mapName);
	if (scene == NULL)
	{
		QMessageBox mb(QMessageBox::Warning, "Map Error", "There was an error in creating map with name " + QString(mapName.c_str()) + ". Try a different name", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	scene->setCreator(mapCreator);
	scene->setNotes(mapNotes);
	scene->setMaxPlayers(maxPlayers);
	scene->setBackgroundColor(color);

	NSScene * curScene = nsengine.currentScene();
	nsengine.setCurrentScene(scene, true, true);
	if (tile != NULL)
	{
		ivec3 bounds(mNewMapUI.mGridXSB->value(), mNewMapUI.mGridYSB->value(), mNewMapUI.mHeightSB->value());
		scene->addGridded(tile, bounds);
		actPlug->save(scene);
	}
	if (skyb != NULL)
		scene->setSkydome(skyb, true);
	scene->setCamera(scenecam, true);
	scene->add(dirLight, fvec3(20.0f, 20.0f, -40.0f), ::orientation(fvec4(0, 1, 0, 10)));
	nsengine.setCurrentScene(curScene, false, true);
	accept();
}

void NewMapDialog::onCancel()
{
	reject();
}

void NewMapDialog::_setColorStyle()
{
	QString rgbcol = "rgb(" + QString::number(prevC.red()) + "," + QString::number(prevC.green()) + "," + QString::number(prevC.blue()) + ");";
	mNewMapUI.mColorBtn->setStyleSheet(
		"QFrame {\
		background-color: " + rgbcol +
		"border: none\
		}\
		QFrame:hover\
		{\
		background-color: rgb(200,200,200);\
		}");
}