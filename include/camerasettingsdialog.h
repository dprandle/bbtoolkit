#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H
#include <qdialog.h>
#include <ui_camerasettings.h>

class CameraSettingsDialog : public QDialog
{
	Q_OBJECT
public:
	CameraSettingsDialog(QWidget * pParent=NULL);
	~CameraSettingsDialog();

	void showIt();

public slots:
	void onTurnChange(int);
	void onMoveChange(int);
	void onCamCenterYCB(QString);
	void onCamCenterXCB(QString);
	void onObjCenterYCB(QString);
	void onObjCenterXCB(QString);

private:
	Ui_CameraSettingsDialog mUI;
};

#endif
