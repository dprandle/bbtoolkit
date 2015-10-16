#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <qwidget.h>
#include <ui_previewwidget.h>

class PreviewWidget : public QWidget
{
  public:
    PreviewWidget(QWidget * parent=NULL);
    virtual ~PreviewWidget();
	Ui::PreviewWidget & ui();
  private:
	Ui::PreviewWidget mUI;
};


#endif
