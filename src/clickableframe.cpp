#include <clickableframe.h>
#include <qpushbutton.h>

ClickableFrame::ClickableFrame(QWidget * parent) :
QFrame(parent)
{
	
}

ClickableFrame::~ClickableFrame()
{
	QPushButton b;
	
}

void ClickableFrame::mousePressEvent(QMouseEvent * event)
{
	emit clicked();
}
