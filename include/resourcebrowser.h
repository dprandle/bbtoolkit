#ifndef RESOURCE_BROWSER_H
#define RESOURCE_BROWSER_H

#include <qmainwindow.h>
#include <ui_resourcebrowser.h>
#include <vector>

class Toolkit;
class NewResourceDialog;

class ResourceBrowser : public QMainWindow
{
	Q_OBJECT
	
  public:
	ResourceBrowser(QWidget * parent = NULL);
	~ResourceBrowser();

	void init(Toolkit * tk);
	NewResourceDialog * newResourceDialog();

  public slots:
	void onCreateEntity();
	void onCreateMap();
	void onCreatePlugin();
	void onCreateShader();
	void onCreateMaterial();
	void onCreateTexture();
	void onCreateAnimation();
	void onCreateMesh();
	void onEditResource();
	void onCreateResource();
	void onDeleteResource();
	
private:
	Toolkit * mTK;
	NewResourceDialog * mNewResDialog;
	Ui::ResourceBrowser mUI;
};


#endif
