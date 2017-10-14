
#include <entityeditordialog.h>
#include <toolkit.h>
#include <nsengine.h>
#include <nsentity.h>
#include <nstex_manager.h>
#include <componentwidgets.h>
#include <nsanim_comp.h>
#include <nscam_comp.h>
#include <nslight_comp.h>
#include <nsoccupy_comp.h>
#include <nsparticle_comp.h>
#include <nsrender_comp.h>
#include <nssel_comp.h>
#include <nstform_comp.h>
#include <nstile_brush_comp.h>
#include <nstile_comp.h>
#include <ui_select_res_dialog.h>
#include <qtimer.h>

EntityEditorDialog::EntityEditorDialog(QWidget * parent) :
QDialog(parent),
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

nsentity * EntityEditorDialog::entity()
{
    return mEnt;
}

void EntityEditorDialog::init()
{
	mUI.setupUi(this);
    mUI.mPreview->init();

	mTypeToIndex[nse.guid(std::type_index(typeid(nsanim_comp))).c_str()] = Animation;
	mTypeToIndex[nse.guid(std::type_index(typeid(nscam_comp))).c_str()] = Camera;
	mTypeToIndex[nse.guid(std::type_index(typeid(nslight_comp))).c_str()] = Light;
	mTypeToIndex[nse.guid(std::type_index(typeid(nsoccupy_comp))).c_str()] = Occupy;
	mTypeToIndex[nse.guid(std::type_index(typeid(nsparticle_comp))).c_str()] = Particle;
	mTypeToIndex[nse.guid(std::type_index(typeid(nsrender_comp))).c_str()] = Render;
	mTypeToIndex[nse.guid(std::type_index(typeid(nssel_comp))).c_str()] = Selection;
	mTypeToIndex[nse.guid(std::type_index(typeid(nstform_comp))).c_str()] = Transform;
	mTypeToIndex[nse.guid(std::type_index(typeid(nstile_brush_comp))).c_str()] = Tilebrush;
	mTypeToIndex[nse.guid(std::type_index(typeid(nstile_comp))).c_str()] = Tile;
	
	// setup component widgets
	mUI.mCompW->insertWidget(None,new QWidget());
	mUI.mCompW->insertWidget(Animation, new AnimCompWidget());
	mUI.mCompW->insertWidget(Camera, new CamCompWidget());
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
            ((CompWidget*)mUI.mCompW->widget(i))->init();
	}

	QTimer * t = new QTimer();
	connect(t, SIGNAL(timeout()), this, SLOT(onIdle()));
	t->start(0);
}

void EntityEditorDialog::onIdle()
{
    bbtk.map()->update();
}

void EntityEditorDialog::onChooseIcon()
{

}

void EntityEditorDialog::setEntity(nsentity * pEnt)
{
	if (pEnt == NULL)
		return;
	clear();
	mEnt = pEnt;
	
	mUI.mNameLE->setText(mEnt->name().c_str());

	auto compIter = mEnt->begin();
	while (compIter != mEnt->end())
	{
		nsstring guid = nse.guid(compIter->first);
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
    bbtk.map()->makeCurrent();
    mEnt->destroy(item->text().toStdString());
	setEntity(mEnt);
    bbtk.map()->update();
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
    Ui::select_res_dialog ui;
	ui.setupUi(&addCompD);
    ui.m_lw->addItem(ANIM_COMP_TYPESTRING);
    ui.m_lw->addItem(CAM_COMP_TYPESTRING);
    ui.m_lw->addItem(LIGHT_COMP_TYPESTRING);
    ui.m_lw->addItem(OCCUPY_COMP_TYPESTRING);
    ui.m_lw->addItem(RENDER_COMP_TYPESTRING);
    ui.m_lw->addItem(PARTICLE_COMP_TYPESTRING);
    ui.m_lw->addItem(SEL_COMP_TYPESTRING);
    ui.m_lw->addItem(TILEBRUSH_COMP_TYPESTRING);
    ui.m_lw->addItem(TILE_COMP_TYPESTRING);

	addCompD.setWindowTitle("Choose Component");

	if (addCompD.exec() == QDialog::Accepted)
	{
        auto lwitems = ui.m_lw->selectedItems();
		if (lwitems.isEmpty())
			return;
		auto lwitem = lwitems.first();
	}
}
