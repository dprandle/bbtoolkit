#ifndef PREVIEW_H
#define PREVIEW_H


// Includes
#include <myGL\glew.h>
#include <qopenglwidget.h>
#include <toolkitdef.h>
#include <nsglobal.h>
#include <nsmath.h>
#include <nstformcomp.h>

class Toolkit;

class Preview : public QOpenGLWidget
{
	Q_OBJECT
  public:
	Preview(QWidget * parent=NULL);
	~Preview();

	void init(Toolkit * pTK);
						  
  public slots:
	void onIdle();

  signals:
	void opengl_initialized(nsuint);

  protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

	void enterEvent(QEvent * pEevent);
	void keyPressEvent(QKeyEvent * keyEvent);
	void keyReleaseEvent(QKeyEvent * keyEvent);
	void mouseMoveEvent(QMouseEvent * mevent);
	void mousePressEvent(QMouseEvent * mevent);
	void mouseReleaseEvent(QMouseEvent * mevent);
	void mouseDoubleClickEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * wevent);

  private:
	
	nsuint mEngineContextID;
	void _connect();
	Toolkit * mTK;
};

#endif // MAPWINDOW_H
