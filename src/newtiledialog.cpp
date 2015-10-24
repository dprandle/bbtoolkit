#include <nsengine.h>
#include <nsentity_manager.h>
#include <nstile_comp.h>
#include <nsoccupy_comp.h>
#include <nssel_comp.h>
#include <nsmaterial.h>
#include <nsmat_manager.h>
#include <nsmesh_manager.h>
#include <nstex_manager.h>
#include <nstexture.h>
#include <nsmesh.h>
#include <nsplugin.h>
#include <nsplugin_manager.h>

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <preview.h>
#include <toolkit.h>
#include <qcolordialog.h>
#include <ui_newfilterdialog.h>
#include <newtiledialog.h>

NewTileDialog::NewTileDialog(QWidget * parent) :
QDialog(parent),
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
void NewTileDialog::init()
{
	mUI.setupUi(this);
    mUI.mPreview->init();
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
	//nsmat_manager * mats = mEngine->getManager<nsmat_manager>();
	//nstex_manager * textures = mEngine->getManager<nstex_manager>();

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
	//nsmesh * tileMesh = NULL;
	//if (mUI.mTypeCB->currentText() == "Full Tile")
	//	tileMesh = mEngine->getManager<nsmesh_manager>()->get(MESH_FULL_TILE);
	//else
	//	tileMesh = mEngine->getManager<nsmesh_manager>()->get(MESH_HALF_TILE);

	//// As mentioned before - if the tile mesh did not load then something weird is happening.. however it would still
	//// be good to avoid a crash
	//if (tileMesh == NULL)
	//{
	//	QMessageBox mb(this);
	//	mb.setText("Something has gone pretty badly wrong - we can't seem to find where the mesh for the tile is...");
	//	mb.exec();
	//	if (diffuseLoaded)
	//		textures->remove(mTempMat->getMapTextureID(nsmaterial::Diffuse));
	//	if (normalLoaded)
	//		textures->remove(mTempMat->getMapTextureID(nsmaterial::Normal));
	//	mats->remove(mTempMat->getResourceID());
	//	return;
	//}

	//// Create the new tile entity. If this fails then throw up a message box and delete the newly created textures
	//// (if there were textures created).
	//nsentity * newTile = mEngine->getManager<nsentity_manager>()->create(name);
	//if (newTile == NULL)
	//{
	//	QMessageBox mb(this);
	//	mb.setText("An object or tile already exists with that name. Please choose another name.");
	//	mb.exec();
	//	if (diffuseLoaded)
	//		textures->remove(mTempMat->getMapTextureID(nsmaterial::Diffuse));
	//	if (normalLoaded)
	//		textures->remove(mTempMat->getMapTextureID(nsmaterial::Normal));
	//	mats->remove(mTempMat->getResourceID());
	//	return;
	//}

	//// Set the texture subdirectories if the material has textures - this may expand
	//// in the future to include more types of texture maps
	//nstexture* texD = textures->get(mTempMat->getMapTextureID(nsmaterial::Diffuse));
	//nstexture* texN = textures->get(mTempMat->getMapTextureID(nsmaterial::Normal));

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
	//nstile_comp * tileComp = newTile->createComponent<nstile_comp>();

	//// Set the rendering material and mesh ID
	//nsrender_comp * renComp = newTile->createComponent<nsrender_comp>();
	//renComp->setMaterial(0, mTempMat->getResourceID());
	//renComp->setMeshID(tileMesh->getResourceID());
	//renComp->setCastShadow(mUI.mCastShadowsCB->isChecked());

	//// Create the selection component used for the toolkit
	//nssel_comp * selComp = newTile->createComponent<nssel_comp>();


	//// If tile is collidable give it an occupy component
	//if (mUI.mCollidableChk->isChecked())
	//{
	//	nsoccupy_comp * occComp = newTile->createComponent<nsoccupy_comp>();
	//	occComp->build(mEngine->getManager<nsmesh_manager>()->get(MESH_FULL_TILE)->getAABB());
	//}

	//// Get the active plugin and make sure that it is valid
	//nsplugin * activePlug = mEngine->getManager<nsplugin_manager>()->getActive();

	//// If not valid throw up a message box
	//if (activePlug == NULL)
	//{
	//	QMessageBox mb(this);
	//	mb.setText("Could not add resources to plugin.. No active plugin detected.");
	//	mb.exec();
	//}

	//// Add all of the resources to the active plugin
	//activePlug->addResource(nsentity_manager::getResTypeString(), newTile->getResourceID());
	//activePlug->addResource(nsmat_manager::getResTypeString(), mTempMat->getResourceID());
	//if (texD != NULL)
	//	activePlug->addResource(nstex_manager::getResTypeString(), mTempMat->getMapTextureID(nsmaterial::Diffuse));
	//if (texN != NULL)
	//	activePlug->addResource(nstex_manager::getResTypeString(), mTempMat->getMapTextureID(nsmaterial::Normal));
	//activePlug->addResource(nsmesh_manager::getResTypeString(), tileMesh->getResourceID());
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
	//mUI.mPreview->setDiffuse(float(pNewVal / 100.0f));
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

	//QString end = QString(mEngine->getResourceDirectory().c_str()) + QString(mEngine->getManager<nstex_manager>()->getLocalDirectory().c_str()) + "Icons/" + subDir;

	//end += mUI.mNameLE->text() + "Icon.png";

	//file.rename(end);
	//mUI.mIconFileLE->setText(end);
	//mGenFile = end;
}

