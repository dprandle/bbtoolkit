/*!
  \file   newresourcedialog.cpp
  \author  <DreamInColor@DAN-LAPTOP>
  \date   Sun Jul 12 20:31:40 2015
  
  \brief  Dialog for creating new resource
*/

#include <newresourcedialog.h>
#include <toolkit.h>
#include <qlayout.h>
#include <materialwidget.h>
#include <nsengine.h>
#include <nsplugin.h>

NewResourceDialog::NewResourceDialog(QWidget * parent):
	mTK(NULL),
	mContextID(0),
	gbLayout(new QVBoxLayout),
	QDialog(parent)
{
	mUI.setupUi(this);
	mUI.mPropertiesGB->setLayout(gbLayout);
	// Here we should add all of the resource manager widgets
	mResWidgets[NSMaterial::getManagerTypeString()] = new MaterialWidget(this);
}

NewResourceDialog::~NewResourceDialog()
{}


void NewResourceDialog::init(Toolkit * tk)
{
	mTK = tk;
	mUI.mPreviewWidget->ui().mPreview->init(tk);
	connect(mUI.mPreviewWidget->ui().mPreview, SIGNAL(opengl_initialized(nsuint)), this, SLOT(onContextInitialized(nsuint)));
}

void NewResourceDialog::_setResourceType(const nsstring & manager_type, const nsstring & res_type)
{
	
	// create new plugin with same name as the main context active plugin
	auto fiter = mResWidgets.find(manager_type);
	if (fiter == mResWidgets.end())
		return;
	mCurrentResWidget = fiter->second;
	
	// Remove all widgets from groub box layout
	QLayoutItem * child = gbLayout->takeAt(0);
	while (child != NULL)
	{
		delete child;
		child = gbLayout->takeAt(0);
	}

	// Add the widget to the group box layout
	gbLayout->addWidget(fiter->second);

	if (mContextID == 0)
	{
		mTmpResType = res_type;
		mTmpManagerType = manager_type;
		return;
	}

	mUI.mPreviewWidget->ui().mPreview->makeCurrent();
	nsengine.makeCurrent(mContextID);
	NSPlugin * plug = nsengine.plugin("preview");
	NSResource * res = plug->manager(manager_type)->create(res_type,mCurrentResWidget->defaultResName());
	mCurrentResWidget->setResource(res);
}

void NewResourceDialog::onContextInitialized(nsuint id)
{
	if (mCurrentResWidget == NULL)
		return;
	
	mContextID = id;
	// Make the resource now that the engine has been initialized
	mUI.mPreviewWidget->ui().mPreview->makeCurrent();
	nsengine.makeCurrent(mContextID);
	NSPlugin * plug = nsengine.plugin("preview");
	NSResource * res = plug->manager(mTmpManagerType)->create(mTmpResType,mCurrentResWidget->defaultResName());
	mCurrentResWidget->setResource(res);

}


void NewResourceDialog::onCreate()
{
	
}

void NewResourceDialog::onCancel()
{
	NSResource * res = mCurrentResWidget->resource();
	mCurrentResWidget->setResource(NULL);
	mCurrentResWidget = NULL;

	nsengine.delResource(res);
}
