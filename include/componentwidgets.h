#ifndef NSCOMPONENTWIDGETS_H
#define NSCOMPONENTWIDGETS_H

#include <qwidget.h>
#include <ui_animcompwidget.h>
#include <ui_camcompwidget.h>
#include <ui_inputcompwidget.h>
#include <ui_lightcompwidget.h>
#include <ui_occupycompwidget.h>
#include <ui_particlecompwidget.h>
#include <ui_rendercompwidget.h>
#include <ui_selcompwidget.h>
#include <ui_transformcompwidget.h>
#include <ui_tilebrushcompwidget.h>
#include <ui_tilecompwidget.h>

class NSEntity;
class NSEngine;
class Toolkit;

class CompWidget : public QWidget
{
	Q_OBJECT
public:
	CompWidget(QWidget * pParent = NULL);
	virtual void init(Toolkit * pTK);
	virtual void setEntity(NSEntity * pEnt);
	NSEntity * getEntity();

protected:
	NSEntity * mEnt;
	Toolkit * mTK;
};

class AnimCompWidget : public CompWidget
{
	Q_OBJECT
public:
	AnimCompWidget(QWidget * pParent = NULL);
	~AnimCompWidget();

private:
	Ui::AnimCompWidget mUI;

};

class CamCompWidget : public CompWidget
{
	Q_OBJECT
public:
	CamCompWidget(QWidget * pParent = NULL);
	~CamCompWidget();

	void setEntity(NSEntity * pEnt);

	public slots:

	void onChangeSpeed(double);

private:
	Ui::CamCompWidget mUI;
};

class InputCompWidget : public CompWidget
{
	Q_OBJECT
public:
	InputCompWidget(QWidget * pParent = NULL);
	~InputCompWidget();

private:
	Ui::InputCompWidget mUI;
};

class LightCompWidget : public CompWidget
{
	Q_OBJECT
public:
	LightCompWidget(QWidget * pParent = NULL);
	~LightCompWidget();

	void setEntity(NSEntity * pEnt);

	public slots:

	void onAmbientChange(int);
	void onColorChange();
	void onCutoffChange(int);
	void onDiffuseChange(int);
	void onDistanceChange(int);
	void onDistanceChangeSB(double);
	void onFalloffChange(QString);
	void onLightTypeChange(QString);
	void onRadiusChange(double);
	void onShadowDarknessChange(int);
	void onShadowSamplesChange(int);
	void onCastShadowChange(bool);

private:
	void _setColorStyle(const QColor & pCol);
	Ui::LightCompWidget mUI;
};

class OccupyCompWidget : public CompWidget
{
	Q_OBJECT
public:
	OccupyCompWidget(QWidget * pParent = NULL);
	~OccupyCompWidget();

private:
	Ui::OccupyCompWidget mUI;
};

class ParticleCompWidget : public CompWidget
{
	Q_OBJECT
public:
	ParticleCompWidget(QWidget * pParent = NULL);
	~ParticleCompWidget();

	void setEntity(NSEntity * pEnt);

	public slots:

	void onChooseMat();
	void onChooseShader();
	void onChooseRandTex();
	void onChangeMaxParticle();
	void onChangeEmissionRate();
	void onChangeAngVel();
	void onChangeLifetime();
	void onChangeStartHeight();
	void onChangeStartWidth();
	void onToggleLoop(bool);
	void onToggleSim(bool);
	void onAddForce();
	void onDelForce();
	void onAddKeyframe();
	void onDelKeyframe();
	void onForceItemChange(QTableWidgetItem*);
	void onKeyframeItemChange(QTableWidgetItem*);
	void onChangeEmitterShape(int);
	void onChangeEmitterSize();
	void onChangeMotionType(int);
	void onToggleInterpolate(bool);
	void onToggleGlobalTime(bool);
	void onToggleInterpolateVisual(bool);
	void onToggleGlobalTimeVisual(bool);
	void onChangeInitVel();
	void onChangeBlendMode(int);

private:
	Ui::ParticleCompWidget mUI;
};

class RenderCompWidget : public CompWidget
{
	Q_OBJECT
public:
	RenderCompWidget(QWidget * pParent = NULL);
	~RenderCompWidget();

	void setEntity(NSEntity * pEnt);

	public slots:

	void onCastShadowsChange(bool);
	void onChooseMat();
	void onChooseMesh();
	void onClearMat();
	void onSelectionChange();

private:
	Ui::RenderCompWidget mUI;
};

class SelCompWidget : public CompWidget
{
	Q_OBJECT
public:
	SelCompWidget(QWidget * pParent = NULL);
	~SelCompWidget();

	void setEntity(NSEntity * pEnt);

	public slots:
	
	void onAlphaChange(double);
	void onChangeColor();
	
private:
	void _setColorStyle(const QColor & pCol);
	Ui::SelCompWidget mUI;
};

class TransformCompWidget : public CompWidget
{
	Q_OBJECT
public:
	TransformCompWidget(QWidget * pParent = NULL);
	~TransformCompWidget();

private:
	Ui::TransformCompWidget mUI;
};

class TilebrushCompWidget : public CompWidget
{
	Q_OBJECT
public:
	TilebrushCompWidget(QWidget * pParent = NULL);
	~TilebrushCompWidget();

private:
	Ui::TilebrushCompWidget mUI;
};

class TileCompWidget : public CompWidget
{
	Q_OBJECT
public:
	TileCompWidget(QWidget * pParent = NULL);
	~TileCompWidget();

private:
	Ui::TileCompWidget mUI;
};

#endif