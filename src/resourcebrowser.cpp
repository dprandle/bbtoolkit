#include <resourcebrowser.h>
#include <toolkit.h>
#include <newresourcedialog.h>

ResourceBrowser::ResourceBrowser(QWidget * parent) :
	mTK(NULL),
	mNewResDialog(new NewResourceDialog(this)),
	QMainWindow(parent)
{
	mUI.setupUi(this);
}

ResourceBrowser::~ResourceBrowser()
{
	delete mNewResDialog;
}

void ResourceBrowser::init(Toolkit * tk)
{
	mTK = tk;
	mNewResDialog->init(mTK);
}

NewResourceDialog * ResourceBrowser::newResourceDialog()
{
	return mNewResDialog;
}

void ResourceBrowser::onCreateEntity()
{
	
}

void ResourceBrowser::onCreateMap()
{
	
}

void ResourceBrowser::onCreatePlugin()
{
	
}

void ResourceBrowser::onCreateShader()
{
	
}

void ResourceBrowser::onCreateMaterial()
{
	mNewResDialog->setResourceType<NSMaterial>();
	if (mNewResDialog->exec() == QDialog::Accepted)
	{
		// add material to engine main context
	}
	
}

void ResourceBrowser::onCreateTexture()
{
	
}

void ResourceBrowser::onCreateAnimation()
{
	
}

void ResourceBrowser::onCreateMesh()
{
	
}

void ResourceBrowser::onEditResource()
{
	
}

void ResourceBrowser::onCreateResource()
{
	
}

void ResourceBrowser::onDeleteResource()
{
	
}
