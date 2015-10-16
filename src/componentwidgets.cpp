#include <componentwidgets.h>
#include <nsentity.h>
#include <nsanimcomp.h>
#include <nscamcomp.h>
#include <nsinputcomp.h>
#include <nslightcomp.h>
#include <nsoccupycomp.h>
#include <nsparticlecomp.h>
#include <nsshadermanager.h>
#include <nspluginmanager.h>
#include <nsrendercomp.h>
#include <nsselcomp.h>
#include <nstformcomp.h>
#include <nsmatmanager.h>
#include <nstilebrushcomp.h>
#include <nstexmanager.h>
#include <nstilecomp.h>
#include <qcolordialog.h>
#include <qcolor.h>
#include <toolkit.h>
#include <nsengine.h>
#include <nsmeshmanager.h>
#include <ui_selectresdialog.h>

CompWidget::CompWidget(QWidget * pParent) :
mEnt(NULL),
QWidget(pParent)
{}

void CompWidget::init(Toolkit * pTK)
{
	mTK = pTK;
}

void CompWidget::setEntity(NSEntity * pEnt)
{
	mEnt = pEnt;
}

NSEntity * CompWidget::getEntity()
{
	return mEnt;
}

AnimCompWidget::AnimCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

AnimCompWidget::~AnimCompWidget()
{}

CamCompWidget::CamCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

CamCompWidget::~CamCompWidget()
{}

void CamCompWidget::setEntity(NSEntity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;

	NSCamComp * camComp = mEnt->get<NSCamComp>();
	if (camComp == NULL)
		return;

	mUI.mSpeedSB->blockSignals(true);
	mUI.mSpeedSB->setValue(camComp->speed());
	mUI.mSpeedSB->blockSignals(false);
}

void CamCompWidget::onChangeSpeed(double pNewSpeed)
{
	if (mEnt == NULL)
		return;

	NSCamComp * camComp = mEnt->get<NSCamComp>();
	if (camComp == NULL)
		return;

	camComp->setSpeed(pNewSpeed);
	mTK->mapView()->update();
}

InputCompWidget::InputCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

InputCompWidget::~InputCompWidget()
{}

LightCompWidget::LightCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

LightCompWidget::~LightCompWidget()
{}

void LightCompWidget::setEntity(NSEntity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	
	mUI.mAmbientSldr->blockSignals(true);
	mUI.mAmbientSldr->setValue(lightComp->intensity().y*100);
	mUI.mAmbientSldr->blockSignals(false);

	mUI.mCutoffSldr->blockSignals(true);
	mUI.mCutoffSldr->setValue(lightComp->cutoff());
	mUI.mCutoffSldr->blockSignals(false);

	mUI.mDiffuseSldr->blockSignals(true);
	mUI.mDiffuseSldr->setValue(lightComp->intensity().x*100);
	mUI.mDiffuseSldr->blockSignals(false);

	mUI.mDistanceSB->blockSignals(true);
	mUI.mDistanceSB->setValue(lightComp->distance());
	mUI.mDistanceSB->blockSignals(false);

	mUI.mDistanceSldr->blockSignals(true);
	mUI.mDistanceSldr->setValue(lightComp->distance());
	mUI.mDistanceSldr->blockSignals(false);

	mUI.mLightTypeCB->blockSignals(true);
	mUI.mLightTypeCB->setCurrentIndex(lightComp->type());
	mUI.mLightTypeCB->blockSignals(false);

	mUI.mRadiusSB->blockSignals(true);
	mUI.mRadiusSB->setValue(lightComp->radius());
	mUI.mRadiusSB->blockSignals(false);

	mUI.mShadowSldr->blockSignals(true);
	mUI.mShadowSldr->setValue(lightComp->shadowDarkness()*100);
	mUI.mShadowSldr->blockSignals(false);

	mUI.mShadowSamplesSB->blockSignals(true);
	mUI.mShadowSamplesSB->setValue(lightComp->shadowSamples());
	mUI.mShadowSamplesSB->blockSignals(false);

	mUI.mCastsShadowsCB->blockSignals(true);
	mUI.mCastsShadowsCB->setChecked(lightComp->castShadows());
	mUI.mCastsShadowsCB->blockSignals(false);

	fvec3 col = lightComp->color();
	QColor prevC;
	prevC.setRedF(col.x); prevC.setGreenF(col.y); prevC.setBlueF(col.z);
	_setColorStyle(prevC);
}

