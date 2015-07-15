/*!
  \file   resourceinfowidget.h
  \author  <DreamInColor@DAN-LAPTOP>
  \date   Mon Jul 13 01:17:19 2015
  
  \brief  Resource information widget
  
  
*/


#ifndef RESOURCE_INFO_WIDGET_H
#define RESOURCE_INFO_WIDGET_H

#include <qwidget.h>
#include <ui_resourceinfowidget.h>

class ResourceInfoWidget : public QWidget
{
  public:
    ResourceInfoWidget(QWidget * parent=NULL);
    virtual ~ResourceInfoWidget();
  private:
	Ui::ResourceInfoWidget mUI;
};


#endif
