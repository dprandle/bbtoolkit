#ifndef ADDNEWBRUSHDIALOG_H
#define ADDNEWBRUSHDIALOG_H

// Inlcudes
#include <ui_addnewbrushdialog.h>
#include <nsmath.h>

class NSEngine;
class Toolkit;
class QGraphicsScene;

class AddNewBrushDialog : public QDialog
{
	Q_OBJECT
public:
	AddNewBrushDialog(QWidget * parent = NULL);
	~AddNewBrushDialog();

	void init(Toolkit * pTK);
	void drawPolygons();
	void setFromEngineSelection();

	public slots:

	void onAddBrush();
	void onIconBrowse();
	void onGenerateIconCB(bool);

	signals:
	void setCurrentBrush(NSEntity *);

private:
	nsstring _generateIcon(const nsstring & pObjName, const nsstring & pSubdir="");

	Ui_AddNewBrushDialog mUI;
	QGraphicsScene * mScene;
	Toolkit * mTK;
	float mNormX;
	float mNormY;
};
#endif
#ifndef HEXTILEITEM_H
#define HEXTILEITEM_H
#include <qgraphicsitem.h>

class HexTileItem : public QGraphicsPolygonItem
{
public:
	HexTileItem(QGraphicsItem * pParent=NULL);
	~HexTileItem();

	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	void setEnginePos(const ivec2 & pPos);
	const ivec2 & getEnginePos();

private:
	ivec2 mEnginePos;
};

#endif