void LightCompWidget::onAmbientChange(int pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setIntensity(lightComp->intensity().x, float(pVal)/100.0f, NSLightComp::Adjustment(mUI.mFalloffCB->currentIndex()));
	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onColorChange()
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	fvec3 col = lightComp->color();
	QColor prevC;
	prevC.setRedF(col.x); prevC.setGreenF(col.y); prevC.setBlueF(col.z);
	QColor c = QColorDialog::getColor(prevC);
	if (c.isValid())
	{
		fvec3 newCol(c.redF(), c.greenF(), c.blueF());
		lightComp->setColor(newCol);
		prevC = c;
	}
	_setColorStyle(prevC);
	mTK->mapView()->update();
}

void LightCompWidget::onCutoffChange(int pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setCutoff(float(pVal));
	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onDiffuseChange(int pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setIntensity(float(pVal)/100.0f, lightComp->intensity().y, NSLightComp::Adjustment(mUI.mFalloffCB->currentIndex()));
	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onDistanceChange(int pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setDistance(float(pVal), NSLightComp::Adjustment(mUI.mFalloffCB->currentIndex()));
	mUI.mDistanceSB->blockSignals(true);
	mUI.mDistanceSB->setValue(double(pVal));
	mUI.mDistanceSB->blockSignals(false);
	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onDistanceChangeSB(double pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setDistance(pVal, NSLightComp::Adjustment(mUI.mFalloffCB->currentIndex()));
	mUI.mDistanceSldr->blockSignals(true);
	mUI.mDistanceSldr->setValue(int(pVal));
	mUI.mDistanceSldr->blockSignals(false);
	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onFalloffChange(QString pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onLightTypeChange(QString pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	NSLightComp::LightType lType = NSLightComp::LightType(mUI.mLightTypeCB->findText(pVal));
	uivec2 ID;

	switch (lType)
	{
	case(NSLightComp::Directional) :
		ID = nsengine.resource<NSMesh>(ENGINE_PLUG,MESH_DIRLIGHT_BOUNDS)->fullid();
		break;
	case(NSLightComp::Point) :
		ID = nsengine.resource<NSMesh>(ENGINE_PLUG, MESH_POINTLIGHT_BOUNDS)->fullid();
		break;
	case(NSLightComp::Spot) :
		ID = nsengine.resource<NSMesh>(ENGINE_PLUG, MESH_SPOTLIGHT_BOUNDS)->fullid();
		break;
	}

	lightComp->setType(lType);
	lightComp->setMeshID(ID);
	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onRadiusChange(double pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setRadius(pVal);
	setEntity(mEnt);
	mTK->mapView()->update();
}

void LightCompWidget::onCastShadowChange(bool pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setCastShadows(pVal);
	mTK->mapView()->update();
}

void LightCompWidget::onShadowDarknessChange(int pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setShadowDarkness(float(pVal)/100.0f);
	mTK->mapView()->update();
}

void LightCompWidget::onShadowSamplesChange(int pVal)
{
	if (mEnt == NULL)
		return;

	NSLightComp * lightComp = mEnt->get<NSLightComp>();
	if (lightComp == NULL)
		return;

	lightComp->setShadowSamples(pVal);
	mTK->mapView()->update();
}

void LightCompWidget::_setColorStyle(const QColor & pCol)
{
	QString rgbcol = "rgb(" + QString::number(pCol.red()) + "," + QString::number(pCol.green()) + "," + QString::number(pCol.blue()) + ");";
	mUI.mColorFrame->setStyleSheet(
		"QFrame {\
		background-color: " + rgbcol +
		"border: none\
		}\
		QFrame:hover\
		{\
		background-color: rgb(200,200,200);\
		}");
}


OccupyCompWidget::OccupyCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

OccupyCompWidget::~OccupyCompWidget()
{}

ParticleCompWidget::ParticleCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

ParticleCompWidget::~ParticleCompWidget()
{}

void ParticleCompWidget::setEntity(NSEntity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;


	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	mUI.mKeyframeTblW->clearContents();
	mUI.mForceTblW->clearContents();
	mUI.mForceTblW->setMinimumHeight(40);
	mUI.mKeyframeTblW->setMinimumHeight(40);

	NSMaterial * mat = nsengine.resource<NSMaterial>(partComp->materialID());
	NSShader * shader = nsengine.resource<NSShader>(partComp->shaderID());
	NSTexture * tex = nsengine.resource<NSTexture>(partComp->randomTextureID());

	if (mat != NULL)
		mUI.mMaterialLE->setText(mat->name().c_str());
	else
		mUI.mMaterialLE->setText("None");

	if (shader != NULL)
		mUI.mShaderLE->setText(shader->name().c_str());
	else
		mUI.mShaderLE->setText("None");
	
	if (tex != NULL)
		mUI.mRandTexLE->setText(tex->name().c_str());
	else
		mUI.mRandTexLE->setText("None");

	mUI.mLoopCB->setChecked(partComp->looping());
	mUI.mSimCB->setChecked(partComp->simulating());

	mUI.mMaxParticlesSB->blockSignals(true);
	mUI.mMaxParticlesSB->setValue(partComp->maxParticles());
	mUI.mMaxParticlesSB->blockSignals(false);

	mUI.mEmissionSB->blockSignals(true);
	mUI.mEmissionSB->setValue(partComp->emissionRate());
	mUI.mEmissionSB->blockSignals(false);

	mUI.mAngVelSB->blockSignals(true);
	mUI.mAngVelSB->setValue(partComp->angularVelocity());
	mUI.mAngVelSB->blockSignals(false);

	mUI.mLifetimeDSB->blockSignals(true);
	mUI.mLifetimeDSB->setValue(float(partComp->lifetime())/1000.0f);
	mUI.mLifetimeDSB->blockSignals(false);

	mUI.mStartWidthDSB->blockSignals(true);
	mUI.mStartWidthDSB->setValue(partComp->startingSize().u);
	mUI.mStartWidthDSB->blockSignals(false);

	mUI.mStartHeightDSB->blockSignals(true);
	mUI.mStartHeightDSB->setValue(partComp->startingSize().v);
	mUI.mStartHeightDSB->blockSignals(false);

	mUI.mEmitComb->blockSignals(true);
	mUI.mEmitComb->setCurrentIndex(partComp->emitterShape());
	mUI.mEmitComb->blockSignals(false);

	fvec3 ss = partComp->emitterSize();
	mUI.mEmitXDSB->blockSignals(true);
	mUI.mEmitXDSB->setValue(ss.x);
	mUI.mEmitXDSB->blockSignals(false);

	mUI.mEmitYDSB->blockSignals(true);
	mUI.mEmitYDSB->setValue(ss.y);
	mUI.mEmitYDSB->blockSignals(false);

	mUI.mEmitZDSB->blockSignals(true);
	mUI.mEmitZDSB->setValue(ss.z);
	mUI.mEmitZDSB->blockSignals(false);

	mUI.mMotionComb->blockSignals(true);
	mUI.mMotionComb->setCurrentIndex(partComp->motionKeyType());
	mUI.mMotionComb->blockSignals(false);

	mUI.mInterpMotionCB->blockSignals(true);
	mUI.mInterpMotionCB->setChecked(partComp->motionKeyInterpolation());
	mUI.mInterpMotionCB->blockSignals(false);

	mUI.mGlobalTimeCB->blockSignals(true);
	mUI.mGlobalTimeCB->setChecked(partComp->motionGlobalTime());
	mUI.mGlobalTimeCB->blockSignals(false);

	mUI.mGlobalTimeVisualCB->blockSignals(true);
	mUI.mGlobalTimeVisualCB->setChecked(partComp->visualGlobalTime());
	mUI.mGlobalTimeVisualCB->blockSignals(false);

	mUI.mInterpolateVisualCB->blockSignals(true);
	mUI.mInterpolateVisualCB->setChecked(partComp->visualKeyInterpolation());
	mUI.mInterpolateVisualCB->blockSignals(false);

	fvec3 iv = partComp->initVelocityMult();
	mUI.mInitVelXDSB->blockSignals(true);
	mUI.mInitVelXDSB->setValue(iv.x);
	mUI.mInitVelXDSB->blockSignals(false);

	mUI.mInitVelYDSB->blockSignals(true);
	mUI.mInitVelYDSB->setValue(iv.y);
	mUI.mInitVelYDSB->blockSignals(false);

	mUI.mInitVelZDSB->blockSignals(true);
	mUI.mInitVelZDSB->setValue(iv.z);
	mUI.mInitVelZDSB->blockSignals(false);

	mUI.mBlendComb->blockSignals(true);
	mUI.mBlendComb->setCurrentIndex(partComp->blendMode());
	mUI.mBlendComb->blockSignals(false);



	mUI.mForceTblW->setRowCount(partComp->motionKeyCount());
	mUI.mKeyframeTblW->setRowCount(partComp->visualKeyCount());

	mUI.mForceTblW->blockSignals(true);
	auto iterF = partComp->beginMotionKey();
	int index = 0;
	while (iterF != partComp->endMotionKey())
	{
		QTableWidgetItem * item = new QTableWidgetItem();
		QTableWidgetItem * item2 = new QTableWidgetItem();
		QTableWidgetItem * item3 = new QTableWidgetItem();
		QTableWidgetItem * item4 = new QTableWidgetItem();
		
		float normTime = float(iterF->first) / float(partComp->maxMotionKeys());
		item->setData(0, normTime);
		item->setData(Qt::UserRole, normTime);

		item2->setData(0, iterF->second.x);
		item2->setData(Qt::UserRole, iterF->second.x);

		item3->setData(0, iterF->second.y);
		item3->setData(Qt::UserRole, iterF->second.y);

		item4->setData(0, iterF->second.z);
		item4->setData(Qt::UserRole, iterF->second.z);

		mUI.mForceTblW->setItem(index, 0, item);
		mUI.mForceTblW->setItem(index, 1, item2);
		mUI.mForceTblW->setItem(index, 2, item3);
		mUI.mForceTblW->setItem(index, 3, item4);
		mUI.mForceTblW->setMinimumHeight(mUI.mForceTblW->minimumHeight()+20);
		++index;
		++iterF;
	}
	mUI.mForceTblW->blockSignals(false);

	mUI.mKeyframeTblW->blockSignals(true);
	index = 0;
	auto iterR = partComp->beginVisualKey();
	while (iterR != partComp->endVisualKey())
	{
		QTableWidgetItem * item = new QTableWidgetItem();
		QTableWidgetItem * item2 = new QTableWidgetItem();
		QTableWidgetItem * item3 = new QTableWidgetItem();
		QTableWidgetItem * item4 = new QTableWidgetItem();

		float normTime = float(iterR->first) / float(partComp->maxVisualKeys());
		item->setData(0, normTime);
		item->setData(Qt::UserRole, normTime);

		item2->setData(0, iterR->second.x);
		item2->setData(Qt::UserRole, iterR->second.x);

		item3->setData(0, iterR->second.y);
		item3->setData(Qt::UserRole, iterR->second.y);

		item4->setData(0, iterR->second.z);
		item4->setData(Qt::UserRole, iterR->second.z);

		mUI.mKeyframeTblW->setItem(index, 0, item);
		mUI.mKeyframeTblW->setItem(index, 1, item2);
		mUI.mKeyframeTblW->setItem(index, 2, item3);
		mUI.mKeyframeTblW->setItem(index, 3, item4);
		mUI.mKeyframeTblW->setMinimumHeight(mUI.mKeyframeTblW->minimumHeight() + 20);
		++index;
		++iterR;
	}
	mUI.mKeyframeTblW->blockSignals(false);
}

void ParticleCompWidget::onChooseMat()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	QDialog selMatD(this);
	Ui::SelectResDialog ui;
	ui.setupUi(&selMatD);
	selMatD.setWindowTitle("Select Material");

	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);

		NSMatManager * mats = plg->manager<NSMatManager>();
		auto miter = mats->begin();
		while (miter != mats->end())
		{
			QListWidgetItem * toAdd = new QListWidgetItem(miter->second->name().c_str());
			toAdd->setData(VIEW_WIDGET_ITEM_PLUG, miter->second->plugid());
			toAdd->setData(VIEW_WIDGET_ITEM_ENT, miter->second->id());
			ui.mListWidget->addItem(toAdd);
			++miter;
		}

		++plugiter;
	}

	if (selMatD.exec() == QDialog::Accepted)
	{
		auto lwitems = ui.mListWidget->selectedItems();
		if (lwitems.isEmpty())
			return;
		auto lwitem = lwitems.first();

		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

		NSMaterial * mat = nsengine.resource<NSMaterial>(id);
		if (mat == NULL)
			return;

		partComp->setMaterialID(mat->fullid());
		setEntity(mEnt);
	}
}

void ParticleCompWidget::onChangeBlendMode(int pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->setBlendMode(NSParticleComp::BlendMode(pVal));
	setEntity(mEnt);
}

void ParticleCompWidget::onChooseShader()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	QDialog selShaderD(this);
	Ui::SelectResDialog ui;
	ui.setupUi(&selShaderD);
	selShaderD.setWindowTitle("Select Shader");

	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin *plg = nsengine.plugin(plugiter->first);
		NSShaderManager * shaders = plg->manager<NSShaderManager>();
		auto siter = shaders->begin();
		while (siter != shaders->end())
		{
			QListWidgetItem * lwitem = new QListWidgetItem(siter->second->name().c_str());
			lwitem->setData(VIEW_WIDGET_ITEM_PLUG, siter->second->plugid());
			lwitem->setData(VIEW_WIDGET_ITEM_ENT, siter->second->id());
			ui.mListWidget->addItem(lwitem);
			++siter;
		}
		++plugiter;
	}

	if (selShaderD.exec() == QDialog::Accepted)
	{
		auto lwitems = ui.mListWidget->selectedItems();
		if (lwitems.isEmpty())
			return;
		auto lwitem = lwitems.first();

		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());
		NSShader * shader = nsengine.resource<NSShader>(id);
		if (shader == NULL)
			return;

		partComp->setShaderID(shader->fullid());
		setEntity(mEnt);
	}
}

