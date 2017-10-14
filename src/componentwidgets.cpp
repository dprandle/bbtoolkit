#include <componentwidgets.h>
#include <nsentity.h>
#include <nsanim_comp.h>
#include <nscam_comp.h>
#include <nslight_comp.h>
#include <nsoccupy_comp.h>
#include <nsparticle_comp.h>
#include <nsshader_manager.h>
#include <nsplugin_manager.h>
#include <nsrender_comp.h>
#include <nssel_comp.h>
#include <nstform_comp.h>
#include <nsmat_manager.h>
#include <nstile_brush_comp.h>
#include <nstex_manager.h>
#include <nstile_comp.h>
#include <qcolordialog.h>
#include <qcolor.h>
#include <toolkit.h>
#include <nsengine.h>
#include <nsmesh_manager.h>
#include <ui_select_res_dialog.h>
#include <nstform_system.h>

CompWidget::CompWidget(QWidget * pParent) :
mEnt(NULL),
QWidget(pParent)
{}

void CompWidget::init()
{}

void CompWidget::setEntity(nsentity * pEnt)
{
	mEnt = pEnt;
}

nsentity * CompWidget::getEntity()
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

void CamCompWidget::setEntity(nsentity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;

	nscam_comp * camComp = mEnt->get<nscam_comp>();
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

	nscam_comp * camComp = mEnt->get<nscam_comp>();
	if (camComp == NULL)
		return;

    camComp->set_speed(pNewSpeed);
    bbtk.map()->update();
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

void LightCompWidget::setEntity(nsentity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
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
    mUI.mShadowSldr->setValue(lightComp->shadow_darkness()*100);
	mUI.mShadowSldr->blockSignals(false);

	mUI.mShadowSamplesSB->blockSignals(true);
    mUI.mShadowSamplesSB->setValue(lightComp->shadow_samples());
	mUI.mShadowSamplesSB->blockSignals(false);

	mUI.mCastsShadowsCB->blockSignals(true);
    mUI.mCastsShadowsCB->setChecked(lightComp->cast_shadows());
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

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_intensity(lightComp->intensity().x, float(pVal)/100.0f, nslight_comp::adjustment_t(mUI.mFalloffCB->currentIndex()));
	setEntity(mEnt);
    bbtk.map()->update();
}

void LightCompWidget::onColorChange()
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

	fvec3 col = lightComp->color();
	QColor prevC;
	prevC.setRedF(col.x); prevC.setGreenF(col.y); prevC.setBlueF(col.z);
	QColor c = QColorDialog::getColor(prevC);
	if (c.isValid())
	{
		fvec3 newCol(c.redF(), c.greenF(), c.blueF());
        lightComp->set_color(newCol);
		prevC = c;
	}
	_setColorStyle(prevC);
    bbtk.map()->update();
}

void LightCompWidget::onCutoffChange(int pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_cutoff(float(pVal));
	setEntity(mEnt);
    bbtk.map()->update();
}

void LightCompWidget::onDiffuseChange(int pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_intensity(float(pVal)/100.0f, lightComp->intensity().y, nslight_comp::adjustment_t(mUI.mFalloffCB->currentIndex()));
	setEntity(mEnt);
    bbtk.map()->update();
}

void LightCompWidget::onDistanceChange(int pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_distance(float(pVal), nslight_comp::adjustment_t(mUI.mFalloffCB->currentIndex()));
	mUI.mDistanceSB->blockSignals(true);
	mUI.mDistanceSB->setValue(double(pVal));
	mUI.mDistanceSB->blockSignals(false);
	setEntity(mEnt);
    bbtk.map()->update();
}

void LightCompWidget::onDistanceChangeSB(double pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_distance(pVal, nslight_comp::adjustment_t(mUI.mFalloffCB->currentIndex()));
	mUI.mDistanceSldr->blockSignals(true);
	mUI.mDistanceSldr->setValue(int(pVal));
	mUI.mDistanceSldr->blockSignals(false);
	setEntity(mEnt);
    bbtk.map()->update();
}

