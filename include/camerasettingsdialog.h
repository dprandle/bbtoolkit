#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H
#include <qdialog.h>
#include <ui_camerasettings.h>

class NSEngine;
class Toolkit;

class CameraSettingsDialog : public QDialog
{
	Q_OBJECT
public:
	CameraSettingsDialog(QWidget * pParent=NULL);
	~CameraSettingsDialog();

	void init(Toolkit * pTK);
	void showIt();

public slots:
	void onTurnChange(int);
	void onMoveChange(int);
	void onCamCenterYCB(QString);
	void onCamCenterXCB(QString);
	void onObjCenterYCB(QString);
	void onObjCenterXCB(QString);

private:
	NSEngine * mEngine;
	Toolkit * mTK;
	Ui_CameraSettingsDialog mUI;
};

#endif