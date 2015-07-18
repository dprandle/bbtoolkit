#include <entityeditordialog.h>
#include <toolkit.h>
#include <nsengine.h>
#include <nsentity.h>
#include <nstexmanager.h>
#include <componentwidgets.h>
#include <nsanimcomp.h>
#include <nscamcomp.h>
#include <nsinputcomp.h>
#include <nslightcomp.h>
#include <nsoccupycomp.h>
#include <nsparticlecomp.h>
#include <nsrendercomp.h>
#include <nsselcomp.h>
#include <nstformcomp.h>
#include <nstilebrushcomp.h>
#include <nstilecomp.h>
#include <ui_selectresdialog.h>
#include <nsglobal.h>
#include <qtimer.h>

EntityEditorDialog::EntityEditorDialog(QWidget * parent) :
QDialog(parent),
mTK(NULL),
mEnt(NULL)
{}

EntityEditorDialog::~EntityEditorDialog()
{

}

void EntityEditorDialog::clear()
{
	mUI.mCompLW->clear();
	mUI.mNameLE->clear();
	mUI.mIconLE->clear();
}

NSEntity * EntityEditorDialog::entity()
{
	return mEnt;
}

void EntityEditorDialog::init(Toolkit * pTK)
{
	mTK = pTK;
	mUI.setupUi(this);
	mUI.mPreview->init(mTK);

	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSAnimComp))).c_str()] = Animation;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSCamComp))).c_str()] = Camera;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSInputComp))).c_str()] = Input;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSLightComp))).c_str()] = Light;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSOccupyComp))).c_str()] = Occupy;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSParticleComp))).c_str()] = Particle;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSRenderComp))).c_str()] = Render;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSSelComp))).c_str()] = Selection;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSTFormComp))).c_str()] = Transform;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSTileBrushComp))).c_str()] = Tilebrush;
	mTypeToIndex[nsengine.guid(std::type_index(typeid(NSTileComp))).c_str()] = Tile;
	
	// setup component widgets
	mUI.mCompW->insertWidget(None,new QWidget());
	mUI.mCompW->insertWidget(Animation, new AnimCompWidget());
	mUI.mCompW->insertWidget(Camera, new CamCompWidget());
	mUI.mCompW->insertWidget(Input, new InputCompWidget());
	mUI.mCompW->insertWidget(Light, new LightCompWidget());
	mUI.mCompW->insertWidget(Occupy, new OccupyCompWidget());
	mUI.mCompW->insertWidget(Particle, new ParticleCompWidget());
	mUI.mCompW->insertWidget(Render, new RenderCompWidget());
	mUI.mCompW->insertWidget(Selection, new SelCompWidget());
	mUI.mCompW->insertWidget(Transform, new TransformCompWidget());
	mUI.mCompW->insertWidget(Tilebrush, new TilebrushCompWidget());
	mUI.mCompW->insertWidget(Tile, new TileCompWidget());

	// Set up parameters that are true for all widgets
	for (unsigned int i = 0; i <= Tile; ++i)
	{
		mUI.mCompW->widget(i)->setMinimumSize(QSize(200, 200));
		if (i != 0)
			((CompWidget*)mUI.mCompW->widget(i))->init(mTK);
	}

	QTimer * t = new QTimer();
	connect(t, SIGNAL(timeout()), this, SLOT(onIdle()));
	t->start(0);
}

void EntityEditorDialog::onIdle()
{
	mTK->mapView()->update();
}

void EntityEditorDialog::onChooseIcon()
{

}

void EntityEditorDialog::setEntity(NSEntity * pEnt)
{
	if (pEnt == NULL)
		return;
	clear();
	mEnt = pEnt;
	
	mUI.mNameLE->setText(mEnt->name().c_str());

	auto compIter = mEnt->begin();
	while (compIter != mEnt->end())
	{
		nsstring guid = nsengine.guid(compIter->first);
		mUI.mCompLW->addItem(guid.c_str());
		++compIter;
	}

	for (int widgetI = 1; widgetI <= Tile; ++widgetI)
	{
		CompWidget * c = (CompWidget*)mUI.mCompW->widget(widgetI);
		c->setEntity(mEnt);
	}
}

void EntityEditorDialog::onRemoveComp()
{
	auto items = mUI.mCompLW->selectedItems();
	if (items.isEmpty())
		return;
	auto item = items.first();
	mTK->mapView()->makeCurrent();
	mEnt->del(item->text().toStdString());
	setEntity(mEnt);
	mTK->mapView()->update();
}

void EntityEditorDialog::onPrevDefault()
{

}

void EntityEditorDialog::onOkay()
{

}

void EntityEditorDialog::onNameChange()
{

}

void EntityEditorDialog::onGenerateIcon()
{

}

void EntityEditorDialog::onDiffuseChange(int pVal)
{

}

void EntityEditorDialog::onCompItemPressed(QListWidgetItem * pItem)
{
	CompWidgetIndex index = None;
	auto iter = mTypeToIndex.find(pItem->text());
	if (iter != mTypeToIndex.end())
		index = iter.value();
	mUI.mCompW->setCurrentIndex(index);
}

void EntityEditorDialog::onColorChange()
{

}

void EntityEditorDialog::onCancel()
{

}

void EntityEditorDialog::onAmbientChange(int pVal)
{

}

void EntityEditorDialog::onAddComp()
{

	QDialog addCompD(this);
	Ui::SelectResDialog ui;
	ui.setupUi(&addCompD);
	ui.mListWidget->addItem(ANIM_COMP_TYPESTRING);
	ui.mListWidget->addItem(CAM_COMP_TYPESTRING);
	ui.mListWidget->addItem(INPUT_COMP_TYPESTRING);
	ui.mListWidget->addItem(LIGHT_COMP_TYPESTRING);
	ui.mListWidget->addItem(OCCUPY_COMP_TYPESTRING);
	ui.mListWidget->addItem(RENDER_COMP_TYPESTRING);
	ui.mListWidget->addItem(PARTICLE_COMP_TYPESTRING);
	ui.mListWidget->addItem(SEL_COMP_TYPESTRING);
	ui.mListWidget->addItem(TILEBRUSH_COMP_TYPESTRING);
	ui.mListWidget->addItem(TILE_COMP_TYPESTRING);

	addCompD.setWindowTitle("Choose Component");

	if (addCompD.exec() == QDialog::Accepted)
	{
		auto lwitems = ui.mListWidget->selectedItems();
		if (lwitems.isEmpty())
			return;
		auto lwitem = lwitems.first();
	}
}