void LightCompWidget::onFalloffChange(QString pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

	setEntity(mEnt);
    bbtk.map()->update();
}

void LightCompWidget::onLightTypeChange(QString pVal)
{
//	if (mEnt == NULL)
//		return;

//	nslight_comp * lightComp = mEnt->get<nslight_comp>();
//	if (lightComp == NULL)
//		return;

//    nslight_comp::light_t lType = nslight_comp::light_t(mUI.mLightTypeCB->findText(pVal));
//	uivec2 ID;

//	switch (lType)
//	{
//    case(nslight_comp::l_dir) :
//        ID = nse.resource<nsmesh>(ENGINE_PLUG,MESH_DIRLIGHT_BOUNDS)->full_id();
//		break;
//    case(nslight_comp::l_point) :
//        ID = nse.resource<nsmesh>(ENGINE_PLUG, MESH_POINTLIGHT_BOUNDS)->full_id();
//		break;
//    case(nslight_comp::l_spot) :
//        ID = nse.resource<nsmesh>(ENGINE_PLUG, MESH_SPOTLIGHT_BOUNDS)->full_id();
//		break;
//	}

//    lightComp->set_type(lType);
//    lightComp->set_mesh_id(ID);
//	setEntity(mEnt);
//    bbtk.map()->update();
}

void LightCompWidget::onRadiusChange(double pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_radius(pVal);
	setEntity(mEnt);
    bbtk.map()->update();
}

void LightCompWidget::onCastShadowChange(bool pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_cast_shadows(pVal);
    bbtk.map()->update();
}

void LightCompWidget::onShadowDarknessChange(int pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_shadow_darkness(float(pVal)/100.0f);
    bbtk.map()->update();
}

