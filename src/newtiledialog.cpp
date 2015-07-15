#include <newtiledialog.h>
#include <nsengine.h>
#include <qfiledialog.h>
#include <nsentitymanager.h>
#include <nstilecomp.h>
#include <nsoccupycomp.h>
#include <nsinputcomp.h>
#include <nsselcomp.h>
#include <nsmaterial.h>
#include <qmessagebox.h>
#include <nsmatmanager.h>
#include <nsmeshmanager.h>
#include <nstexmanager.h>
#include <nstexture.h>
#include <nsmesh.h>
#include <preview.h>
#include <toolkit.h>
#include <qcolordialog.h>
#include <ui_newfilterdialog.h>
#include <nsplugin.h>
#include <nspluginmanager.h>


NewTileDialog::NewTileDialog(QWidget * parent) :
QDialog(parent),
mTK(NULL),
mTempMat(NULL),
mGenFile(QString()),
diffuseLoaded(false),
normalLoaded(false)
{}

NewTileDialog::~NewTileDialog()
{

}

/*!
Initialize the dialog with some basic rendering settings for the preview window, and
creating a temporary material that is used to store the textures as they are changed.
Upon clicking the add tile button this material is renamed to match the new tile name.
*/
void NewTileDialog::init(Toolkit * pTK)
{
	mTK = pTK;
	mUI.setupUi(this);
	mUI.mPreview->init(mTK);
}

QString NewTileDialog::getEntityName()
{
	return mUI.mNameLE->text();
}

void NewTileDialog::onNewFilter()
{
	QDialog newFilt(this);
	Ui_NewFilterDialog ui;
	ui.setupUi(&newFilt);
	if (newFilt.exec() == QDialog::Accepted)
	{
		mUI.mFilterCB->addItem(ui.mFilterLE->text());
	}
}

