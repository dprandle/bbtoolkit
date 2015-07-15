#include <nsengine.h>
#include <qtreewidget.h>
#include <nsscenemanager.h>
#include <nsscene.h>
#include <managemapsdialog.h>
#include <newmapdialog.h>
#include <qmessagebox.h>
#include <toolkit.h>
#include <nsselectionsystem.h>
#include <ui_editmapdialog.h>
#include <qcolordialog.h>
#include <nsplugin.h>
#include <nspluginmanager.h>

ManageMapsDialog::ManageMapsDialog(QWidget * parent):
QDialog(parent),
mEditD(this),
mNewMapD(new NewMapDialog(parent))
{

}

ManageMapsDialog::~ManageMapsDialog()
{
	delete mNewMapD;
}

/*!
Initialize the dialog table widget with scenes int the current plugin
*/
void ManageMapsDialog::init(Toolkit * pTK)
{
	mTK = pTK;

	mUI.setupUi(this);
	mNewMapD->init(pTK);
	_rescanPluginMaps();

	mEditMapUI.setupUi(&mEditD);
	connect(mEditMapUI.mColorBtn_2, SIGNAL(clicked()), this, SLOT(onEditColClick()));
}

/*!
When a item is selected we want to display the maps information in the appropriate fields
*/
void ManageMapsDialog::onMapItemChanged(QTreeWidgetItem* pItem, int pColumn)
{
	if (pItem == NULL) // Dont do anything if NULL item clicked.. not sure this can actually even happen
		return;

	if (pItem->checkState(0) == Qt::Unchecked)
		pItem->setCheckState(pColumn, Qt::Checked);

	mUI.mMapsTW->blockSignals(true);
	for (nsint index = 0; index < mUI.mMapsTW->topLevelItemCount(); ++index)
	{
		auto item = mUI.mMapsTW->topLevelItem(index);
		if (pItem != item)
		{
			item->setCheckState(pColumn, Qt::Unchecked);
		}
	}
	mUI.mMapsTW->blockSignals(false);
}

void ManageMapsDialog::onMapItemPressed(QTreeWidgetItem* pItem, int pColumn)
{
	if (pItem == NULL) // Dont do anything if NULL item clicked.. not sure this can actually even happen
		return;

	NSPlugin * aplg = nsengine.active();

	NSScene * mMap = aplg->resource<NSScene>(pItem->text(pColumn).toStdString()); // Get the map that was clicked by looking up the name

	if (mMap == NULL) // Again do nothing if map not found
		return;

	// Now go through and start setting the fields
	mUI.mCreatorLE->setText(mMap->creator().c_str());
	mUI.mNotesTextBox->setPlainText(mMap->notes().c_str());
	mUI.mMaxPlayersLbl->setText(QString::number(mMap->maxPlayers()));

	pItem->setCheckState(pColumn, Qt::Checked);
}

void ManageMapsDialog::onNewMap()
{
	if (mNewMapD->exec() == QDialog::Accepted)
	{
		_rescanPluginMaps();
	}
}