void LightCompWidget::onShadowSamplesChange(int pVal)
{
	if (mEnt == NULL)
		return;

	nslight_comp * lightComp = mEnt->get<nslight_comp>();
	if (lightComp == NULL)
		return;

    lightComp->set_shadow_samples(pVal);
    bbtk.map()->update();
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

void ParticleCompWidget::setEntity(nsentity * pEnt)
{
//	CompWidget::setEntity(pEnt);
//	if (mEnt == NULL)
//		return;


//	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
//	if (partComp == NULL)
//		return;

//	mUI.mKeyframeTblW->clearContents();
//	mUI.mForceTblW->clearContents();
//	mUI.mForceTblW->setMinimumHeight(40);
//	mUI.mKeyframeTblW->setMinimumHeight(40);

//    nsmaterial * mat = nse.resource<nsmaterial>(partComp->material_id());
//    nsshader * shader = nse.resource<nsshader>(partComp->shader_id());
//    nstexture * tex = nse.resource<nstexture>(partComp->rand_tex_id());

//	if (mat != NULL)
//		mUI.mMaterialLE->setText(mat->name().c_str());
//	else
//		mUI.mMaterialLE->setText("None");

//	if (shader != NULL)
//		mUI.mShaderLE->setText(shader->name().c_str());
//	else
//		mUI.mShaderLE->setText("None");
	
//	if (tex != NULL)
//		mUI.mRandTexLE->setText(tex->name().c_str());
//	else
//		mUI.mRandTexLE->setText("None");

//	mUI.mLoopCB->setChecked(partComp->looping());
//	mUI.mSimCB->setChecked(partComp->simulating());

//	mUI.mMaxParticlesSB->blockSignals(true);
//    mUI.mMaxParticlesSB->setValue(partComp->max_particles());
//	mUI.mMaxParticlesSB->blockSignals(false);

//	mUI.mEmissionSB->blockSignals(true);
//    mUI.mEmissionSB->setValue(partComp->emission_rate());
//	mUI.mEmissionSB->blockSignals(false);

//	mUI.mAngVelSB->blockSignals(true);
//    mUI.mAngVelSB->setValue(partComp->angular_vel());
//	mUI.mAngVelSB->blockSignals(false);

//	mUI.mLifetimeDSB->blockSignals(true);
//	mUI.mLifetimeDSB->setValue(float(partComp->lifetime())/1000.0f);
//	mUI.mLifetimeDSB->blockSignals(false);

//	mUI.mStartWidthDSB->blockSignals(true);
//    mUI.mStartWidthDSB->setValue(partComp->starting_size().u);
//	mUI.mStartWidthDSB->blockSignals(false);

//	mUI.mStartHeightDSB->blockSignals(true);
//    mUI.mStartHeightDSB->setValue(partComp->starting_size().v);
//	mUI.mStartHeightDSB->blockSignals(false);

//	mUI.mEmitComb->blockSignals(true);
//    mUI.mEmitComb->setCurrentIndex(partComp->emitter_shape());
//	mUI.mEmitComb->blockSignals(false);

//    fvec3 ss = partComp->emitter_size();
//	mUI.mEmitXDSB->blockSignals(true);
//	mUI.mEmitXDSB->setValue(ss.x);
//	mUI.mEmitXDSB->blockSignals(false);

//	mUI.mEmitYDSB->blockSignals(true);
//	mUI.mEmitYDSB->setValue(ss.y);
//	mUI.mEmitYDSB->blockSignals(false);

//	mUI.mEmitZDSB->blockSignals(true);
//	mUI.mEmitZDSB->setValue(ss.z);
//	mUI.mEmitZDSB->blockSignals(false);

//	mUI.mMotionComb->blockSignals(true);
//    mUI.mMotionComb->setCurrentIndex(partComp->motion_key_type());
//	mUI.mMotionComb->blockSignals(false);

//	mUI.mInterpMotionCB->blockSignals(true);
//    mUI.mInterpMotionCB->setChecked(partComp->motion_key_interpolation());
//	mUI.mInterpMotionCB->blockSignals(false);

//	mUI.mGlobalTimeCB->blockSignals(true);
//    mUI.mGlobalTimeCB->setChecked(partComp->motion_global_time());
//	mUI.mGlobalTimeCB->blockSignals(false);

//	mUI.mGlobalTimeVisualCB->blockSignals(true);
//    mUI.mGlobalTimeVisualCB->setChecked(partComp->visual_global_time());
//	mUI.mGlobalTimeVisualCB->blockSignals(false);

//	mUI.mInterpolateVisualCB->blockSignals(true);
//    mUI.mInterpolateVisualCB->setChecked(partComp->visual_key_interpolation());
//	mUI.mInterpolateVisualCB->blockSignals(false);

//    fvec3 iv = partComp->init_vel_mult();
//	mUI.mInitVelXDSB->blockSignals(true);
//	mUI.mInitVelXDSB->setValue(iv.x);
//	mUI.mInitVelXDSB->blockSignals(false);

//	mUI.mInitVelYDSB->blockSignals(true);
//	mUI.mInitVelYDSB->setValue(iv.y);
//	mUI.mInitVelYDSB->blockSignals(false);

//	mUI.mInitVelZDSB->blockSignals(true);
//	mUI.mInitVelZDSB->setValue(iv.z);
//	mUI.mInitVelZDSB->blockSignals(false);

//	mUI.mBlendComb->blockSignals(true);
//    mUI.mBlendComb->setCurrentIndex(partComp->blend_mode());
//	mUI.mBlendComb->blockSignals(false);



//    mUI.mForceTblW->setRowCount(partComp->motion_key_count());
//    mUI.mKeyframeTblW->setRowCount(partComp->visual_key_count());

//	mUI.mForceTblW->blockSignals(true);
//    auto iterF = partComp->begin_motion_key();
//	int index = 0;
//    while (iterF != partComp->end_motion_key())
//	{
//		QTableWidgetItem * item = new QTableWidgetItem();
//		QTableWidgetItem * item2 = new QTableWidgetItem();
//		QTableWidgetItem * item3 = new QTableWidgetItem();
//		QTableWidgetItem * item4 = new QTableWidgetItem();
		
//        float normTime = float(iterF->first) / float(partComp->max_motion_keys());
//		item->setData(0, normTime);
//		item->setData(Qt::UserRole, normTime);

//		item2->setData(0, iterF->second.x);
//		item2->setData(Qt::UserRole, iterF->second.x);

//		item3->setData(0, iterF->second.y);
//		item3->setData(Qt::UserRole, iterF->second.y);

//		item4->setData(0, iterF->second.z);
//		item4->setData(Qt::UserRole, iterF->second.z);

//		mUI.mForceTblW->setItem(index, 0, item);
//		mUI.mForceTblW->setItem(index, 1, item2);
//		mUI.mForceTblW->setItem(index, 2, item3);
//		mUI.mForceTblW->setItem(index, 3, item4);
//		mUI.mForceTblW->setMinimumHeight(mUI.mForceTblW->minimumHeight()+20);
//		++index;
//		++iterF;
//	}
//	mUI.mForceTblW->blockSignals(false);

//	mUI.mKeyframeTblW->blockSignals(true);
//	index = 0;
//    auto iterR = partComp->begin_visual_key();
//    while (iterR != partComp->end_visual_key())
//	{
//		QTableWidgetItem * item = new QTableWidgetItem();
//		QTableWidgetItem * item2 = new QTableWidgetItem();
//		QTableWidgetItem * item3 = new QTableWidgetItem();
//		QTableWidgetItem * item4 = new QTableWidgetItem();

//        float normTime = float(iterR->first) / float(partComp->max_visual_keys());
//		item->setData(0, normTime);
//		item->setData(Qt::UserRole, normTime);

//		item2->setData(0, iterR->second.x);
//		item2->setData(Qt::UserRole, iterR->second.x);

//		item3->setData(0, iterR->second.y);
//		item3->setData(Qt::UserRole, iterR->second.y);

//		item4->setData(0, iterR->second.z);
//		item4->setData(Qt::UserRole, iterR->second.z);

//		mUI.mKeyframeTblW->setItem(index, 0, item);
//		mUI.mKeyframeTblW->setItem(index, 1, item2);
//		mUI.mKeyframeTblW->setItem(index, 2, item3);
//		mUI.mKeyframeTblW->setItem(index, 3, item4);
//		mUI.mKeyframeTblW->setMinimumHeight(mUI.mKeyframeTblW->minimumHeight() + 20);
//		++index;
//		++iterR;
//	}
//	mUI.mKeyframeTblW->blockSignals(false);
}

void ParticleCompWidget::onChooseMat()
{
//	if (mEnt == NULL)
//		return;

//	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
//	if (partComp == NULL)
//		return;

//	QDialog selMatD(this);
//    Ui::select_res_dialog ui;
//	ui.setupUi(&selMatD);
//	selMatD.setWindowTitle("Select Material");

//	auto plugiter = nse.plugins()->begin();
//	while (plugiter != nse.plugins()->end())
//	{
//		nsplugin * plg = nse.plugin(plugiter->first);

//		nsmat_manager * mats = plg->manager<nsmat_manager>();
//		auto miter = mats->begin();
//		while (miter != mats->end())
//		{
//			QListWidgetItem * toAdd = new QListWidgetItem(miter->second->name().c_str());
//            toAdd->setData(VIEW_WIDGET_ITEM_PLUG, miter->second->plugin_id());
//			toAdd->setData(VIEW_WIDGET_ITEM_ENT, miter->second->id());
//            ui.m_lw->addItem(toAdd);
//			++miter;
//		}

//		++plugiter;
//	}

//	if (selMatD.exec() == QDialog::Accepted)
//	{
//        auto lwitems = ui.m_lw->selectedItems();
//		if (lwitems.isEmpty())
//			return;
//		auto lwitem = lwitems.first();

//		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

//		nsmaterial * mat = nse.resource<nsmaterial>(id);
//		if (mat == NULL)
//			return;

//        partComp->set_material_id(mat->full_id());
//		setEntity(mEnt);
//	}
}

void ParticleCompWidget::onChangeBlendMode(int pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->set_blend_mode(nsparticle_comp::blend_m(pVal));
	setEntity(mEnt);
}

void ParticleCompWidget::onChooseShader()
{
//	if (mEnt == NULL)
//		return;

//	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
//	if (partComp == NULL)
//		return;

//	QDialog selShaderD(this);
//    Ui::select_res_dialog ui;
//	ui.setupUi(&selShaderD);
//	selShaderD.setWindowTitle("Select Shader");

//	auto plugiter = nse.plugins()->begin();
//	while (plugiter != nse.plugins()->end())
//	{
//		nsplugin *plg = nse.plugin(plugiter->first);
//		nsshader_manager * shaders = plg->manager<nsshader_manager>();
//		auto siter = shaders->begin();
//		while (siter != shaders->end())
//		{
//			QListWidgetItem * lwitem = new QListWidgetItem(siter->second->name().c_str());
//            lwitem->setData(VIEW_WIDGET_ITEM_PLUG, siter->second->plugin_id());
//			lwitem->setData(VIEW_WIDGET_ITEM_ENT, siter->second->id());
//            ui.m_lw->addItem(lwitem);
//			++siter;
//		}
//		++plugiter;
//	}

//	if (selShaderD.exec() == QDialog::Accepted)
//	{
//        auto lwitems = ui.m_lw->selectedItems();
//		if (lwitems.isEmpty())
//			return;
//		auto lwitem = lwitems.first();

//		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());
//		nsshader * shader = nse.resource<nsshader>(id);
//		if (shader == NULL)
//			return;

//        partComp->set_shader_id(shader->full_id());
//		setEntity(mEnt);
//	}
}

void ParticleCompWidget::onChooseRandTex()
{
//	if (mEnt == NULL)
//		return;

//	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
//	if (partComp == NULL)
//		return;

//	QDialog selTextureD(this);
//    Ui::select_res_dialog ui;
//	ui.setupUi(&selTextureD);
//	selTextureD.setWindowTitle("Select Texture");


//	auto plugiter = nse.plugins()->begin();
//	while (plugiter != nse.plugins()->end())
//	{
//		nsplugin *plg = nse.plugin(plugiter->first);
//		nstex_manager * textures = plg->manager<nstex_manager>();
//		auto txiter = textures->begin();
//		while (txiter != textures->end())
//		{
//			nstexture * tex = textures->get(txiter->first);
//			QListWidgetItem * lwitem = new QListWidgetItem(txiter->second->name().c_str());
//            lwitem->setData(VIEW_WIDGET_ITEM_PLUG, tex->plugin_id());
//			lwitem->setData(VIEW_WIDGET_ITEM_ENT, tex->id());
//            ui.m_lw->addItem(lwitem);
//			++txiter;
//		}
//		++plugiter;
//	}
//	if (selTextureD.exec() == QDialog::Accepted)
//	{
//        auto lwitems = ui.m_lw->selectedItems();
//		if (lwitems.isEmpty())
//			return;
//		auto lwitem = lwitems.first();
//		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());
//		nstexture * tex = nse.resource<nstexture>(id);
//		if (tex == NULL)
//			return;

//        partComp->set_rand_tex_id(tex->full_id());
//		setEntity(mEnt);
//	}
}

void ParticleCompWidget::onChangeMaxParticle()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->set_max_particles(mUI.mMaxParticlesSB->value());
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeEmissionRate()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->set_emission_rate(mUI.mEmissionSB->value());
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeAngVel()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->set_angular_vel(mUI.mAngVelSB->value());
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeLifetime()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->set_lifetime(uint32(mUI.mLifetimeDSB->value()*1000.0f));
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeStartHeight()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    fvec2 size = partComp->starting_size();
	size.v = mUI.mStartHeightDSB->value();
    partComp->set_starting_size(size);
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeStartWidth()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    fvec2 size = partComp->starting_size();
	size.u = mUI.mStartWidthDSB->value();
    partComp->set_starting_size(size);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleLoop(bool pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->enable_looping(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleSim(bool pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->enable_simulation(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onAddForce()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mForceTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();

    while (partComp->has_motion_key(time))
        time += 1 / float(partComp->max_motion_keys());

    partComp->set_motion_key(time, fvec3());
	setEntity(mEnt);
}

void ParticleCompWidget::onDelForce()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mForceTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();
    partComp->remove_motion_key(time);

	setEntity(mEnt);
}

void ParticleCompWidget::onAddKeyframe()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mKeyframeTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();

    while (partComp->has_visual_key(time))
        time += 1 / float(partComp->max_visual_keys());

    partComp->set_visual_key(time, fvec3(1.0f, 1.0f, 1.0f));
	setEntity(mEnt);
}

void ParticleCompWidget::onDelKeyframe()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	auto stuff = mUI.mKeyframeTblW->selectionModel()->selectedRows();
	if (stuff.empty())
		return;

	float time = stuff.first().data().toFloat();
    partComp->remove_visual_key(time);

	setEntity(mEnt);
}

void ParticleCompWidget::onForceItemChange(QTableWidgetItem* pItem)
{
	if (mEnt == NULL || pItem == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	if (pItem->column() == 0)
	{
		float oldTime = pItem->data(Qt::UserRole).toFloat();
		float newTime = pItem->data(0).toFloat();

        fvec3 force = partComp->motion_key_at(oldTime);
        partComp->remove_motion_key(oldTime);
        partComp->set_motion_key(newTime, force);
	}
	else
	{
		float time = mUI.mForceTblW->item(pItem->row(), 0)->data(0).toFloat();
        fvec3 force = partComp->motion_key_at(time);
		if (pItem->column() == 1)
			force.x = pItem->data(0).toFloat();
		else if (pItem->column() == 2)
			force.y = pItem->data(0).toFloat();
		else
			force.z = pItem->data(0).toFloat();
        partComp->set_motion_key(time, force);
	}

	setEntity(mEnt);
}

void ParticleCompWidget::onKeyframeItemChange(QTableWidgetItem* pItem)
{
	if (mEnt == NULL || pItem == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	if (pItem->column() == 0)
	{
		float oldTime = pItem->data(Qt::UserRole).toFloat();
		float newTime = pItem->data(0).toFloat();

        fvec3 renKey = partComp->visual_key_at(oldTime);
        partComp->remove_visual_key(oldTime);
        partComp->set_visual_key(newTime, renKey);
	}
	else
	{
		float time = mUI.mKeyframeTblW->item(pItem->row(), 0)->data(0).toFloat();
        fvec3 renKey = partComp->visual_key_at(time);
		if (pItem->column() == 1)
			renKey.x = pItem->data(0).toFloat();
		else if (pItem->column() == 2)
			renKey.y = pItem->data(0).toFloat();
		else
			renKey.z = pItem->data(0).toFloat();
        partComp->set_visual_key(time, renKey);
	}

	setEntity(mEnt);
}

void ParticleCompWidget::onChangeEmitterShape(int pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->set_emitter_shape(nsparticle_comp::emitter_shape_t(pVal));
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeEmitterSize()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	fvec3 emitterSize;
	emitterSize.x = mUI.mEmitXDSB->value();
	emitterSize.y = mUI.mEmitYDSB->value();
	emitterSize.z = mUI.mEmitZDSB->value();
    partComp->set_emitter_size(emitterSize);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleInterpolateVisual(bool pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;
    partComp->enable_visual_key_interp(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleGlobalTimeVisual(bool pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->enable_visual_global_time(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeInitVel()
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

	fvec3 initVel;
	initVel.x = mUI.mInitVelXDSB->value();
	initVel.y = mUI.mInitVelYDSB->value();
	initVel.z = mUI.mInitVelZDSB->value();
    partComp->set_init_vel_mult(initVel);
	setEntity(mEnt);
}

void ParticleCompWidget::onChangeMotionType(int pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->set_motion_key_type(nsparticle_comp::motion_key_t(pVal));
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleInterpolate(bool pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->enable_motion_key_interp(pVal);
	setEntity(mEnt);
}

void ParticleCompWidget::onToggleGlobalTime(bool pVal)
{
	if (mEnt == NULL)
		return;

	nsparticle_comp * partComp = mEnt->get<nsparticle_comp>();
	if (partComp == NULL)
		return;

    partComp->enable_motion_global_time(pVal);
	setEntity(mEnt);
}



RenderCompWidget::RenderCompWidget(QWidget * pParent) :
CompWidget(pParent)
{
	mUI.setupUi(this);
}

RenderCompWidget::~RenderCompWidget()
{}

void RenderCompWidget::setEntity(nsentity * pEnt)
{
//	CompWidget::setEntity(pEnt);
//	if (mEnt == NULL)
//		return;

//	nsrender_comp * renComp = mEnt->get<nsrender_comp>();
//	if (renComp == NULL)
//		return;

//	mUI.mMaterialsTW->clear();

//	mUI.mCastsShadowsCB->blockSignals(true);
//    mUI.mCastsShadowsCB->setChecked(renComp->cast_shadow());
//	mUI.mCastsShadowsCB->blockSignals(false);

//    nsmesh * mesh = nse.resource<nsmesh>(renComp->mesh_id());

//	mUI.mMeshNameLE->setText(mesh->name().c_str());
	
	
//	for (uint32 i = 0; i < mesh->count(); ++i)
//	{
//        nsmaterial * mat = nse.resource<nsmaterial>(renComp->material_id(i));
//		QTreeWidgetItem * item = new QTreeWidgetItem();
//		item->setText(0, QString::number(i)); item->setData(0, 0, i);
//		item->setText(1, "None");
//		if (mat != NULL)
//			item->setText(1,mat->name().c_str());
//		mUI.mMaterialsTW->addTopLevelItem(item);
//	}
}


void RenderCompWidget::onCastShadowsChange(bool)
{
	if (mEnt == NULL)
		return;

	nsrender_comp * renComp = mEnt->get<nsrender_comp>();
	if (renComp == NULL)
		return;

    renComp->set_cast_shadow(mUI.mCastsShadowsCB->isChecked());
    bbtk.map()->update();
}

void RenderCompWidget::onChooseMat()
{
//	if (mEnt == NULL)
//		return;

//	nsrender_comp * renComp = mEnt->get<nsrender_comp>();
//	if (renComp == NULL)
//		return;

//	auto items = mUI.mMaterialsTW->selectedItems();
//	if (items.isEmpty())
//		return;

//	QTreeWidgetItem * twItem = items.first();

//	QDialog selMatD(this);
//    Ui::select_res_dialog ui;
//	ui.setupUi(&selMatD);
//	selMatD.setWindowTitle("Select Material");

//	auto plugiter = nse.plugins()->begin();
//	while (plugiter != nse.plugins()->end())
//	{
//		nsplugin * plg = nse.plugin(plugiter->first);

//		nsmat_manager * mats = plg->manager<nsmat_manager>();
//		auto miter = mats->begin();
//		while (miter != mats->end())
//		{
//			QListWidgetItem * toAdd = new QListWidgetItem(miter->second->name().c_str());
//            toAdd->setData(VIEW_WIDGET_ITEM_PLUG, miter->second->plugin_id());
//			toAdd->setData(VIEW_WIDGET_ITEM_ENT, miter->second->id());
//            ui.m_lw->addItem(toAdd);
//			++miter;
//		}

//		++plugiter;
//	}

//	if (selMatD.exec() == QDialog::Accepted)
//	{
//        auto lwitems = ui.m_lw->selectedItems();
//		if (lwitems.isEmpty())
//			return;
//		auto lwitem = lwitems.first();

//		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

//		nsmaterial * mat = nse.resource<nsmaterial>(id);
//		if (mat == NULL)
//			return;

//        renComp->set_material(twItem->data(0, 0).toInt(), mat->full_id(), true);
//		setEntity(mEnt);
//        bbtk.map()->update();
//	}
}

void RenderCompWidget::onChooseMesh()
{
//	if (mEnt == NULL)
//		return;

//	nsrender_comp * renComp = mEnt->get<nsrender_comp>();
//	if (renComp == NULL)
//		return;

//	QDialog selMeshD(this);
//    Ui::select_res_dialog ui;
//	ui.setupUi(&selMeshD);

//	selMeshD.setWindowTitle("Select Mesh");

//	auto plugiter = nse.plugins()->begin();
//	while (plugiter != nse.plugins()->end())
//	{
//		nsplugin * plg = nse.plugin(plugiter->first);

//		nsmesh_manager * meshes = plg->manager<nsmesh_manager>();
//		auto miter = meshes->begin();
//		while (miter != meshes->end())
//		{
//			QListWidgetItem * toAdd = new QListWidgetItem(miter->second->name().c_str());
//            toAdd->setData(VIEW_WIDGET_ITEM_PLUG, miter->second->plugin_id());
//			toAdd->setData(VIEW_WIDGET_ITEM_ENT, miter->second->id());
//            ui.m_lw->addItem(toAdd);
//			++miter;
//		}

//		++plugiter;
//	}

//	if (selMeshD.exec() == QDialog::Accepted)
//	{
//        auto lwitems = ui.m_lw->selectedItems();
//		if (lwitems.isEmpty())
//			return;
//		auto lwitem = lwitems.first();

//		uivec2 id(lwitem->data(VIEW_WIDGET_ITEM_PLUG).toUInt(), lwitem->data(VIEW_WIDGET_ITEM_ENT).toUInt());

//		nsmesh * mesh = nse.resource<nsmesh>(id);
//		if (mesh == NULL)
//			return;

//        renComp->set_mesh_id(mesh->full_id());
//        renComp->clear_mats();
//		setEntity(mEnt);
//        bbtk.map()->update();
//	}
}

void RenderCompWidget::onClearMat()
{
	if (mEnt == NULL)
		return;

	nsrender_comp * renComp = mEnt->get<nsrender_comp>();
	if (renComp == NULL)
		return;

	auto items = mUI.mMaterialsTW->selectedItems();
	if (items.isEmpty())
		return;

	QTreeWidgetItem * twItem = items.first();

    renComp->remove_material(twItem->data(0, 0).toInt());
	setEntity(mEnt);
    bbtk.map()->update();

}

void RenderCompWidget::onSelectionChange()
{
	if (mEnt == NULL)
		return;

	nsrender_comp * renComp = mEnt->get<nsrender_comp>();
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

void SelCompWidget::setEntity(nsentity * pEnt)
{
	CompWidget::setEntity(pEnt);
	if (mEnt == NULL)
		return;

	nssel_comp * selComp = mEnt->get<nssel_comp>();
	if (selComp == NULL)
		return;

	mUI.mAlphaSB->blockSignals(true);
    mUI.mAlphaSB->setValue(selComp->mask_alpha());
	mUI.mAlphaSB->blockSignals(false);
	
    fvec4 col = selComp->default_color();
	QColor prevC;
	prevC.setRedF(col.x); prevC.setGreenF(col.y); prevC.setBlueF(col.z); prevC.setAlphaF(col.w);
	_setColorStyle(prevC);
}

void SelCompWidget::onAlphaChange(double pVal)
{
	if (mEnt == NULL)
		return;

	nssel_comp * selComp = mEnt->get<nssel_comp>();
	if (selComp == NULL)
		return;

    selComp->set_mask_alpha(pVal);
    bbtk.map()->update();
}

void SelCompWidget::onChangeColor()
{
	if (mEnt == NULL)
		return;

	nssel_comp * selComp = mEnt->get<nssel_comp>();
	if (selComp == NULL)
		return;

    fvec4 col = selComp->default_color();
	QColor prevC;
	prevC.setRedF(col.x); prevC.setGreenF(col.y); prevC.setBlueF(col.z); prevC.setAlphaF(col.w);
	QColor c = QColorDialog::getColor(prevC);
	if (c.isValid())
	{
		fvec4 newCol(c.redF(), c.greenF(), c.blueF(), c.alphaF());
        selComp->set_default_sel_color(newCol);
		prevC = c;
	}
	_setColorStyle(prevC);
    bbtk.map()->update();
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