/*!
This function is called when the add button is pressed. It takes all of the current settings
from the new tile dialog and creates a tile out of it. A new material will always be created at this
point, however textures are re-used. We may want to add the ability to use existing materials in the
future.
*/
void NewTileDialog::onAdd()
{
	//// Throw exception if engine pointer is null
	//if (mEngine == NULL)
	//	throw(std::exception("Engine pointer is NULL"));

	//// Get the materials and textures from the engine
	//NSMatManager * mats = mEngine->getManager<NSMatManager>();
	//NSTexManager * textures = mEngine->getManager<NSTexManager>();

	//// Get the name and sub directories from the filter and name line edits
	//nsstring name = mUI.mNameLE->text().toStdString();
	//nsstring subDir = mUI.mFilterCB->currentText().toStdString() + "/";

	//// Sub dir will be None by default (no filter)
	//if (subDir == "None/")
	//	subDir = "";

	//// Get the icon path
	////nsstring normalPath = mUI.mMatNormalLE->text().toStdString();
	//nsstring iconPath = mUI.mIconFileLE->text().toStdString();

	//// Now check to make sure the name field was not left empty - we have to have a name to create resources
	//// so if there is no name then we have to return
	//if (name.empty())
	//{
	//	QMessageBox mb(this);
	//	mb.setText("Please enter a name for the new tile.");
	//	mb.exec();
	//	return;
	//}

	//nsstring matName = name + "Mat";
	//// We have to rename the temporarily created material - if the rename fails it means that most likely
	//// the material already exists
	//if (!mats->rename(mTempMat->getResourceName(), matName))
	//{
	//	QMessageBox mb(this);
	//	mb.setText("Unable to create material with name: " + QString(matName.c_str()) + ". Try renaming the tile.");
	//	mb.exec();
	//	return;
	//}

	//// Alright get the basic tile mesh as stored by the engine - we are counting on this already being 
	//// loaded - if it is not something is wrong
	//// We might add different tile types in the future - this is where we would want to insert them.
	//NSMesh * tileMesh = NULL;
	//if (mUI.mTypeCB->currentText() == "Full Tile")
	//	tileMesh = mEngine->getManager<NSMeshManager>()->get(MESH_FULL_TILE);
	//else
	//	tileMesh = mEngine->getManager<NSMeshManager>()->get(MESH_HALF_TILE);

	//// As mentioned before - if the tile mesh did not load then something weird is happening.. however it would still
	//// be good to avoid a crash
	//if (tileMesh == NULL)
	//{
	//	QMessageBox mb(this);
	//	mb.setText("Something has gone pretty badly wrong - we can't seem to find where the mesh for the tile is...");
	//	mb.exec();
	//	if (diffuseLoaded)
	//		textures->remove(mTempMat->getMapTextureID(NSMaterial::Diffuse));
	//	if (normalLoaded)
	//		textures->remove(mTempMat->getMapTextureID(NSMaterial::Normal));
	//	mats->remove(mTempMat->getResourceID());
	//	return;
	//}

	//// Create the new tile entity. If this fails then throw up a message box and delete the newly created textures
	//// (if there were textures created).
	//NSEntity * newTile = mEngine->getManager<NSEntityManager>()->create(name);
	//if (newTile == NULL)
	//{
	//	QMessageBox mb(this);
	//	mb.setText("An object or tile already exists with that name. Please choose another name.");
	//	mb.exec();
	//	if (diffuseLoaded)
	//		textures->remove(mTempMat->getMapTextureID(NSMaterial::Diffuse));
	//	if (normalLoaded)
	//		textures->remove(mTempMat->getMapTextureID(NSMaterial::Normal));
	//	mats->remove(mTempMat->getResourceID());
	//	return;
	//}

	//// Set the texture subdirectories if the material has textures - this may expand
	//// in the future to include more types of texture maps
	//NSTexture* texD = textures->get(mTempMat->getMapTextureID(NSMaterial::Diffuse));
	//NSTexture* texN = textures->get(mTempMat->getMapTextureID(NSMaterial::Normal));

	//if (texD != NULL)
	//	texD->setSubDir(subDir);
	//if (texN != NULL)
	//	texN->setSubDir(subDir);

	//// Also set the material and entity subdirectories
	//mTempMat->setSubDir(subDir);
	//newTile->setSubDir(subDir);
	//
	//// Set up the tile and render components - 
	//// tile component has the movement modifier but thats really it - currently
	//// the movement modifier field still needs to be added
	//// The tile component however makes it so that the entity can be used as a brush
	//NSTileComp * tileComp = newTile->createComponent<NSTileComp>();

	//// Set the rendering material and mesh ID
	//NSRenderComp * renComp = newTile->createComponent<NSRenderComp>();
	//renComp->setMaterial(0, mTempMat->getResourceID());
	//renComp->setMeshID(tileMesh->getResourceID());
	//renComp->setCastShadow(mUI.mCastShadowsCB->isChecked());

	//// Create the selection component used for the toolkit
	//NSSelComp * selComp = newTile->createComponent<NSSelComp>();

	//// Create an input component with all the normal commands
	//NSInputComp * inComp = newTile->createComponent<NSInputComp>();
	//inComp->add(SELECT);
	//inComp->add(ROTATE_X);
	//inComp->add(ROTATE_Z);
	//inComp->add(MULTI_SELECT);
	//inComp->add(PAINT_SELECT);
	//inComp->add(DRAG_OBJECT_XY);
	//inComp->add(DRAG_OBJECT_XZ);
	//inComp->add(DRAG_OBJECT_YZ);
	//inComp->add(XZ_MOVE_END);
	//inComp->add(YZ_MOVE_END);
	//inComp->add(SHIFT_DONE);

	//// If the tile has an icon give it an icon component
	//if (!iconPath.empty())
	//	newTile->createComponent<NSIconComp>()->setIconPath(iconPath);

	//// If tile is collidable give it an occupy component
	//if (mUI.mCollidableChk->isChecked())
	//{
	//	NSOccupyComp * occComp = newTile->createComponent<NSOccupyComp>();
	//	occComp->build(mEngine->getManager<NSMeshManager>()->get(MESH_FULL_TILE)->getAABB());
	//}

	//// Get the active plugin and make sure that it is valid
	//NSPlugin * activePlug = mEngine->getManager<NSPluginManager>()->getActive();

	//// If not valid throw up a message box
	//if (activePlug == NULL)
	//{
	//	QMessageBox mb(this);
	//	mb.setText("Could not add resources to plugin.. No active plugin detected.");
	//	mb.exec();
	//}

	//// Add all of the resources to the active plugin
	//activePlug->addResource(NSEntityManager::getResTypeString(), newTile->getResourceID());
	//activePlug->addResource(NSMatManager::getResTypeString(), mTempMat->getResourceID());
	//if (texD != NULL)
	//	activePlug->addResource(NSTexManager::getResTypeString(), mTempMat->getMapTextureID(NSMaterial::Diffuse));
	//if (texN != NULL)
	//	activePlug->addResource(NSTexManager::getResTypeString(), mTempMat->getMapTextureID(NSMaterial::Normal));
	//activePlug->addResource(NSMeshManager::getResTypeString(), tileMesh->getResourceID());
	done(Accepted);
}

