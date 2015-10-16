#include <camerasettingsdialog.h>
#include <nsengine.h>
#include <nscamerasystem.h>

CameraSettingsDialog::CameraSettingsDialog(QWidget * pParent) :
QDialog(pParent)
{}

CameraSettingsDialog::~CameraSettingsDialog()
{}

void CameraSettingsDialog::init(Toolkit * pTK)
{
	mTK = pTK;
	mUI.setupUi(this);
}

void CameraSettingsDialog::showIt()
{
	NSCameraSystem * camSys = mEngine->system<NSCameraSystem>();
	int turn = camSys->sensitivity(NSCameraSystem::Turn);
	int move = camSys->sensitivity(NSCameraSystem::Strafe);
	bool invertXFree = camSys->xinv();
	bool invertYFree = camSys->yinv();
	bool invertXFoc = camSys->xinv(NSCameraSystem::Focus);
	bool invertYFoc = camSys->yinv(NSCameraSystem::Focus);

	mUI.mTurnSldr->blockSignals(true);
	mUI.mTurnSldr->setValue(turn);
	mUI.mTurnSldr->blockSignals(false);

	mUI.mMoveSldr->blockSignals(true);
	mUI.mMoveSldr->setValue(move);
	mUI.mMoveSldr->blockSignals(false);

	mUI.mCamCenterXCmB->blockSignals(true);
	mUI.mCamCenterXCmB->setCurrentIndex(int(invertXFree));
	mUI.mCamCenterXCmB->blockSignals(false);

	mUI.mCamCenterYCmB->blockSignals(true);
	mUI.mCamCenterYCmB->setCurrentIndex(int(invertYFree));
	mUI.mCamCenterYCmB->blockSignals(false);

	mUI.mObjCenterXCmB->blockSignals(true);
	mUI.mObjCenterXCmB->setCurrentIndex(int(invertXFoc));
	mUI.mObjCenterXCmB->blockSignals(false);

	mUI.mObjCenterYCmB->blockSignals(true);
	mUI.mObjCenterYCmB->setCurrentIndex(int(invertYFoc));
	mUI.mObjCenterYCmB->blockSignals(false);
	QDialog::show();
}

void CameraSettingsDialog::onTurnChange(int pVal)
{
	NSCameraSystem * camSys = mEngine->system<NSCameraSystem>();
	camSys->setSensitivity(pVal, NSCameraSystem::Turn);
}

void CameraSettingsDialog::onMoveChange(int pVal)
{
	NSCameraSystem * camSys = mEngine->system<NSCameraSystem>();
	camSys->setSensitivity(pVal, NSCameraSystem::Strafe);
}

void CameraSettingsDialog::onCamCenterYCB(QString pStr)
{
	NSCameraSystem * camSys = mEngine->system<NSCameraSystem>();
	camSys->setyinv((pStr == "Inverted"));
}

void CameraSettingsDialog::onCamCenterXCB(QString pStr)
{
	NSCameraSystem * camSys = mEngine->system<NSCameraSystem>();
	camSys->setxinv((pStr == "Inverted"));
}

void CameraSettingsDialog::onObjCenterYCB(QString pStr)
{
	NSCameraSystem * camSys = mEngine->system<NSCameraSystem>();
	camSys->setyinv((pStr == "Inverted"),NSCameraSystem::Focus);
}

void CameraSettingsDialog::onObjCenterXCB(QString pStr)
{
	NSCameraSystem * camSys = mEngine->system<NSCameraSystem>();
	camSys->setxinv((pStr == "Inverted"), NSCameraSystem::Focus);
}