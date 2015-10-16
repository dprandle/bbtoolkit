#include <tileview.h>
#include <newtiledialog.h>
#include <nsengine.h>
#include <nsentity.h>
#include <nsentitymanager.h>
#include <nsbuildsystem.h>
#include <toolkit.h>
#include <nstilecomp.h>
#include <nspluginmanager.h>
#include <nstilebrushcomp.h>
#include <qevent.h>

TileView::TileView(QWidget * parent):
mEntD(NULL),
mTK(NULL),
QMainWindow(parent)
{}

TileView::~TileView()
{

}

void TileView::init(Toolkit * pTK, EntityEditorDialog * pEntityDialogEditor)
{
	mTK = pTK;
	mEntD = pEntityDialogEditor;
	mUI.setupUi(this);
	mUI.mListWidget->init(mTK);
}

void TileView::onActionNew()
{
	NewTileDialog newTileDialog(parentWidget()->parentWidget());
	newTileDialog.init(mTK);
	NSPlugin * actplg = nsengine.active();

	if (newTileDialog.exec() == QDialog::Accepted)
	{
		QListWidgetItem * item = new QListWidgetItem(newTileDialog.getEntityName());
		NSEntity * ent = actplg->get<NSEntity>(item->text().toStdString());
		if (ent == NULL)
			return;
		
		if (!ent->iconPath().empty())
			item->setIcon(QIcon(ent->iconPath().c_str()));
		else
			item->setIcon(QIcon(":/TileIcons/Resources/defaulthexicon.png"));

		mUI.mListWidget->addItem(item);
	}
}

void TileView::onActionDelete()
{

}

void TileView::onActionEdit()
{

}

void TileView::refresh()
{
	reset();

	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);
		auto iter = plg->manager<NSEntityManager>()->begin();
		while (iter != plg->manager<NSEntityManager>()->end())
		{
			NSEntity * ent = (NSEntity*)iter->second;
			if (ent->has<NSTileComp>() && !ent->has<NSTileBrushComp>())
			{
				QListWidgetItem * item = new QListWidgetItem();

				// Use resource name as the name shown in window
				item->setText(ent->name().c_str());

				// Store the resource id and plug id for easy lookup later
				item->setData(VIEW_WIDGET_ITEM_PLUG, ent->plugid());
				item->setData(VIEW_WIDGET_ITEM_ENT, ent->id());
				
				if (!ent->iconPath().empty())
					item->setIcon(QIcon(ent->iconPath().c_str()));
				else
					item->setIcon(QIcon(":/TileIcons/Resources/defaulthexicon.png"));

				mUI.mListWidget->addItem(item);
			}
			++iter;
		}
		++plugiter;
	}
}

void TileView::onSelectionChanged()
{
	NSBuildSystem * buildSys = nsengine.system<NSBuildSystem>();
	QListWidgetItem * item = NULL;

	auto items = mUI.mListWidget->selectedItems();
	if (!items.isEmpty())
		item = items[0];

	if (item == NULL)
	{
		nsengine.system<NSBuildSystem>()->setBuildEnt(NULL);
		return;
	}

	uivec2 itid(item->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), item->data(VIEW_WIDGET_ITEM_ENT).toUInt());
	NSEntity * ent = nsengine.resource<NSEntity>(itid);
	nsengine.system<NSBuildSystem>()->setBuildEnt(ent);

	//if (buildSys->isEnabled() && buildSys->getBrushMode() == NSBuildSystem::Tile)
	//	((Toolkit*)parentWidget()->parentWidget())->getMapView()->setFocus();
}

void TileView::reset()
{
	mUI.mListWidget->clear();
}

Ui_TileView * TileView::getUI()
{
	return &mUI;
}