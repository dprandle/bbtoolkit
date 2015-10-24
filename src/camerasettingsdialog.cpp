#include <camerasettingsdialog.h>
#include <nsengine.h>
#include <nscamera_system.h>

CameraSettingsDialog::CameraSettingsDialog(QWidget * pParent) :
QDialog(pParent)
{
    mUI.setupUi(this);
}

CameraSettingsDialog::~CameraSettingsDialog()
{}

void CameraSettingsDialog::showIt()
{
    nscamera_system * camSys = nse.system<nscamera_system>();
    int turn = camSys->sensitivity(nscamera_system::sens_turn);
    int move = camSys->sensitivity(nscamera_system::sens_strafe);
    bool invertXFree = camSys->x_inverted();
    bool invertYFree = camSys->y_inverted();
    bool invertXFoc = camSys->x_inverted(nscamera_system::mode_focus);
    bool invertYFoc = camSys->y_inverted(nscamera_system::mode_focus);

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
    nscamera_system * camSys = nse.system<nscamera_system>();
    camSys->set_sensitivity(pVal, nscamera_system::sens_turn);
}

void CameraSettingsDialog::onMoveChange(int pVal)
{
    nscamera_system * camSys = nse.system<nscamera_system>();
    camSys->set_sensitivity(pVal, nscamera_system::sens_strafe);
}

void CameraSettingsDialog::onCamCenterYCB(QString pStr)
{
    nscamera_system * camSys = nse.system<nscamera_system>();
    camSys->invert_y(pStr == "Inverted");
}

void CameraSettingsDialog::onCamCenterXCB(QString pStr)
{
    nscamera_system * camSys = nse.system<nscamera_system>();
    camSys->invert_x(pStr == "Inverted");
}

void CameraSettingsDialog::onObjCenterYCB(QString pStr)
{
    nscamera_system * camSys = nse.system<nscamera_system>();
    camSys->invert_y((pStr == "Inverted"),nscamera_system::mode_focus);
}

void CameraSettingsDialog::onObjCenterXCB(QString pStr)
{
    nscamera_system * camSys = nse.system<nscamera_system>();
    camSys->invert_x((pStr == "Inverted"), nscamera_system::mode_focus);
}
