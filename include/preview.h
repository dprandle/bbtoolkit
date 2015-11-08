#ifndef PREVIEW_H
#define PREVIEW_H


// Includes
#include <qopenglwidget.h>
#include <toolkitdef.h>
#include <nstypes.h>
#include <nsmath.h>

class Toolkit;

class Preview : public QOpenGLWidget
{
	Q_OBJECT
  public:
	Preview(QWidget * parent=NULL);
	~Preview();

    void init();

    void make_current();

    uint32 glew_id();
						  
  public slots:
	void onIdle();

  signals:
    void opengl_initialized();
    void opengl_updated();
    void opengl_resized();

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
    bool _on_key(bool pressed, int key);
    bool _on_mouse(bool pressed, int mbutton, const fvec2 & norm_mpos);

    uint32 m_glew_id;
	void _connect();
};

#endif // MAPWINDOW_H
