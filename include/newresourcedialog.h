/*!
  \file   newresourcedialog.h
  \author  <DreamInColor@DAN-LAPTOP>
  \date   Sun Jul 12 01:33:35 2015
  
  \brief  Dialog for creating new resources
*/

#ifndef NEWRESOURCEDIALOG_H
#define NEWRESOURCEDIALOG_H

#include <qdialog.h>
#include <ui_newresourcedialog.h>
#include <nstypes.h>
#include <nsstring.h>

//#include <nsengine.h>
//#include <nsmaterial.h>
//#include <nsfactory.h>

class Toolkit;
class QVBoxLayout;
class ResourceWidget;

class NewResourceDialog : public QDialog
{
	Q_OBJECT
	
  public:

	typedef std::map<nsstring, ResourceWidget*> ResourceWidgetMap;
	
    NewResourceDialog(QWidget * parent=NULL);
    virtual ~NewResourceDialog();
	
    void init();

//	template<class T>
//	ResourceWidget * resourceWidget()
//	{
//		auto fiter = mResWidgets.find(T::getManagerTypeString());
//		if (fiter != mResWidgets.end())
//			return fiter->second;
//		return NULL;
//	}

//	template<class ResType>
//	void setResourceType()
//	{
//		auto fac = nse.factory<NSResFactory, ResType>();
//		auto restp = type_to_guid(ResType);
//		auto rmstr = hash_to_guid(nse.managerID(restp));
		
//		_setResourceType(rmstr, restp);
//	}

  public slots:

	void onCreate();
	void onCancel();
	void onContextInitialized(uint32);
 
  private:

	void _setResourceType(const nsstring & manager_type, const nsstring & res_type);

	QVBoxLayout * gbLayout;
	
	ResourceWidget * mCurrentResWidget;
	uint32 mContextID;
	Ui::NewResourceDialog mUI;

	nsstring mTmpResType;
	nsstring mTmpManagerType;
	
	ResourceWidgetMap mResWidgets;	
};


#endif