void ParticleCompWidget::onChooseRandTex()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	QDialog selTextureD(this);
	Ui::SelectResDialog ui;
	ui.setupUi(&selTextureD);
	selTextureD.setWindowTitle("Select Texture");


	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin *plg = nsengine.plugin(plugiter->first);
		NSTexManager * textures = plg->manager<NSTexManager>();
		auto txiter = textures->begin();
		while (txiter != textures->end())
		{
			NSTexture * tex = textures->get(txiter->first);
			QListWidgetItem * lwitem = new QListWidgetItem(txiter->second->name().c_str());
			lwitem->setData(VIEW_WIDGET_ITEM_PLUG, tex->plugid());
			lwitem->setData(VIEW_WIDGET_ITEM_ENT, tex->id());
			ui.mListWidget->addItem(lwitem);
			++txiter;
		}
		++plugiter;
	}
	if (selTextureD.exec() == QDialog::Accepted)
	{
		auto lwitems = ui.mListWidget->selectedItems();
		if (lwitems.isEmpty())
			return;
		auto lwitem = lwitems.first();
		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());
		NSTexture * tex = nsengine.resource<NSTexture>(id);
		if (tex == NULL)
			return;

		partComp->setRandTextureID(tex->fullid());
		setEntity(mEnt);
	}
}