void NewTileDialog::onColorChange()
{
	//fvec3 orCol = mUI.mPreview->getColor();
	//QColor orColQ(orCol.x*255, orCol.y*255, orCol.z*255);
	//QColor c = QColorDialog::getColor(orColQ);
	//if (!c.isValid())
	//	c = orColQ;

	//fvec3 color(c.redF(),c.greenF(),c.blueF());
	//mUI.mPreview->setColor(color);

	//QPalette palette = mUI.mColorFrame->palette();
	//palette.setColor(backgroundRole(), c);
	//mUI.mColorFrame->setPalette(palette);
	//mUI.mColorFrame->setAutoFillBackground(true);
}

void NewTileDialog::onMatColorChange()
{
	//if (mTempMat == NULL)
	//	return;

	//fvec4 orCol = mTempMat->getColor();
	//QColor orColQ(orCol.x * 255, orCol.y * 255, orCol.z*255,orCol.w*255);
	//QColor c = QColorDialog::getColor(orColQ);

	//fvec4 color(c.redF(), c.greenF(), c.blueF(),c.alphaF());
	//mTempMat->setColor(color);

	//QPalette palette = mUI.mColorFrame->palette();
	//palette.setColor(backgroundRole(), c);
	//mUI.mMatColorFrame->setPalette(palette);
	//mUI.mMatColorFrame->setAutoFillBackground(true);
}

void NewTileDialog::onSpecColorChange()
{
	//if (mTempMat == NULL)
	//	return;

	//fvec3 orCol = mTempMat->getSpecularColor();
	//QColor orColQ(orCol.x * 255, orCol.y * 255, orCol.z * 255);
	//QColor c = QColorDialog::getColor(orColQ);

	//fvec3 color(c.redF(), c.greenF(), c.blueF());
	//mTempMat->setSpecularColor(color);

	//QPalette palette = mUI.mColorFrame->palette();
	//palette.setColor(backgroundRole(), c);
	//mUI.mSpecColorFrame->setPalette(palette);
	//mUI.mSpecColorFrame->setAutoFillBackground(true);
}

void NewTileDialog::onShininessChange(int pVal)
{
	//if (mTempMat == NULL)
	//	return;

	//float shininess = float(pVal) / 100.0f;
	//mTempMat->setSpecularIntensity(shininess);
}

void NewTileDialog::onTileSizeChange(QString pText)
{
	//mUI.mPreview->setFullTile((pText == "Full Tile"));
}

void NewTileDialog::onDiffuseSldrChange(int pNewVal)
{
	//mUI.mPreview->setDiffuse(nsfloat(pNewVal / 100.0f));
}

void NewTileDialog::onNameEdit()
{
	//if (mGenFile.isEmpty())
	//	return;

	//QFile file(mUI.mIconFileLE->text());
	//
	//QString subDir = mUI.mFilterCB->currentText();
	//if (subDir == "None")
	//	subDir = "";

	//if (!subDir.isEmpty())
	//	subDir += "/";

	//QString end = QString(mEngine->getResourceDirectory().c_str()) + QString(mEngine->getManager<NSTexManager>()->getLocalDirectory().c_str()) + "Icons/" + subDir;

	//end += mUI.mNameLE->text() + "Icon.png";

	//file.rename(end);
	//mUI.mIconFileLE->setText(end);
	//mGenFile = end;
}

void NewTileDialog::onAmbientSldrChange(int pNewVal)
{
	//mUI.mPreview->setAmbient(nsfloat(pNewVal / 100.0f));
}

void NewTileDialog::onPreviewDefaults()
{
	//mUI.mDiffuseSldr->setValue(70);
	//mUI.mAmbientSldr->setValue(5);
	//mUI.mPreview->setColor(fvec3(1.0f, 1.0f, 1.0f));
	//mUI.mPreview->resetCam();
	//mUI.mPreview->resetObj();

	//QPalette palette = mUI.mColorFrame->palette();
	//palette.setColor(backgroundRole(), QColor(255,255,255,255));
	//mUI.mColorFrame->setPalette(palette);
	//mUI.mColorFrame->setAutoFillBackground(true);
}

