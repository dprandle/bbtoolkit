#ifndef PREVIEW_H
#define PREVIEW_H


// Includes
#include <qopenglwidget.h>
#include <toolkitdef.h>
#include <nstypes.h>

class Toolkit;

class Preview : public QOpenGLWidget
{
	Q_OBJECT
  public:
	Preview(QWidget * parent=NULL);
	~Preview();

    void init();
						  
  public slots:
	void onIdle();

  signals:
    void opengl_initialized(uint32);

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
	
    uint32 mEngineContextID;
	void _connect();
};

#endif // MAPWINDOW_H