void ParticleCompWidget::onChangeMaxParticle()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->setMaxParticles(mUI.mMaxParticlesSB->value());
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeEmissionRate()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->setEmissionRate(mUI.mEmissionSB->value());
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeAngVel()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->setAngularVelocity(mUI.mAngVelSB->value());
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeLifetime()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->setLifetime(nsuint(mUI.mLifetimeDSB->value()*1000.0f));
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeStartHeight()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	fvec2 size = partComp->startingSize();
	size.v = mUI.mStartHeightDSB->value();
	partComp->setStartingSize(size);
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeStartWidth()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	fvec2 size = partComp->startingSize();
	size.u = mUI.mStartWidthDSB->value();
	partComp->setStartingSize(size);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleLoop(bool pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->enableLooping(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleSim(bool pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->enableSimulation(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onAddForce()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mForceTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();

	while (partComp->hasMotionKey(time))
		time += 1 / float(partComp->maxMotionKeys());

	partComp->setMotionKey(time, fvec3());
	setEntity(mEnt);
}

void ParticleCompWidget::onDelForce()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mForceTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();
	partComp->removeMotionKey(time);

	setEntity(mEnt);
}

void ParticleCompWidget::onAddKeyframe()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mKeyframeTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();

	while (partComp->hasVisualKey(time))
		time += 1 / float(partComp->maxVisualKeys());

	partComp->setVisualKey(time, fvec3(1.0f, 1.0f, 1.0f));
	setEntity(mEnt);
}

void ParticleCompWidget::onDelKeyframe()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mKeyframeTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();
	partComp->removeVisualKey(time);

	setEntity(mEnt);
}