void NewTileDialog::onNormLEEdit(QString pNewText)
{
	//NSTexManager * texManager = mEngine->getManager<NSTexManager>();

	//if (mTempMat == NULL)
	//	return;

	//nsstring fName = pNewText.toStdString();
	//nsuint namePos = fName.find_last_of("/\\");
	//nsuint extPos = fName.find_last_of(".");

	//if (namePos == nsstring::npos || extPos == nsstring::npos)
	//	return;

	//NSTexture * tex = texManager->loadFromFile(fName, false);
	//normalLoaded = true;


	//if (tex == NULL)
	//{
	//	tex = texManager->get(NSTexManager::nameFromFilename(fName));
	//	normalLoaded = false;
	//}

	//if (tex != NULL)
	//{
	//	if (normalLoaded)
	//		mEngine->getManager<NSTexManager>()->remove(mTempMat->getMapTextureID(NSMaterial::Normal));

	//	mTempMat->setMapTextureID(NSMaterial::Normal, tex->getResourceID(), true);
	//}
	//else
	//{
	//	QMessageBox mb(QMessageBox::Warning, "Filename Error", "Could not load normal texture - make sure file exists and filename in box is correct", QMessageBox::NoButton, this);
	//	mb.exec();
	//}
}

void NewTileDialog::onDiffLEEdit(QString pNewText)
{
	//NSTexManager * texManager = mEngine->getManager<NSTexManager>();

	//if (mTempMat == NULL)
	//	return;

	//nsstring fName = pNewText.toStdString();
	//nsuint namePos = fName.find_last_of("/\\");
	//nsuint extPos = fName.find_last_of(".");

	//if (namePos == nsstring::npos || extPos == nsstring::npos)
	//	return;

	//NSTexture * tex = texManager->loadFromFile(fName, false);
	//diffuseLoaded = true;


	//if (tex == NULL)
	//{
	//	tex = texManager->get(NSTexManager::nameFromFilename(fName));
	//	diffuseLoaded = false;
	//}

	//if (tex != NULL)
	//{
	//	if (diffuseLoaded)
	//		mEngine->getManager<NSTexManager>()->remove(mTempMat->getMapTextureID(NSMaterial::Diffuse));

	//	mTempMat->setMapTextureID(NSMaterial::Diffuse, tex->getResourceID(), true);
	//}
	//else
	//{
	//	QMessageBox mb(QMessageBox::Warning, "Filename Error", "Could not load diffuse texture - make sure file exists and filename in box is correct", QMessageBox::NoButton, this);
	//	mb.exec();
	//}
}

void NewTileDialog::onIconLEEdit(QString pNewText)
{
	//QPixmap p(pNewText);
	//mUI.mIconLbl->setPixmap(p.scaled(QSize(128, 128), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void NewTileDialog::reject()
{
	onCancel();
}

void NewTileDialog::onCancel()
{
	//NSMatManager * mats = mEngine->getManager<NSMatManager>();
	//NSTexManager * textures = mEngine->getManager<NSTexManager>();

	//if (diffuseLoaded)
	//	textures->remove(mTempMat->getMapTextureID(NSMaterial::Diffuse));

	//if (normalLoaded)
	//	textures->remove(mTempMat->getMapTextureID(NSMaterial::Normal));

	//mats->remove(mTempMat->getResourceID());
	done(Rejected);
}

void NewTileDialog::onBrowseNormal()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Normal texture", "ImportModels", tr("Image Files (*.png *.jpg *.bmp *.tga *.dds)"));
	mUI.mMatNormalLE->setText(filePath);
}

void NewTileDialog::onBrowseDiffuse()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Diffuse texture", "ImportModels", tr("Image Files (*.png *.jpg *.bmp *.tga *.dds)"));
	mUI.mMatDiffuseLE->setText(filePath);
}

void NewTileDialog::onBrowseIcon()
{
	if (!mGenFile.isEmpty())
		QFile::remove(mGenFile);

	QString filePath = QFileDialog::getOpenFileName(this, "Icon image", "Resources", tr("Image Files (*.png *.jpg *.bmp)"));
	mUI.mIconFileLE->setText(filePath);	
}

void NewTileDialog::onGenerateIcon()
{
	//if (!mGenFile.isEmpty())
	//	QFile::remove(mGenFile);

	//QImage p = mUI.mPreview->grabFrameBuffer(true);

	//QString subDir = mUI.mFilterCB->currentText();
	//if (subDir == "None")
	//	subDir = "";

	//if (!subDir.isEmpty())
	//	subDir += "/";

	//QString end = QString(mEngine->getResourceDirectory().c_str()) + QString(mEngine->getManager<NSTexManager>()->getLocalDirectory().c_str()) + "Icons/" + subDir;

	//end += mUI.mNameLE->text() + "Icon.png";

	//mGenFile = end;
	//p.save(mGenFile);

	//mUI.mIconFileLE->setText("");

	//mUI.mIconFileLE->setText(mGenFile);
}