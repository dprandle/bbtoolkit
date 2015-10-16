#include <objectview.h>
#include <nsengine.h>
#include <nsentity.h>
#include <nsentitymanager.h>
#include <toolkit.h>
#include <nstilecomp.h>
#include <nspluginmanager.h>
#include <nsentity.h>
#include <entityeditordialog.h>


ObjectView::ObjectView(QWidget * parent) :
mTK(NULL),
mEntD(NULL),
QMainWindow(parent)
{}

ObjectView::~ObjectView()
{

}

void ObjectView::init(Toolkit * pTK, EntityEditorDialog * pEntityEditorDialog)
{
	mTK = pTK;
	mEntD = pEntityEditorDialog;

	mUI.setupUi(this);
}

void ObjectView::refresh()
{
	mUI.mListWidget->clear();

	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);

		auto iter = plg->manager<NSEntityManager>()->begin();
		while (iter != plg->manager<NSEntityManager>()->end())
		{
			NSEntity * ent = (NSEntity*)iter->second;
			QListWidgetItem * item = new QListWidgetItem();
			item->setText(ent->name().c_str());
			if (!ent->iconPath().empty())
				item->setIcon(QIcon(ent->iconPath().c_str()));

			item->setData(VIEW_WIDGET_ITEM_PLUG, ent->plugid());
			item->setData(VIEW_WIDGET_ITEM_ENT, ent->id());

			mUI.mListWidget->addItem(item);
			++iter;
		}

		++plugiter;
	}
}

void ObjectView::onActionNew()
{
}

void ObjectView::onActionDelete()
{

}

void ObjectView::onActionEdit()
{
	auto items = mUI.mListWidget->selectedItems();
	if (items.isEmpty())
		return;

	auto item = items.first();
	uivec2 fid(item->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), item->data(VIEW_WIDGET_ITEM_ENT).toUInt());
	NSEntity * ent = nsengine.resource<NSEntity>(fid);
	mEntD->setEntity(ent);
	mEntD->show();
}

void ObjectView::onItemPressed(QListWidgetItem* pItem)
{
	uivec2 fid(pItem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), pItem->data(VIEW_WIDGET_ITEM_ENT).toUInt());
	NSEntity * ent = nsengine.resource<NSEntity>(fid);
	mEntD->setEntity(ent);
}