void ParticleCompWidget::onForceItemChange(QTableWidgetItem* pItem)
{
	if (mEnt == NULL || pItem == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	if (pItem->column() == 0)
	{
		float oldTime = pItem->data(Qt::UserRole).toFloat();
		float newTime = pItem->data(0).toFloat();

		fvec3 force = partComp->motionKeyAt(oldTime);
		partComp->removeMotionKey(oldTime);
		partComp->setMotionKey(newTime, force);
	}
	else
	{
		float time = mUI.mForceTblW->item(pItem->row(), 0)->data(0).toFloat();
		fvec3 force = partComp->motionKeyAt(time);
		if (pItem->column() == 1)
			force.x = pItem->data(0).toFloat();
		else if (pItem->column() == 2)
			force.y = pItem->data(0).toFloat();
		else
			force.z = pItem->data(0).toFloat();
		partComp->setMotionKey(time, force);
	}

	setEntity(mEnt);
}

void ParticleCompWidget::onKeyframeItemChange(QTableWidgetItem* pItem)
{
	if (mEnt == NULL || pItem == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	if (pItem->column() == 0)
	{
		float oldTime = pItem->data(Qt::UserRole).toFloat();
		float newTime = pItem->data(0).toFloat();

		fvec3 renKey = partComp->visualKeyAt(oldTime);
		partComp->removeVisualKey(oldTime);
		partComp->setVisualKey(newTime, renKey);
	}
	else
	{
		float time = mUI.mKeyframeTblW->item(pItem->row(), 0)->data(0).toFloat();
		fvec3 renKey = partComp->visualKeyAt(time);
		if (pItem->column() == 1)
			renKey.x = pItem->data(0).toFloat();
		else if (pItem->column() == 2)
			renKey.y = pItem->data(0).toFloat();
		else
			renKey.z = pItem->data(0).toFloat();
		partComp->setVisualKey(time, renKey);
	}

	setEntity(mEnt);
}

void ParticleCompWidget::onChangeEmitterShape(int pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->setEmitterShape(NSParticleComp::EmitterShape(pVal));
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeEmitterSize()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	fvec3 emitterSize;
	emitterSize.x = mUI.mEmitXDSB->value();
	emitterSize.y = mUI.mEmitYDSB->value();
	emitterSize.z = mUI.mEmitZDSB->value();
	partComp->setEmitterSize(emitterSize);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleInterpolateVisual(bool pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;
	partComp->enableVisualKeyInterpolation(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleGlobalTimeVisual(bool pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->enableVisualGlobalTime(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeInitVel()
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	fvec3 initVel;
	initVel.x = mUI.mInitVelXDSB->value();
	initVel.y = mUI.mInitVelYDSB->value();
	initVel.z = mUI.mInitVelZDSB->value();
	partComp->setInitVelocityMult(initVel);
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeMotionType(int pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->setMotionKeyType(NSParticleComp::MotionKeyType(pVal));
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleInterpolate(bool pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->enableMotionKeyInterpolation(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleGlobalTime(bool pVal)
{
	if (mEnt == NULL)
		return;

	NSParticleComp * partComp = mEnt->get<NSParticleComp>();
	if (partComp == NULL)
		return;

	partComp->enableMotionGlobalTime(pVal);
	setEntity(mEnt);
}



RenderCompWidget::RenderCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

RenderCompWidget::~RenderCompWidget()
{}

void RenderCompWidget::setEntity(NSEntity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;

	NSRenderComp * renComp = mEnt->get<NSRenderComp>();
	if (renComp == NULL)
		return;

	mUI.mMaterialsTW->clear();

	mUI.mCastsShadowsCB->blockSignals(true);
	mUI.mCastsShadowsCB->setChecked(renComp->castShadow());
	mUI.mCastsShadowsCB->blockSignals(false);

	NSMesh * mesh = nsengine.resource<NSMesh>(renComp->meshID());

	mUI.mMeshNameLE->setText(mesh->name().c_str());
	
	
	for (nsuint i = 0; i < mesh->count(); ++i)
	{
		NSMaterial * mat = nsengine.resource<NSMaterial>(renComp->materialID(i));
		QTreeWidgetItem * item = new QTreeWidgetItem();
		item->setText(0, QString::number(i)); item->setData(0, 0, i);
		item->setText(1, "None");
		if (mat != NULL)
			item->setText(1,mat->name().c_str());
		mUI.mMaterialsTW->addTopLevelItem(item);
	}
}


void RenderCompWidget::onCastShadowsChange(bool)
{
	if (mEnt == NULL)
		return;

	NSRenderComp * renComp = mEnt->get<NSRenderComp>();
	if (renComp == NULL)
		return;

	renComp->setCastShadow(mUI.mCastsShadowsCB->isChecked());
	mTK->mapView()->update();
}

void RenderCompWidget::onChooseMat()
{
	if (mEnt == NULL)
		return;

	NSRenderComp * renComp = mEnt->get<NSRenderComp>();
	if (renComp == NULL)
		return;

	auto items = mUI.mMaterialsTW->selectedItems();
	if (items.isEmpty())
		return;

	QTreeWidgetItem * twItem = items.first();

	QDialog selMatD(this);
	Ui::SelectResDialog ui;
	ui.setupUi(&selMatD);
	selMatD.setWindowTitle("Select Material");

	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);

		NSMatManager * mats = plg->manager<NSMatManager>();
		auto miter = mats->begin();
		while (miter != mats->end())
		{
			QListWidgetItem * toAdd = new QListWidgetItem(miter->second->name().c_str());
			toAdd->setData(VIEW_WIDGET_ITEM_PLUG, miter->second->plugid());
			toAdd->setData(VIEW_WIDGET_ITEM_ENT, miter->second->id());
			ui.mListWidget->addItem(toAdd);
			++miter;
		}

		++plugiter;
	}

	if (selMatD.exec() == QDialog::Accepted)
	{
		auto lwitems = ui.mListWidget->selectedItems();
		if (lwitems.isEmpty())
			return;
		auto lwitem = lwitems.first();

		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

		NSMaterial * mat = nsengine.resource<NSMaterial>(id);
		if (mat == NULL)
			return;

		renComp->setMaterial(twItem->data(0, 0).toInt(), mat->fullid(), true);
		setEntity(mEnt);
		mTK->mapView()->update();
	}
}

void RenderCompWidget::onChooseMesh()
{
	if (mEnt == NULL)
		return;

	NSRenderComp * renComp = mEnt->get<NSRenderComp>();
	if (renComp == NULL)
		return;

	QDialog selMeshD(this);
	Ui::SelectResDialog ui;
	ui.setupUi(&selMeshD);

	selMeshD.setWindowTitle("Select Mesh");

	auto plugiter = nsengine.plugins()->begin();
	while (plugiter != nsengine.plugins()->end())
	{
		NSPlugin * plg = nsengine.plugin(plugiter->first);

		NSMeshManager * meshes = plg->manager<NSMeshManager>();
		auto miter = meshes->begin();
		while (miter != meshes->end())
		{
			QListWidgetItem * toAdd = new QListWidgetItem(miter->second->name().c_str());
			toAdd->setData(VIEW_WIDGET_ITEM_PLUG, miter->second->plugid());
			toAdd->setData(VIEW_WIDGET_ITEM_ENT, miter->second->id());
			ui.mListWidget->addItem(toAdd);
			++miter;
		}

		++plugiter;
	}

	if (selMeshD.exec() == QDialog::Accepted)
	{
		auto lwitems = ui.mListWidget->selectedItems();
		if (lwitems.isEmpty())
			return;
		auto lwitem = lwitems.first();

		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

		NSMesh * mesh = nsengine.resource<NSMesh>(id);
		if (mesh == NULL)
			return;

		renComp->setMeshID(mesh->fullid());
		renComp->clearMats();
		setEntity(mEnt);
		mTK->mapView()->update();
	}
}

void RenderCompWidget::onClearMat()
{
	if (mEnt == NULL)
		return;

	NSRenderComp * renComp = mEnt->get<NSRenderComp>();
	if (renComp == NULL)
		return;

	auto items = mUI.mMaterialsTW->selectedItems();
	if (items.isEmpty())
		return;

	QTreeWidgetItem * twItem = items.first();

	renComp->removeMaterial(twItem->data(0, 0).toInt());
	setEntity(mEnt);
	mTK->mapView()->update();

}

void RenderCompWidget::onSelectionChange()
{
	if (mEnt == NULL)
		return;

	NSRenderComp * renComp = mEnt->get<NSRenderComp>();
	if (renComp == NULL)
		return;

	auto items = mUI.mMaterialsTW->selectedItems();

	mUI.mChooseMatBtn->setEnabled(!items.isEmpty());
	mUI.mClearMatBtn->setEnabled(!items.isEmpty());
}


SelCompWidget::SelCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

SelCompWidget::~SelCompWidget()
{}

void SelCompWidget::setEntity(NSEntity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;

	NSSelComp * selComp = mEnt->get<NSSelComp>();
	if (selComp == NULL)
		return;

	mUI.mAlphaSB->blockSignals(true);
	mUI.mAlphaSB->setValue(selComp->maskAlpha());
	mUI.mAlphaSB->blockSignals(false);
	
	fvec4 col = selComp->defaultColor();
	QColor prevC;
	prevC.setRedF(col.x); prevC.setGreenF(col.y); prevC.setBlueF(col.z); prevC.setAlphaF(col.w);
	_setColorStyle(prevC);
}

void SelCompWidget::onAlphaChange(double pVal)
{
	if (mEnt == NULL)
		return;

	NSSelComp * selComp = mEnt->get<NSSelComp>();
	if (selComp == NULL)
		return;

	selComp->setMaskAlpha(pVal);
	mTK->mapView()->update();
}

void SelCompWidget::onChangeColor()
{
	if (mEnt == NULL)
		return;

	NSSelComp * selComp = mEnt->get<NSSelComp>();
	if (selComp == NULL)
		return;

	fvec4 col = selComp->defaultColor();
	QColor prevC;
	prevC.setRedF(col.x); prevC.setGreenF(col.y); prevC.setBlueF(col.z); prevC.setAlphaF(col.w);
	QColor c = QColorDialog::getColor(prevC);
	if (c.isValid())
	{
		fvec4 newCol(c.redF(), c.greenF(), c.blueF(), c.alphaF());
		selComp->setDefaultColor(newCol);
		prevC = c;
	}
	_setColorStyle(prevC);
	mTK->mapView()->update();
}

void SelCompWidget::_setColorStyle(const QColor & pCol)
{
	QString rgbcol = "rgb(" + QString::number(pCol.red()) + "," + QString::number(pCol.green()) + "," + QString::number(pCol.blue()) + ");";
	mUI.mColorFrame->setStyleSheet(
		"QFrame {\
		background-color: " + rgbcol +
		"border: none\
		}\
		QFrame:hover\
		{\
		background-color: rgb(200,200,200);\
		}");
}

TransformCompWidget::TransformCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

TransformCompWidget::~TransformCompWidget()
{}

TilebrushCompWidget::TilebrushCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

TilebrushCompWidget::~TilebrushCompWidget()
{}

TileCompWidget::TileCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

TileCompWidget::~TileCompWidget()
{}
