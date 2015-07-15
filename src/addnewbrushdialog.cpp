#include <addnewbrushdialog.h>
#include <qgraphicsscene.h>
#include <nsglobal.h>
#include <qgraphicsitem.h>
#include <nsengine.h>
#include <nsselectionsystem.h>
#include <qstyle.h>
#include <nsselcomp.h>
#include <nsscenemanager.h>
#include <nsscene.h>
#include <nstformcomp.h>
#include <nstilegrid.h>
#include <nstexmanager.h>
#include <nsentitymanager.h>
#include <nstilebrushcomp.h>
#include <nsoccupycomp.h>
#include <nsinputcomp.h>
#include <nsselcomp.h>
#include <nstilecomp.h>
#include <toolkit.h>
#include <nsmeshmanager.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <nsfileos.h>
#include <nspluginmanager.h>

AddNewBrushDialog::AddNewBrushDialog(QWidget * parent):
QDialog(parent),	
mScene(new QGraphicsScene(this)),
mNormX(0.0f),
mNormY(0.0f)
{

}

AddNewBrushDialog::~AddNewBrushDialog()
{

}

void AddNewBrushDialog::init(Toolkit * pTK)
{
	mTK = pTK;
	mUI.setupUi(this);
	mUI.mBrushView->setScene(mScene);
	QObject::connect(this, SIGNAL(setCurrentBrush(NSEntity *)), mTK, SLOT(onSetCurrentBrush(NSEntity *)));
	drawPolygons();
}

void AddNewBrushDialog::drawPolygons()
{
	QRectF rect;
	QSizeF s = mUI.mBrushView->size();

	mNormX = s.width() / 22.0f;
	mNormY = s.height() / 22.0f;

	rect.setTopLeft(QPointF(-s.width()/2.0f, s.height()/2.0f)); rect.setTopRight(QPointF(s.width()/2.0f-2, s.height()/2.0f));
	rect.setBottomRight(QPointF(-s.width() / 2.0f, -s.height() / 2.0f)); rect.setBottomRight(QPointF(s.width() / 2.0f-2, -s.height() / 2.0f));
	mUI.mBrushView->setSceneRect(rect);
	mUI.mBrushView->setStyleSheet("background:transparent");
	
	QPolygonF p;
	p << QPointF(-X_GRID*mNormX, 0.5f*mNormY) << QPointF(0.0f, mNormY) << QPointF(X_GRID * mNormX, 0.5f*mNormY)
		<< QPointF(X_GRID*mNormX, -0.5f*mNormY) << QPointF(0.0f, -mNormY) << QPointF(-X_GRID*mNormX, -0.5f*mNormY) << QPointF(-X_GRID*mNormX, 0.5f*mNormY);
	
	for (nsint i = -5; i < 6; ++i)
	{
		if (i % 2 != 0)
		{
			for (nsint j = -5; j < 5; ++j)
			{
				HexTileItem * item = new HexTileItem();
				item->setPolygon(p);
				item->setFlags(QGraphicsItem::ItemIsSelectable);
				item->setBrush(QBrush());
				item->setEnginePos(ivec2(j, -i));
				item->setPos(j * X_GRID * 2 * mNormX + X_GRID*mNormX, i*Y_GRID*mNormY);
				mScene->addItem(item);
			}
		}
		else
		{
			for (nsint j = -5; j < 6; ++j)
			{
				HexTileItem * item = new HexTileItem();
				item->setPolygon(p);
				item->setFlags(QGraphicsItem::ItemIsSelectable);
				item->setBrush(QBrush());
				item->setEnginePos(ivec2(j, -i));
				item->setPos(j * X_GRID * 2 * mNormX, i*Y_GRID*mNormY);
				mScene->addItem(item);
			}
		}
	}
	
}

void AddNewBrushDialog::setFromEngineSelection()
{
	uivec3 foc = nsengine.system<NSSelectionSystem>()->center();
	NSScene * scene = nsengine.currentScene();

	if (scene == NULL)
		return;

	NSEntity * ent = scene->entity(foc.xy());
	if (ent == NULL)
		return;

	NSSelComp * selComp = ent->get<NSSelComp>();
	NSTFormComp * tComp = ent->get<NSTFormComp>();
	if (selComp == NULL || tComp == NULL)
		return;

	ivec3 centerPos = NSTileGrid::grid(tComp->wpos(foc.y));

	auto selIter = selComp->begin();
	while (selIter != selComp->end())
	{
		ivec3 currentPos = NSTileGrid::grid(tComp->wpos(*selIter)) - centerPos;

		nsfloat posX = currentPos.x * X_GRID * 2.0f * mNormX;
		nsfloat posY = -currentPos.y * Y_GRID * mNormY;
		if (currentPos.y % 2 != 0 && centerPos.y % 2 == 0)
			posX += X_GRID * mNormX;
		else if (currentPos.y % 2)
			posX -= X_GRID * mNormX;

		QGraphicsItem * item = mScene->itemAt(QPointF(posX, posY),QTransform());
		if (item != NULL)
			item->setSelected(true);

		++selIter;
	}
}

