/*!
  \file   newresourcedialog.cpp
  \author  <DreamInColor@DAN-LAPTOP>
  \date   Sun Jul 12 20:31:40 2015
  
  \brief  Dialog for creating new resource
*/

#include <nsengine.h>
#include <nsplugin.h>
#include <nsmat_manager.h>

#include <newresourcedialog.h>
#include <toolkit.h>
#include <qlayout.h>
#include <materialwidget.h>


NewResourceDialog::NewResourceDialog(QWidget * parent):
	mContextID(0),
	gbLayout(new QVBoxLayout),
	QDialog(parent)
{
	mUI.setupUi(this);
	mUI.mPropertiesGB->setLayout(gbLayout);
	// Here we should add all of the resource manager widgets
	nsstring rm = type_to_guid(nsmat_manager);
	mResWidgets[rm] = new MaterialWidget(this);
}

NewResourceDialog::~NewResourceDialog()
{}


void NewResourceDialog::init()
{
    mUI.mPreviewWidget->ui().mPreview->init();
	connect(mUI.mPreviewWidget->ui().mPreview, SIGNAL(opengl_initialized(uint32)), this, SLOT(onContextInitialized(uint32)));
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
	nse.make_current(mContextID);
	nsplugin * plug = nse.plugin("preview");
	nsresource * res = plug->manager(manager_type)->create(res_type,mCurrentResWidget->defaultResName());
	mCurrentResWidget->setResource(res);
}

void NewResourceDialog::onContextInitialized(uint32 id)
{
	if (mCurrentResWidget == NULL)
		return;
	
	mContextID = id;
	// Make the resource now that the engine has been initialized
	mUI.mPreviewWidget->ui().mPreview->makeCurrent();
	nse.make_current(mContextID);
	nsplugin * plug = nse.plugin("preview");
	nsresource * res = plug->manager(mTmpManagerType)->create(mTmpResType,mCurrentResWidget->defaultResName());
	mCurrentResWidget->setResource(res);

}


void NewResourceDialog::onCreate()
{
	
}

void NewResourceDialog::onCancel()
{
	nsresource * res = mCurrentResWidget->resource();
	mCurrentResWidget->setResource(NULL);
	mCurrentResWidget = NULL;
    nsplugin * plg = nse.plugin(res->plugin_id());
	plg->destroy(res);
}