void NewTileDialog::onAmbientSldrChange(int pNewVal)
{
	//mUI.mPreview->setAmbient(float(pNewVal / 100.0f));
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
	//nstex_manager * texManager = mEngine->getManager<nstex_manager>();

	//if (mTempMat == NULL)
	//	return;

	//nsstring fName = pNewText.toStdString();
	//uint32 namePos = fName.find_last_of("/\\");
	//uint32 extPos = fName.find_last_of(".");

	//if (namePos == nsstring::npos || extPos == nsstring::npos)
	//	return;

	//nstexture * tex = texManager->loadFromFile(fName, false);
	//normalLoaded = true;


	//if (tex == NULL)
	//{
	//	tex = texManager->get(nstex_manager::nameFromFilename(fName));
	//	normalLoaded = false;
	//}

	//if (tex != NULL)
	//{
	//	if (normalLoaded)
	//		mEngine->getManager<nstex_manager>()->remove(mTempMat->getMapTextureID(nsmaterial::Normal));

	//	mTempMat->setMapTextureID(nsmaterial::Normal, tex->getResourceID(), true);
	//}
	//else
	//{
	//	QMessageBox mb(QMessageBox::Warning, "Filename Error", "Could not load normal texture - make sure file exists and filename in box is correct", QMessageBox::NoButton, this);
	//	mb.exec();
	//}
}

void NewTileDialog::onDiffLEEdit(QString pNewText)
{
	//nstex_manager * texManager = mEngine->getManager<nstex_manager>();

	//if (mTempMat == NULL)
	//	return;

	//nsstring fName = pNewText.toStdString();
	//uint32 namePos = fName.find_last_of("/\\");
	//uint32 extPos = fName.find_last_of(".");

	//if (namePos == nsstring::npos || extPos == nsstring::npos)
	//	return;

	//nstexture * tex = texManager->loadFromFile(fName, false);
	//diffuseLoaded = true;


	//if (tex == NULL)
	//{
	//	tex = texManager->get(nstex_manager::nameFromFilename(fName));
	//	diffuseLoaded = false;
	//}

	//if (tex != NULL)
	//{
	//	if (diffuseLoaded)
	//		mEngine->getManager<nstex_manager>()->remove(mTempMat->getMapTextureID(nsmaterial::Diffuse));

	//	mTempMat->setMapTextureID(nsmaterial::Diffuse, tex->getResourceID(), true);
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
	//nsmat_manager * mats = mEngine->getManager<nsmat_manager>();
	//nstex_manager * textures = mEngine->getManager<nstex_manager>();

	//if (diffuseLoaded)
	//	textures->remove(mTempMat->getMapTextureID(nsmaterial::Diffuse));

	//if (normalLoaded)
	//	textures->remove(mTempMat->getMapTextureID(nsmaterial::Normal));

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

	//QString end = QString(mEngine->getResourceDirectory().c_str()) + QString(mEngine->getManager<nstex_manager>()->getLocalDirectory().c_str()) + "Icons/" + subDir;

	//end += mUI.mNameLE->text() + "Icon.png";

	//mGenFile = end;
	//p.save(mGenFile);

	//mUI.mIconFileLE->setText("");

	//mUI.mIconFileLE->setText(mGenFile);
}