void ManageMapsDialog::onDeleteMap()
{
	auto items = mUI.mMapsTW->selectedItems();
	if (items.isEmpty())
		return;

	QString mapNameQt = items.first()->text(0);
	QMessageBox mb(QMessageBox::Warning, "Delete", "Do you want to remove " + mapNameQt+" permanently from the hard drive ? ", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
	int ret = mb.exec();
	if (ret != QMessageBox::Cancel)
	{
		nsstring mapName = mapNameQt.toStdString();
		NSScene * map = nsengine.active()->resource<NSScene>(mapName);

		bool remFromFile = (ret == QMessageBox::Yes);
		if (remFromFile)
			nsengine.delResource(map);
		else
			nsengine.unloadResource(map);

		_rescanPluginMaps();
	}
}

void ManageMapsDialog::onEditMap()
{
	auto items = mUI.mMapsTW->selectedItems();
	if (items.isEmpty())
		return;

	nsstring mapName = items.first()->text(0).toStdString();
	NSScene * map = nsengine.active()->resource<NSScene>(mapName);

	mEditMapUI.mMapNameLE->setText(mapName.c_str());
	mEditMapUI.mCreatorLE->setText(map->creator().c_str());
	mEditMapUI.mMaxPlayersSB_2->setValue(map->maxPlayers());
	mEditMapUI.mSceneNotesPTE_2->setPlainText(map->notes().c_str());

	fvec3 col = map->backgroundColor();
	prevC.setRgbF(col.x, col.y, col.z);

	QString rgbcol = "rgb(" + QString::number(prevC.red()) + "," + QString::number(prevC.green()) + "," + QString::number(prevC.blue()) + ");";
	mEditMapUI.mColorBtn_2->setStyleSheet(
		"QFrame {\
		background-color: " + rgbcol +
		"border: none\
		}\
		QFrame:hover\
		{\
		background-color: rgb(200,200,200);\
		}");

	if (mEditD.exec() == QDialog::Accepted)
	{
		nsstring name = mEditMapUI.mMapNameLE->text().toStdString();
		if (map->name() != name)
			map->rename(name);

		nsstring creator = mEditMapUI.mCreatorLE->text().toStdString();
		if (map->creator() != creator)
			map->setCreator(creator);

		if (map->maxPlayers() != mEditMapUI.mMaxPlayersSB_2->value())
			map->setMaxPlayers(mEditMapUI.mMaxPlayersSB_2->value());

		nsstring notes = mEditMapUI.mSceneNotesPTE_2->toPlainText().toStdString();
		if (map->notes() != notes)
			map->setNotes(notes);

		fvec3 col(map->backgroundColor());
		fvec3 nCol(prevC.redF(), prevC.greenF(), prevC.blueF());
		if (col != nCol)
			map->setBackgroundColor(nCol);

		_rescanPluginMaps();
	}
}

void ManageMapsDialog::onEditColClick()
{
	
	QColor c = QColorDialog::getColor(prevC, this);
	if (c.isValid())
		prevC = c;

	QString rgbcol = "rgb(" + QString::number(prevC.red()) + "," + QString::number(prevC.green()) + "," + QString::number(prevC.blue()) + ");";
	mEditMapUI.mColorBtn_2->setStyleSheet(
		"QFrame {\
		background-color: " + rgbcol +
		"border: none\
		}\
		QFrame:hover\
		{\
		background-color: rgb(200,200,200);\
		}");
}

/*!
Helper function to scan in the maps that are included in the current plugin
*/
void ManageMapsDialog::_rescanPluginMaps()
{
	mUI.mMapsTW->clear();
	NSSceneManager * sceneManager = nsengine.active()->manager<NSSceneManager>();
	auto sceneBeg = sceneManager->begin();
	while (sceneBeg != sceneManager->end())
	{
		NSScene * scene = (NSScene*)sceneBeg->second;
		if (scene->plugid() == nsengine.active()->id())
		{
			QTreeWidgetItem * item = new QTreeWidgetItem();
			item->setText(0, sceneBeg->second->name().c_str());

			if (sceneManager->current() == scene)
				item->setCheckState(0, Qt::Checked);
			else
				item->setCheckState(0, Qt::Unchecked);

			mUI.mMapsTW->addTopLevelItem(item);
		}
		++sceneBeg;
	}
}

void ManageMapsDialog::onLoad()
{
	mTK->onSelect(true);
	nsengine.system<NSSelectionSystem>()->clear();

	for (nsint index = 0; index < mUI.mMapsTW->topLevelItemCount(); ++index)
	{
		auto item = mUI.mMapsTW->topLevelItem(index);
		if (item->checkState(0) == Qt::Checked)
		{
			nsstring mapName = item->text(0).toStdString();
			NSScene * newmap = nsengine.active()->resource<NSScene>(mapName);
			NSScene * curMap = nsengine.currentScene();

			if (newmap == curMap)
			{
				accept();
				return;
			}

			bool saveChanges = false;
			if (curMap != NULL)
			{
				if (nsengine.resourceChanged(curMap))
				{
					QMessageBox mb(QMessageBox::Warning, "Save Current Map", "There are unsaved changes in the current map. Would you like to save before switching maps?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
					int ret = mb.exec();
					if (ret == QMessageBox::Cancel)
						return;
					saveChanges = (ret == QMessageBox::Yes);
				}
			}
			nsengine.setCurrentScene(newmap, false, saveChanges);
			accept();
			return;
		}
	}

	NSScene * curMap = nsengine.currentScene();
	bool saveChanges = false;
	if (curMap != NULL && nsengine.resourceChanged(curMap))
	{
		QMessageBox mb(QMessageBox::Warning, "Save Current Map", "There are unsaved changes in the current map. Would you like to save before switching maps?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, this);
		int ret = mb.exec();
		if (ret == QMessageBox::Cancel)
			return;
		saveChanges = (ret == QMessageBox::Yes);
	}
	nsengine.setCurrentScene(nullptr, false, saveChanges);
}

void ManageMapsDialog::onCancel()
{
	reject();
}
