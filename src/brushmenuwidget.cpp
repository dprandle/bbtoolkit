#include <brushmenuwidget.h>
#include <qtoolbar.h>
#include <qmessagebox.h>
#include <addnewbrushdialog.h>
#include <nsengine.h>
#include <nsentitymanager.h>
#include <nstilebrushcomp.h>
#include <toolkit.h>
#include <nspluginmanager.h>
#include <nsbuildsystem.h>

BrushMenuWidget::BrushMenuWidget(QWidget * parent):
mTB(new QToolBar()),
QWidget(parent)
{

}

BrushMenuWidget::~BrushMenuWidget()
{

}

void BrushMenuWidget::init(Toolkit * pTK)
{
	mTK = pTK;
	mUI.setupUi(this);
	mUI.verticalLayout->addWidget(mTB);
	mTB->addAction(mUI.actionNewBrush);
	mTB->addAction(mUI.actionEditBrush);
	mTB->addAction(mUI.actionDeleteBrush);
	mTB->setLayoutDirection(Qt::RightToLeft);
}

void BrushMenuWidget::setSelectedItem(NSEntity * ent)
{
	auto fItems = mUI.mBrushedLW->findItems(QString(ent->name().c_str()), Qt::MatchCaseSensitive);
	for (nsint i = 0; i < fItems.size(); ++i)
	{
		auto curitem = fItems[i];
		if (uivec2(curitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), curitem->data(VIEW_WIDGET_ITEM_ENT).toUInt()) == ent->fullid())
			mUI.mBrushedLW->setItemSelected(curitem, true);
	}
}

NSEntity * BrushMenuWidget::selectedItem()
{
	auto items = mUI.mBrushedLW->selectedItems();
	QListWidgetItem * item = NULL;

	if (!items.isEmpty())
		item = items.first();

	if (item != NULL)
		return nsengine.resource<NSEntity>(uivec2(item->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), item->data(VIEW_WIDGET_ITEM_ENT).toUInt()));

	return NULL;
}

void BrushMenuWidget::setupLW()
{
	QListWidgetItem * curItem = mUI.mBrushedLW->currentItem();
	QString itemName;
	if (curItem != NULL)
		itemName = curItem->text();

	mUI.mBrushedLW->clear();


	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);

		NSEntityManager * ents = plg->manager<NSEntityManager>();

		auto iter = ents->begin();
		while (iter != ents->end())
		{
			NSEntity * curEnt = ents->get(iter->first);

			if (curEnt->has<NSTileBrushComp>())
			{
				QListWidgetItem * item = new QListWidgetItem(curEnt->name().c_str());
				item->setData(VIEW_WIDGET_ITEM_PLUG, curEnt->plugid());
				item->setData(VIEW_WIDGET_ITEM_ENT, curEnt->id());
				// make else case to give default icon in case no icon assigned
				if (!curEnt->iconPath().empty())
					item->setIcon(QIcon(curEnt->iconPath().c_str()));

				mUI.mBrushedLW->addItem(item);
			}
			++iter;
		}


		++plugiter;
	}

	// Set the selected brush back to whatever was selected before remaking the brush list
	auto fItems = mUI.mBrushedLW->findItems(itemName, Qt::MatchCaseSensitive);
	for (nsint i = 0; i < fItems.size(); ++i)
	{

	}




	if (!fItems.isEmpty() && fItems.first() != NULL)
	{
		mUI.mBrushedLW->setCurrentItem(fItems.first());
		mUI.mBrushedLW->setItemSelected(fItems.first(),true);
	}
	else if (mUI.mBrushedLW->count() > 0)
	{
		QListWidgetItem * item = mUI.mBrushedLW->item(0);
		mUI.mBrushedLW->setCurrentItem(item);
		mUI.mBrushedLW->setItemSelected(item, true);
	}
	else
	{
		mUI.mBrushedLW->setCurrentItem(NULL);
	}
}

void BrushMenuWidget::onDeleteBrush()
{
	QMessageBox mb(mTK);
	mb.setText("Are you sure you want to delete the brush?");
	if (mb.exec() == QMessageBox::Accepted)
	{

	}
}

void BrushMenuWidget::onEditBrush()
{

}

void BrushMenuWidget::onNewBrush()
{
	AddNewBrushDialog brushD(mTK);
	brushD.init(mTK);
	if (brushD.exec() == QDialog::Accepted)
	{

	}
}

void BrushMenuWidget::onDoubleClick()
{
	emit brushDoubleClick();
}

void BrushMenuWidget::onItemChanged(QListWidgetItem* pCurrent)
{
	emit brushChange(pCurrent);
}