void AddNewBrushDialog::onAddBrush()
{
	nsstring name = mUI.mObjectNameLE->text().toStdString();
	nsstring iconFileName = mUI.mIconLE->text().toStdString();
	NSPlugin * act = nsengine.active();

	if (name.empty())
	{
		QMessageBox mb(QMessageBox::Warning, "Object Name Error", "Enter a name for the brush", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	if (act->contains<NSEntity>(name))
	{
		QMessageBox mb(QMessageBox::Warning, "Object Name Error", "An object already has this name - all game objects (including brushes) must have distinct names.", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	NSEntity * tileBrush = act->create<NSEntity>(name);

	if (tileBrush == NULL)
	{
		QMessageBox mb(QMessageBox::Warning, "Object Creation Error", "An error has occured in trying to make this brush - check the engine log for details.", QMessageBox::NoButton, this);
		mb.exec();
		return;
	}

	NSTileBrushComp * tBComp = tileBrush->create<NSTileBrushComp>();
	NSRenderComp * rComp = tileBrush->create<NSRenderComp>();
	NSSelComp * sComp = tileBrush->create<NSSelComp>();
	NSInputComp * inComp = tileBrush->create<NSInputComp>();

	rComp->setMeshID(nsengine.resource<NSMesh>(ENGINE_PLUG, MESH_FULL_TILE)->fullid());
	sComp->setDefaultColor(fvec4(0.0f, 1.0f, 0.0f, 0.8f));
	inComp->add(DRAG_OBJECT_XY);
	inComp->add(DRAG_OBJECT_XZ);
	inComp->add(DRAG_OBJECT_YZ);
	inComp->add(INSERT_OBJECT);
	inComp->add(XZ_MOVE_END);
	inComp->add(YZ_MOVE_END);
	inComp->add(SHIFT_DONE);

	auto items = mScene->selectedItems();
	auto iter = items.begin();
	while (iter != items.end())
	{
		HexTileItem * curItem = (HexTileItem*)*iter;
		tBComp->add(curItem->getEnginePos());
		++iter;
	}

	if (mUI.mGenIconCB->isChecked())
		iconFileName = _generateIcon(name);

	if (!iconFileName.empty())
		tileBrush->setIconPath(iconFileName);

	emit setCurrentBrush(tileBrush);
	accept();
}

void AddNewBrushDialog::onIconBrowse()
{
	QString filename = QFileDialog::getOpenFileName(
		this,
		"Choose icon file...",
		nsengine.importdir().c_str(),
		"Image Files (*.png *.jpg *.bmp)"
		);

	if (!filename.isEmpty())
		mUI.mIconLE->setText(filename);
}

void AddNewBrushDialog::onGenerateIconCB(bool pChecked)
{
	mUI.mIconBrowseBtn->setEnabled(!pChecked);
	mUI.mIconLE->setEnabled(!pChecked);
}

nsstring AddNewBrushDialog::_generateIcon(const nsstring & pObjName,const nsstring & pSubdir)
{
	auto items = mScene->items();
	auto iter = items.begin();
	while (iter != items.end())
	{
		if (!(*iter)->isSelected())
			mScene->removeItem(*iter);
		++iter;
	}

	QRect r = mScene->itemsBoundingRect().toRect();
	r.moveCenter(mUI.mBrushView->rect().center() + r.center());
	r.setWidth(r.width() + 5);

	QPixmap pixMap = QPixmap::grabWidget(mUI.mBrushView, r);
	
	nsstring savePath = nsengine.active()->resourceDirectory() + nsengine.active()->manager<NSTexManager>()->resourceDirectory() + "icons/";
	nsfileio::create_dir(savePath);
	nsfileio::create_dir(savePath + pSubdir);
	nsstring saveName = savePath + pSubdir + pObjName + ".png";
	pixMap.save(saveName.c_str());
	return saveName;
}


HexTileItem::HexTileItem(QGraphicsItem * pParent) :
QGraphicsPolygonItem(pParent)
{
}

HexTileItem::~HexTileItem()
{

}

void HexTileItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QStyleOptionGraphicsItem myOption(*option);
	myOption.state &= ~QStyle::State_Selected;

	if (isSelected())
		setBrush(QBrush(QColor(255, 200, 200)));
	else
		setBrush(QBrush(QColor(255, 255, 255)));

	if (scenePos() == QPointF(0.0f, 0.0f))
	{
		setSelected(true);
		setBrush(QBrush(QColor(255, 0, 0)));
	}

	QGraphicsPolygonItem::paint(painter, &myOption, widget);
}

void HexTileItem::setEnginePos(const ivec2 & pPos)
{
	mEnginePos = pPos;
}

const ivec2 & HexTileItem::getEnginePos()
{
	return mEnginePos;
}
